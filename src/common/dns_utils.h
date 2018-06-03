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

#include <vector>
#include <string>
#include <functional>

namespace tools
{

// RFC defines for record types and classes for DNS, gleaned from ldns source
const static int DNS_CLASS_IN  = 1;
const static int DNS_TYPE_A    = 1;
const static int DNS_TYPE_TXT  = 16;
const static int DNS_TYPE_AAAA = 8;

struct DNSResolverData;

/**
 * @brief Provides high-level access to DNS resolution
 *
 * This class is designed to provide a high-level abstraction to DNS resolution
 * functionality, including access to TXT records and such.  It will also
 * handle DNSSEC validation of the results.
 */
class DNSResolver
{
private:

  /**
   * @brief Constructs an instance of DNSResolver
   *
   * Constructs a class instance and does setup stuff for the backend resolver.
   */
  DNSResolver();

public:

  /**
   * @brief takes care of freeing C pointers and such
   */
  ~DNSResolver();

  /**
   * @brief gets ipv4 addresses from DNS query of a URL
   *
   * returns a vector of all IPv4 "A" records for given URL.
   * If no "A" records found, returns an empty vector.
   *
   * @param url A string containing a URL to query for
   *
   * @param dnssec_available 
   *
   * @return vector of strings containing ipv4 addresses
   */
  std::vector<std::string> get_ipv4(const std::string& url, bool& dnssec_available, bool& dnssec_valid);

  /**
   * @brief gets ipv6 addresses from DNS query
   *
   * returns a vector of all IPv6 "A" records for given URL.
   * If no "A" records found, returns an empty vector.
   *
   * @param url A string containing a URL to query for
   *
   * @return vector of strings containing ipv6 addresses
   */
   std::vector<std::string> get_ipv6(const std::string& url, bool& dnssec_available, bool& dnssec_valid);

  /**
   * @brief gets all TXT records from a DNS query for the supplied URL;
   * if no TXT record present returns an empty vector.
   *
   * @param url A string containing a URL to query for
   *
   * @return A vector of strings containing a TXT record; or an empty vector
   */
  // TODO: modify this to accommodate DNSSEC
   std::vector<std::string> get_txt_record(const std::string& url, bool& dnssec_available, bool& dnssec_valid);

  /**
   * @brief Gets a DNS address from OpenAlias format
   *
   * If the address looks good, but contains one @ symbol, replace that with a .
   * e.g. donate@getmonero.org becomes donate.getmonero.org
   *
   * @param oa_addr  OpenAlias address
   *
   * @return dns_addr  DNS address
   */
  std::string get_dns_format_from_oa_address(const std::string& oa_addr);

  /**
   * @brief Gets the singleton instance of DNSResolver
   *
   * @return returns a pointer to the singleton
   */
  static DNSResolver& instance();

  /**
   * @brief Gets a new instance of DNSResolver
   *
   * @return returns a pointer to the new object
   */
  static DNSResolver create();

private:

  /**
   * @brief gets all records of a given type from a DNS query for the supplied URL;
   * if no such record is present returns an empty vector.
   *
   * @param url A string containing a URL to query for
   * @param record_type the record type to retrieve (DNS_TYPE_A, etc)
   * @param reader a function that converts a record data to a string
   *
   * @return A vector of strings containing the requested record; or an empty vector
   */
  // TODO: modify this to accommodate DNSSEC
  std::vector<std::string> get_record(const std::string& url, int record_type, std::string (*reader)(const char *,size_t), bool& dnssec_available, bool& dnssec_valid);

  /**
   * @brief Checks a string to see if it looks like a URL
   *
   * @param addr the string to be checked
   *
   * @return true if it looks enough like a URL, false if not
   */
  bool check_address_syntax(const char *addr) const;

  DNSResolverData *m_data;
}; // class DNSResolver

namespace dns_utils
{

std::string address_from_txt_record(const std::string& s);
std::vector<std::string> addresses_from_url(const std::string& url, bool& dnssec_valid);

std::string get_account_address_as_str_from_url(const std::string& url, bool& dnssec_valid, std::function<std::string(const std::string&, const std::vector<std::string>&, bool)> confirm_dns);

bool load_txt_records_from_dns(std::vector<std::string> &records, const std::vector<std::string> &dns_urls);

std::vector<std::string> parse_dns_public(const char *s);

}  // namespace tools::dns_utils

}  // namespace tools