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

class AddressBookImpl : public AddressBook
{
public:
    AddressBookImpl(WalletImpl * wallet);
    ~AddressBookImpl();
    
    // Fetches addresses from Wallet2
    void refresh();
    std::vector<AddressBookRow*> getAll() const;
    bool addRow(const std::string &dst_addr , const std::string &payment_id, const std::string &description);
    bool deleteRow(std::size_t rowId);
     
    // Error codes. See AddressBook:ErrorCode enum in wallet2_api.h
    std::string errorString() const {return m_errorString;}
    int errorCode() const {return m_errorCode;}

    int lookupPaymentID(const std::string &payment_id) const;
    
private:
    void clearRows();
    void clearStatus();
    
private:
    WalletImpl *m_wallet;
    std::vector<AddressBookRow*> m_rows;
    std::string m_errorString;
    int m_errorCode;
};

}

namespace Bitmonero = Monero;

