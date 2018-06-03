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

namespace Monero {

class WalletImpl;

class SubaddressAccountImpl : public SubaddressAccount
{
public:
    SubaddressAccountImpl(WalletImpl * wallet);
    ~SubaddressAccountImpl();
    
    // Fetches addresses from Wallet2
    void refresh();
    std::vector<SubaddressAccountRow*> getAll() const;
    void addRow(const std::string &label);
    void setLabel(uint32_t accountIndex, const std::string &label);

private:
    void clearRows();
    
private:
    WalletImpl *m_wallet;
    std::vector<SubaddressAccountRow*> m_rows;
};

}
