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
#include "net/http_client.h"
#include "storages/http_abstract_invoke.h"

class rpcClient {
	public:
        bool init(std::string address = "http://localhost:31931",
                  boost::optional<epee::net_utils::http::login> login = boost::none, uint64_t upper_transaction_size_limit = 0, bool ssl = false);
        bool deinit();

        template<class t_request, class t_response>
        inline bool invoke_http_json(const boost::string_ref uri, const t_request& req, t_response& res, std::chrono::milliseconds timeout = std::chrono::seconds(15), const boost::string_ref http_method = "GET")
        {
            boost::lock_guard<boost::mutex> lock(m_daemon_rpc_mutex);
            return epee::net_utils::invoke_http_json(uri, req, res, m_http_client, timeout, http_method);
        }
        template<class t_request, class t_response>
        inline bool invoke_http_bin(const boost::string_ref uri, const t_request& req, t_response& res, std::chrono::milliseconds timeout = std::chrono::seconds(15), const boost::string_ref http_method = "GET")
        {
            boost::lock_guard<boost::mutex> lock(m_daemon_rpc_mutex);
            return epee::net_utils::invoke_http_bin(uri, req, res, m_http_client, timeout, http_method);
        }
        template<class t_request, class t_response>
        inline bool invoke_http_json_rpc(const boost::string_ref uri, const std::string& method_name, const t_request& req, t_response& res, std::chrono::milliseconds timeout = std::chrono::seconds(15), const boost::string_ref http_method = "GET", const std::string& req_id = "0")
        {
            boost::lock_guard<boost::mutex> lock(m_daemon_rpc_mutex);
            return epee::net_utils::invoke_http_json_rpc(uri, method_name, req, res, m_http_client, timeout, http_method, req_id);
        }
    private: 
        epee::net_utils::http::http_simple_client m_http_client;
        boost::optional<epee::net_utils::http::login> m_daemon_login;
        std::string m_daemon_address;
        bool m_is_initialized;
        boost::mutex m_daemon_rpc_mutex;
};