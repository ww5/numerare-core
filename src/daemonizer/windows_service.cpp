/***
	MIT License

	Copyright (c) 2018 NUMERARE

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

	Parts of this file are originally Copyright (c) 2012-2014 The CryptoNote developers,
	                                                2014-2018 The Monero Project
***/

#include <boost/chrono/chrono.hpp>
#include <boost/thread/thread.hpp>

#undef UNICODE
#undef _UNICODE

#include "common/scoped_message_writer.h"
#include "daemonizer/windows_service.h"
#include "string_tools.h"
#include <chrono>
#include <iostream>
#include <utility>
#include <memory>
#include <shellapi.h>
#include <thread>
#include <windows.h>

namespace windows {

namespace {
  typedef std::unique_ptr<std::remove_pointer<SC_HANDLE>::type, decltype(&::CloseServiceHandle)> service_handle;

  std::string get_last_error()
  {
    LPSTR p_error_text = nullptr;

    FormatMessage(
      FORMAT_MESSAGE_FROM_SYSTEM
    | FORMAT_MESSAGE_ALLOCATE_BUFFER
    | FORMAT_MESSAGE_IGNORE_INSERTS
    , nullptr
    , GetLastError()
    , MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
    , reinterpret_cast<LPSTR>(&p_error_text)
    , 0
    , nullptr
    );

    if (nullptr == p_error_text)
    {
      return "";
    }
    else
    {
      return std::string{p_error_text};
      LocalFree(p_error_text);
    }
  }

  bool relaunch_as_admin(
      std::string const & command
    , std::string const & arguments
    )
  {
    SHELLEXECUTEINFO info{};
    info.cbSize = sizeof(info);
    info.lpVerb = "runas";
    info.lpFile = command.c_str();
    info.lpParameters = arguments.c_str();
    info.hwnd = nullptr;
    info.nShow = SW_SHOWNORMAL;
    if (!ShellExecuteEx(&info))
    {
      tools::fail_msg_writer() << "Admin relaunch failed: " << get_last_error();
      return false;
    }
    else
    {
      return true;
    }
  }

