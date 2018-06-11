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
#include <boost/thread/shared_mutex.hpp>

namespace Monero {

class WalletImpl;

class TransactionHistoryImpl : public TransactionHistory
{
public:
    TransactionHistoryImpl(WalletImpl * wallet);
    ~TransactionHistoryImpl();
    virtual int count() const;
    virtual TransactionInfo * transaction(int index)  const;
    virtual TransactionInfo * transaction(const std::string &id) const;
    virtual std::vector<TransactionInfo*> getAll() const;
    virtual void refresh();

private:

    // TransactionHistory is responsible of memory management
    std::vector<TransactionInfo*> m_history;
    WalletImpl *m_wallet;
    mutable boost::shared_mutex   m_historyMutex;
};

}

namespace Bitmonero = Monero;

