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

#include "subaddress_account.h"
#include "wallet.h"
#include "crypto/hash.h"
#include "wallet/wallet2.h"
#include "common_defines.h"

#include <vector>

namespace Monero {
  
SubaddressAccount::~SubaddressAccount() {}
  
SubaddressAccountImpl::SubaddressAccountImpl(WalletImpl *wallet)
    : m_wallet(wallet) {}

void SubaddressAccountImpl::addRow(const std::string &label)
{
  m_wallet->m_wallet->add_subaddress_account(label);
  refresh();
}

void SubaddressAccountImpl::setLabel(uint32_t accountIndex, const std::string &label)
{
  m_wallet->m_wallet->set_subaddress_label({accountIndex, 0}, label);
  refresh();
}

void SubaddressAccountImpl::refresh() 
{
  LOG_PRINT_L2("Refreshing subaddress account");
  
  clearRows();
  for (uint32_t i = 0; i < m_wallet->m_wallet->get_num_subaddress_accounts(); ++i)
  {
    m_rows.push_back(new SubaddressAccountRow(
      i,
      m_wallet->m_wallet->get_subaddress_as_str({i,0}).substr(0,6),
      m_wallet->m_wallet->get_subaddress_label({i,0}),
      cryptonote::print_money(m_wallet->m_wallet->balance(i)),
      cryptonote::print_money(m_wallet->m_wallet->unlocked_balance(i))
    ));
  }
}

void SubaddressAccountImpl::clearRows() {
   for (auto r : m_rows) {
     delete r;
   }
   m_rows.clear();
}

std::vector<SubaddressAccountRow*> SubaddressAccountImpl::getAll() const
{
  return m_rows;
}

SubaddressAccountImpl::~SubaddressAccountImpl()
{
  clearRows();
}

} // namespace
