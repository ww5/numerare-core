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
#include "net/http_client.h"
#include <string>

namespace Monero {

class WalletManagerImpl : public WalletManager
{
public:
    Wallet * createWallet(const std::string &path, const std::string &password,
                          const std::string &language, NetworkType nettype);
    Wallet * openWallet(const std::string &path, const std::string &password, NetworkType nettype);
    virtual Wallet * recoveryWallet(const std::string &path,
                                       const std::string &password,
                                       const std::string &mnemonic,
                                       NetworkType nettype,
                                       uint64_t restoreHeight);
    virtual Wallet * createWalletFromKeys(const std::string &path,
                                             const std::string &password,
                                             const std::string &language,
                                             NetworkType nettype,
                                             uint64_t restoreHeight,
                                             const std::string &addressString,
                                             const std::string &viewKeyString,
                                             const std::string &spendKeyString = "");
    // next two methods are deprecated - use the above version which allow setting of a password
    virtual Wallet * recoveryWallet(const std::string &path, const std::string &mnemonic, NetworkType nettype, uint64_t restoreHeight);
    // deprecated: use createWalletFromKeys(..., password, ...) instead
    virtual Wallet * createWalletFromKeys(const std::string &path, 
                                                    const std::string &language,
                                                    NetworkType nettype, 
                                                    uint64_t restoreHeight,
                                                    const std::string &addressString,
                                                    const std::string &viewKeyString,
                                                    const std::string &spendKeyString = "");
    virtual bool closeWallet(Wallet *wallet, bool store = true);
    bool walletExists(const std::string &path);
    bool verifyWalletPassword(const std::string &keys_file_name, const std::string &password, bool no_spend_key) const;
    std::vector<std::string> findWallets(const std::string &path);
    std::string errorString() const;
    void setDaemonAddress(const std::string &address);
    bool connected(uint32_t *version = NULL);
    uint64_t blockchainHeight();
    uint64_t blockchainTargetHeight();
    uint64_t networkDifficulty();
    double miningHashRate();
    uint64_t blockTarget();
    bool isMining();
    bool startMining(const std::string &address, uint32_t threads = 1, bool background_mining = false, bool ignore_battery = true);
    bool stopMining();
    std::string resolveOpenAlias(const std::string &address, bool &dnssec_valid) const;

private:
    WalletManagerImpl() {}
    friend struct WalletManagerFactory;
    std::string m_daemonAddress;
    epee::net_utils::http::http_simple_client m_http_client;
    std::string m_errorString;
};

} // namespace

namespace Bitmonero = Monero;
