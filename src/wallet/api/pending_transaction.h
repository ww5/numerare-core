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
#include "wallet/wallet2.h"

#include <string>
#include <vector>


namespace Monero {

class WalletImpl;
class PendingTransactionImpl : public PendingTransaction
{
public:
    PendingTransactionImpl(WalletImpl &wallet);
    ~PendingTransactionImpl();
    int status() const;
    std::string errorString() const;
    bool commit(const std::string &filename = "", bool overwrite = false);
    uint64_t amount() const;
    uint64_t dust() const;
    uint64_t fee() const;
    std::vector<std::string> txid() const;
    uint64_t txCount() const;
    std::vector<uint32_t> subaddrAccount() const;
    std::vector<std::set<uint32_t>> subaddrIndices() const;
    // TODO: continue with interface;

private:
    friend class WalletImpl;
    WalletImpl &m_wallet;

    int  m_status;
    std::string m_errorString;
    std::vector<tools::wallet2::pending_tx> m_pending_tx;
};


}

namespace Bitmonero = Monero;
