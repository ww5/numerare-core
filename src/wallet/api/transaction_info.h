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

#include "wallet/api/wallet2_api.h"
#include <string>
#include <ctime>

namespace Monero {

class TransactionHistoryImpl;

class TransactionInfoImpl : public TransactionInfo
{
public:
    TransactionInfoImpl();
    ~TransactionInfoImpl();
    //! in/out
    virtual int direction() const;
    //! true if hold
    virtual bool isPending() const;
    virtual bool isFailed() const;
    virtual uint64_t amount() const;
    //! always 0 for incoming txes
    virtual uint64_t fee() const;
    virtual uint64_t blockHeight() const;
    virtual std::set<uint32_t> subaddrIndex() const;
    virtual uint32_t subaddrAccount() const;
    virtual std::string label() const;

    virtual std::string hash() const;
    virtual std::time_t timestamp() const;
    virtual std::string paymentId() const;
    virtual const std::vector<Transfer> &transfers() const;
    virtual uint64_t confirmations() const;
    virtual uint64_t unlockTime() const;

private:
    int         m_direction;
    bool        m_pending;
    bool        m_failed;
    uint64_t    m_amount;
    uint64_t    m_fee;
    uint64_t    m_blockheight;
    std::set<uint32_t> m_subaddrIndex;        // always unique index for incoming transfers; can be multiple indices for outgoing transfers
    uint32_t m_subaddrAccount;
    std::string m_label;
    std::string m_hash;
    std::time_t m_timestamp;
    std::string m_paymentid;
    std::vector<Transfer> m_transfers;
    uint64_t    m_confirmations;
    uint64_t    m_unlock_time;

    friend class TransactionHistoryImpl;

};

} // namespace

namespace Bitmonero = Monero;