  // When we relaunch as admin, Windows opens a new window.  This just pauses
  // to allow the user to read any output.
  void pause_to_display_admin_window_messages()
  {
    boost::chrono::milliseconds how_long{1500};
    boost::this_thread::sleep_for(how_long);
  }
}

bool check_admin(bool & result)
{
  BOOL is_admin = FALSE;
  PSID p_administrators_group = nullptr;

  SID_IDENTIFIER_AUTHORITY nt_authority = SECURITY_NT_AUTHORITY;

  if (!AllocateAndInitializeSid(
        &nt_authority
      , 2
      , SECURITY_BUILTIN_DOMAIN_RID
      , DOMAIN_ALIAS_RID_ADMINS
      , 0, 0, 0, 0, 0, 0
      , &p_administrators_group
      ))
  {
    tools::fail_msg_writer() << "Security Identifier creation failed: " << get_last_error();
    return false;
  }

  if (!CheckTokenMembership(
        nullptr
      , p_administrators_group
      , &is_admin
      ))
  {
    tools::fail_msg_writer() << "Permissions check failed: " << get_last_error();
    return false;
  }

  result = is_admin ? true : false;

  return true;
}

bool ensure_admin(
    std::string const & arguments
  )
{
  bool admin;

  if (!check_admin(admin))
  {
    return false;
  }

  if (admin)
  {
    return true;
  }
  else
  {
    std::string command = epee::string_tools::get_current_module_path();
    relaunch_as_admin(command, arguments);
    return false;
  }
}

bool install_service(
    std::string const & service_name
  , std::string const & arguments
  )
{
  std::string command = epee::string_tools::get_current_module_path();
  std::string full_command = command + arguments;

  service_handle p_manager{
    OpenSCManager(
        nullptr
      , nullptr
      , SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE
      )
  , &::CloseServiceHandle
  };
  if (p_manager == nullptr)
  {
    tools::fail_msg_writer() << "Couldn't connect to service manager: " << get_last_error();
    return false;
  }

  service_handle p_service{
    CreateService(
        p_manager.get()
      , service_name.c_str()
      , service_name.c_str()
      , 0
      //, GENERIC_EXECUTE | GENERIC_READ
      , SERVICE_WIN32_OWN_PROCESS
      , SERVICE_DEMAND_START
      , SERVICE_ERROR_NORMAL
      , full_command.c_str()
      , nullptr
      , nullptr
      , ""
      //, "NT AUTHORITY\\LocalService"
      , nullptr // Implies LocalSystem account
      , nullptr
      )
  , &::CloseServiceHandle
  };
  if (p_service == nullptr)
  {
    tools::fail_msg_writer() << "Couldn't create service: " << get_last_error();
    return false;
  }

  tools::success_msg_writer() << "Service installed";

  pause_to_display_admin_window_messages();

  return true;
}

bool start_service(
    std::string const & service_name
  )
{
  tools::msg_writer() << "Starting service";

  SERVICE_STATUS_PROCESS service_status = {};
  DWORD unused = 0;

  service_handle p_manager{
    OpenSCManager(
        nullptr
      , nullptr
      , SC_MANAGER_CONNECT
      )
  , &::CloseServiceHandle
  };
  if (p_manager == nullptr)
  {
    tools::fail_msg_writer() << "Couldn't connect to service manager: " << get_last_error();
    return false;
  }

  service_handle p_service{
    OpenService(
        p_manager.get()
      , service_name.c_str()
      //, SERVICE_START | SERVICE_QUERY_STATUS
      , SERVICE_START
      )
  , &::CloseServiceHandle
  };
  if (p_service == nullptr)
  {
    tools::fail_msg_writer() << "Couldn't find service: " << get_last_error();
    return false;
  }

  if (!StartService(
      p_service.get()
    , 0
    , nullptr
    ))
  {
    tools::fail_msg_writer() << "Service start request failed: " << get_last_error();
    return false;
  }

  tools::success_msg_writer() << "Service started";

  pause_to_display_admin_window_messages();

  return true;
}

bool stop_service(
    std::string const & service_name
  )
{
  tools::msg_writer() << "Stopping service";

  service_handle p_manager{
    OpenSCManager(
        nullptr
      , nullptr
      , SC_MANAGER_CONNECT
      )
  , &::CloseServiceHandle
  };
  if (p_manager == nullptr)
  {
    tools::fail_msg_writer() << "Couldn't connect to service manager: " << get_last_error();
    return false;
  }

  service_handle p_service{
    OpenService(
        p_manager.get()
      , service_name.c_str()
      , SERVICE_STOP | SERVICE_QUERY_STATUS
      )
  , &::CloseServiceHandle
  };
  if (p_service == nullptr)
  {
    tools::fail_msg_writer() << "Couldn't find service: " << get_last_error();
    return false;
  }

  SERVICE_STATUS status = {};
  if (!ControlService(p_service.get(), SERVICE_CONTROL_STOP, &status))
  {
    tools::fail_msg_writer() << "Couldn't request service stop: " << get_last_error();
    return false;
  }

  tools::success_msg_writer() << "Service stopped";

  pause_to_display_admin_window_messages();

  return true;
}

bool uninstall_service(
    std::string const & service_name
  )
{
  service_handle p_manager{
    OpenSCManager(
        nullptr
      , nullptr
      , SC_MANAGER_CONNECT
      )
  , &::CloseServiceHandle
  };
  if (p_manager == nullptr)
  {
    tools::fail_msg_writer() << "Couldn't connect to service manager: " << get_last_error();
    return false;
  }

  service_handle p_service{
    OpenService(
        p_manager.get()
      , service_name.c_str()
      , SERVICE_QUERY_STATUS | DELETE
      )
  , &::CloseServiceHandle
  };
  if (p_service == nullptr)
  {
    tools::fail_msg_writer() << "Couldn't find service: " << get_last_error();
    return false;
  }

  SERVICE_STATUS status = {};
  if (!DeleteService(p_service.get()))
  {
    tools::fail_msg_writer() << "Couldn't uninstall service: " << get_last_error();
    return false;
  }

  tools::success_msg_writer() << "Service uninstalled";

  pause_to_display_admin_window_messages();

  return true;
}

} // namespace windows
