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

#include "transaction_info.h"


using namespace std;

namespace Monero {

TransactionInfo::~TransactionInfo() {}

TransactionInfo::Transfer::Transfer(uint64_t _amount, const string &_address)
    : amount(_amount), address(_address) {}


TransactionInfoImpl::TransactionInfoImpl()
    : m_direction(Direction_Out)
      , m_pending(false)
      , m_failed(false)
      , m_amount(0)
      , m_fee(0)
      , m_blockheight(0)
      , m_subaddrAccount(0)
      , m_timestamp(0)
      , m_confirmations(0)
      , m_unlock_time(0)
{

}

TransactionInfoImpl::~TransactionInfoImpl()
{

}

int TransactionInfoImpl::direction() const
{
    return m_direction;
}


bool TransactionInfoImpl::isPending() const
{
    return m_pending;
}

bool TransactionInfoImpl::isFailed() const
{
    return m_failed;
}

uint64_t TransactionInfoImpl::amount() const
{
    return m_amount;
}

uint64_t TransactionInfoImpl::fee() const
{
    return m_fee;
}

uint64_t TransactionInfoImpl::blockHeight() const
{
    return m_blockheight;
}

std::set<uint32_t> TransactionInfoImpl::subaddrIndex() const
{
    return m_subaddrIndex;
}

uint32_t TransactionInfoImpl::subaddrAccount() const
{
    return m_subaddrAccount;
}

string TransactionInfoImpl::label() const
{
    return m_label;
}


string TransactionInfoImpl::hash() const
{
    return m_hash;
}

std::time_t TransactionInfoImpl::timestamp() const
{
    return m_timestamp;
}

string TransactionInfoImpl::paymentId() const
{
    return m_paymentid;
}

const std::vector<TransactionInfo::Transfer> &TransactionInfoImpl::transfers() const
{
    return m_transfers;
}

uint64_t TransactionInfoImpl::confirmations() const
{
    return m_confirmations;
}

uint64_t TransactionInfoImpl::unlockTime() const
{
    return m_unlock_time;
}

} // namespace

namespace Bitmonero = Monero;
