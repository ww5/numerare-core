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

#pragma once

#include "rapidjson/document.h"
#include "rpc/message_data_structs.h"
#include <string>

/* I normally hate using macros, but in this case it would be untenably
 * verbose to not use a macro.  This macro saves the trouble of explicitly
 * writing the below if block for every single RPC call.
 */
#define REQ_RESP_TYPES_MACRO( runtime_str, type, reqjson, resp_message_ptr, handler) \
  \
  if (runtime_str == type::name) \
  { \
    type::Request reqvar; \
    type::Response *respvar = new type::Response(); \
    \
    reqvar.fromJson(reqjson); \
    \
    handler(reqvar, *respvar); \
    \
    resp_message_ptr = respvar; \
  }

namespace cryptonote
{

namespace rpc
{

  class Message
  {
    public:
      static const char* STATUS_OK;
      static const char* STATUS_RETRY;
      static const char* STATUS_FAILED;
      static const char* STATUS_BAD_REQUEST;
      static const char* STATUS_BAD_JSON;

      Message() : status(STATUS_OK) { }

      virtual ~Message() { }

      virtual rapidjson::Value toJson(rapidjson::Document& doc) const;

      virtual void fromJson(rapidjson::Value& val);

      std::string status;
      std::string error_details;
      uint32_t rpc_version;
  };

  class FullMessage
  {
    public:
      ~FullMessage() { }

      FullMessage(FullMessage&& rhs) noexcept : doc(std::move(rhs.doc)) { }

      FullMessage(const std::string& json_string, bool request=false);

      std::string getJson();

      std::string getRequestType() const;

      rapidjson::Value& getMessage();

      rapidjson::Value getMessageCopy();

      rapidjson::Value& getID();

      void setID(rapidjson::Value& id);

      cryptonote::rpc::error getError();

      static FullMessage requestMessage(const std::string& request, Message* message);
      static FullMessage requestMessage(const std::string& request, Message* message, rapidjson::Value& id);

      static FullMessage responseMessage(Message* message);
      static FullMessage responseMessage(Message* message, rapidjson::Value& id);

      static FullMessage* timeoutMessage();
    private:

      FullMessage() = default;

      FullMessage(const std::string& request, Message* message);
      FullMessage(Message* message);

      rapidjson::Document doc;
  };


  // convenience functions for bad input
  std::string BAD_REQUEST(const std::string& request);
  std::string BAD_REQUEST(const std::string& request, rapidjson::Value& id);

  std::string BAD_JSON(const std::string& error_details);


}  // namespace rpc

}  // namespace cryptonote
