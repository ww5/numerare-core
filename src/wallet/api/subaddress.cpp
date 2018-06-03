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

#include "subaddress.h"
#include "wallet.h"
#include "crypto/hash.h"
#include "wallet/wallet2.h"
#include "common_defines.h"

#include <vector>

namespace Monero {
  
Subaddress::~Subaddress() {}
  
SubaddressImpl::SubaddressImpl(WalletImpl *wallet)
    : m_wallet(wallet) {}

void SubaddressImpl::addRow(uint32_t accountIndex, const std::string &label)
{
  m_wallet->m_wallet->add_subaddress(accountIndex, label);
  refresh(accountIndex);
}

void SubaddressImpl::setLabel(uint32_t accountIndex, uint32_t addressIndex, const std::string &label)
{
  try
  {
    m_wallet->m_wallet->set_subaddress_label({accountIndex, addressIndex}, label);
    refresh(accountIndex);
  }
  catch (const std::exception& e)
  {
    LOG_ERROR("setLabel: " << e.what());
  }
}

void SubaddressImpl::refresh(uint32_t accountIndex) 
{
  LOG_PRINT_L2("Refreshing subaddress");
  
  clearRows();
  for (size_t i = 0; i < m_wallet->m_wallet->get_num_subaddresses(accountIndex); ++i)
  {
    m_rows.push_back(new SubaddressRow(i, m_wallet->m_wallet->get_subaddress_as_str({accountIndex, (uint32_t)i}), m_wallet->m_wallet->get_subaddress_label({accountIndex, (uint32_t)i})));
  }
}

void SubaddressImpl::clearRows() {
   for (auto r : m_rows) {
     delete r;
   }
   m_rows.clear();
}

std::vector<SubaddressRow*> SubaddressImpl::getAll() const
{
  return m_rows;
}

SubaddressImpl::~SubaddressImpl()
{
  clearRows();
}

} // namespace
