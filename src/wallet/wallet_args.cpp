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
#include "wallet/wallet_args.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/format.hpp>
#include "common/i18n.h"
#include "common/util.h"
#include "misc_log_ex.h"
#include "string_tools.h"
#include "version.h"

#if defined(WIN32)
#include <crtdbg.h>
#endif

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "wallet.wallet2"

// workaround for a suspected bug in pthread/kernel on MacOS X
#ifdef __APPLE__
#define DEFAULT_MAX_CONCURRENCY 1
#else
#define DEFAULT_MAX_CONCURRENCY 0
#endif

namespace
{
  class Print
  {
  public:
    Print(const std::function<void(const std::string&, bool)> &p, bool em = false): print(p), emphasis(em) {}
    ~Print() { print(ss.str(), emphasis); }
    template<typename T> std::ostream &operator<<(const T &t) { ss << t; return ss; }
  private:
    const std::function<void(const std::string&, bool)> &print;
    std::stringstream ss;
    bool emphasis;
  };
}

namespace wallet_args
{
  // Create on-demand to prevent static initialization order fiasco issues.
  command_line::arg_descriptor<std::string> arg_generate_from_json()
  {
    return {"generate-from-json", wallet_args::tr("Generate wallet from JSON format file"), ""};
  }
  command_line::arg_descriptor<std::string> arg_wallet_file()
  {
    return {"wallet-file", wallet_args::tr("Use wallet <arg>"), ""};
  }

  const char* tr(const char* str)
  {
    return i18n_translate(str, "wallet_args");
  }

  boost::optional<boost::program_options::variables_map> main(
    int argc, char** argv,
    const char* const usage,
    const char* const notice,
    boost::program_options::options_description desc_params,
    const boost::program_options::positional_options_description& positional_options,
    const std::function<void(const std::string&, bool)> &print,
    const char *default_log_name,
    bool log_to_console)
  
  {
    namespace bf = boost::filesystem;
    namespace po = boost::program_options;
#ifdef WIN32
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    const command_line::arg_descriptor<std::string> arg_log_level = {"log-level", "0-4 or categories", ""};
    const command_line::arg_descriptor<std::size_t> arg_max_log_file_size = {"max-log-file-size", "Specify maximum log file size [B]", MAX_LOG_FILE_SIZE};
    const command_line::arg_descriptor<uint32_t> arg_max_concurrency = {"max-concurrency", wallet_args::tr("Max number of threads to use for a parallel job"), DEFAULT_MAX_CONCURRENCY};
    const command_line::arg_descriptor<std::string> arg_log_file = {"log-file", wallet_args::tr("Specify log file"), ""};
    const command_line::arg_descriptor<std::string> arg_config_file = {"config-file", wallet_args::tr("Config file"), "", true};


    std::string lang = i18n_get_language();
    tools::on_startup();
    tools::set_strict_default_file_permissions(true);

    epee::string_tools::set_module_name_and_folder(argv[0]);

    po::options_description desc_general(wallet_args::tr("General options"));
    command_line::add_arg(desc_general, command_line::arg_help);
    command_line::add_arg(desc_general, command_line::arg_version);

    command_line::add_arg(desc_params, arg_log_file);
    command_line::add_arg(desc_params, arg_log_level);
    command_line::add_arg(desc_params, arg_max_log_file_size);
    command_line::add_arg(desc_params, arg_max_concurrency);
    command_line::add_arg(desc_params, arg_config_file);

    i18n_set_language("translations", "NUMERARE", lang);

    po::options_description desc_all;
    desc_all.add(desc_general).add(desc_params);
    po::variables_map vm;
    bool r = command_line::handle_error_helper(desc_all, [&]()
    {
      auto parser = po::command_line_parser(argc, argv).options(desc_all).positional(positional_options);
      po::store(parser.run(), vm);

      if (command_line::get_arg(vm, command_line::arg_help))
      {
        Print(print) << "NUMERARE '" << MONERO_RELEASE_NAME << "' (" << MONERO_VERSION_FULL << ")" << ENDL;
        Print(print) << wallet_args::tr("This is the command line monero wallet. It needs to connect to a monero\n"
												  "daemon to work correctly.") << ENDL;
        Print(print) << wallet_args::tr("Usage:") << ENDL << "  " << usage;
        Print(print) << desc_all;
        return false;
      }
      else if (command_line::get_arg(vm, command_line::arg_version))
      {
        Print(print) << "NUMERARE '" << MONERO_RELEASE_NAME << "' (" << MONERO_VERSION_FULL << ")";
        return false;
      }

      if(command_line::has_arg(vm, arg_config_file))
      {
        std::string config = command_line::get_arg(vm, arg_config_file);
        bf::path config_path(config);
        boost::system::error_code ec;
        if (bf::exists(config_path, ec))
        {
          po::store(po::parse_config_file<char>(config_path.string<std::string>().c_str(), desc_params), vm);
        }
        else
        {
          MERROR(wallet_args::tr("Can't find config file ") << config);
          return false;
        }
      }

      po::notify(vm);
      return true;
    });
    if (!r)
      return boost::none;

    std::string log_path;
    if (!command_line::is_arg_defaulted(vm, arg_log_file))
      log_path = command_line::get_arg(vm, arg_log_file);
    else
      log_path = mlog_get_default_log_path(default_log_name);
    mlog_configure(log_path, log_to_console, command_line::get_arg(vm, arg_max_log_file_size));
    if (!command_line::is_arg_defaulted(vm, arg_log_level))
    {
      mlog_set_log(command_line::get_arg(vm, arg_log_level).c_str());
    }

    if (notice)
      Print(print) << notice << ENDL;

    if (!command_line::is_arg_defaulted(vm, arg_max_concurrency))
      tools::set_max_concurrency(command_line::get_arg(vm, arg_max_concurrency));

    Print(print) << "NUMERARE '" << MONERO_RELEASE_NAME << "' (" << MONERO_VERSION_FULL << ")";

    if (!command_line::is_arg_defaulted(vm, arg_log_level))
      MINFO("Setting log level = " << command_line::get_arg(vm, arg_log_level));
    else
      MINFO("Setting log levels = " << getenv("MONERO_LOGS"));
    MINFO(wallet_args::tr("Logging to: ") << log_path);

    Print(print) << boost::format(wallet_args::tr("Logging to %s")) % log_path;

    return {std::move(vm)};
  }
}
