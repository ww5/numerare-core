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
class UnsignedTransactionImpl : public UnsignedTransaction
{
public:
    UnsignedTransactionImpl(WalletImpl &wallet);
    ~UnsignedTransactionImpl();
    int status() const;
    std::string errorString() const;
    std::vector<uint64_t> amount() const;
    std::vector<uint64_t> dust() const;
    std::vector<uint64_t> fee() const;
    std::vector<uint64_t> mixin() const;
    std::vector<std::string> paymentId() const;
    std::vector<std::string> recipientAddress() const;
    uint64_t txCount() const;
    // sign txs and save to file
    bool sign(const std::string &signedFileName);
    std::string confirmationMessage() const {return m_confirmationMessage;}
    uint64_t minMixinCount() const;

private:
    // Callback function to check all loaded tx's and generate confirmationMessage
    bool checkLoadedTx(const std::function<size_t()> get_num_txes, const std::function<const tools::wallet2::tx_construction_data&(size_t)> &get_tx, const std::string &extra_message);
    
    friend class WalletImpl;
    WalletImpl &m_wallet;

    int  m_status;
    std::string m_errorString;
    tools::wallet2::unsigned_tx_set m_unsigned_tx_set;
    std::string m_confirmationMessage;
};


}

namespace Bitmonero = Monero;
