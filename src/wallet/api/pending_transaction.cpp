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

#include "pending_transaction.h"
#include "wallet.h"
#include "common_defines.h"

#include "cryptonote_basic/cryptonote_format_utils.h"
#include "cryptonote_basic/cryptonote_basic_impl.h"

#include <memory>
#include <vector>
#include <sstream>
#include <boost/format.hpp>

using namespace std;

namespace Monero {

PendingTransaction::~PendingTransaction() {}


PendingTransactionImpl::PendingTransactionImpl(WalletImpl &wallet)
    : m_wallet(wallet)
{
  m_status = Status_Ok;
}

PendingTransactionImpl::~PendingTransactionImpl()
{

}

int PendingTransactionImpl::status() const
{
    return m_status;
}

string PendingTransactionImpl::errorString() const
{
    return m_errorString;
}

std::vector<std::string> PendingTransactionImpl::txid() const
{
    std::vector<std::string> txid;
    for (const auto &pt: m_pending_tx)
        txid.push_back(epee::string_tools::pod_to_hex(cryptonote::get_transaction_hash(pt.tx)));
    return txid;
}

bool PendingTransactionImpl::commit(const std::string &filename, bool overwrite)
{

    LOG_PRINT_L3("m_pending_tx size: " << m_pending_tx.size());

    try {
      // Save tx to file
      if (!filename.empty()) {
        boost::system::error_code ignore;
        bool tx_file_exists = boost::filesystem::exists(filename, ignore);
        if(tx_file_exists && !overwrite){
          m_errorString = string(tr("Attempting to save transaction to file, but specified file(s) exist. Exiting to not risk overwriting. File:")) + filename;
          m_status = Status_Error;
          LOG_ERROR(m_errorString);
          return false;
        }
        bool r = m_wallet.m_wallet->save_tx(m_pending_tx, filename);
        if (!r) {
          m_errorString = tr("Failed to write transaction(s) to file");
          m_status = Status_Error;
        } else {
          m_status = Status_Ok;
        }
      }
      // Commit tx
      else {
        m_wallet.pauseRefresh();
        while (!m_pending_tx.empty()) {
            auto & ptx = m_pending_tx.back();
            m_wallet.m_wallet->commit_tx(ptx);
            // if no exception, remove element from vector
            m_pending_tx.pop_back();
        } // TODO: extract method;
      }
    } catch (const tools::error::daemon_busy&) {
        // TODO: make it translatable with "tr"?
        m_errorString = tr("daemon is busy. Please try again later.");
        m_status = Status_Error;
    } catch (const tools::error::no_connection_to_daemon&) {
        m_errorString = tr("no connection to daemon. Please make sure daemon is running.");
        m_status = Status_Error;
    } catch (const tools::error::tx_rejected& e) {
        std::ostringstream writer(m_errorString);
        writer << (boost::format(tr("transaction %s was rejected by daemon with status: ")) % get_transaction_hash(e.tx())) <<  e.status();
        std::string reason = e.reason();
        m_status = Status_Error;
        m_errorString = writer.str();
        if (!reason.empty())
          m_errorString  += string(tr(". Reason: ")) + reason;
    } catch (const std::exception &e) {
        m_errorString = string(tr("Unknown exception: ")) + e.what();
        m_status = Status_Error;
    } catch (...) {
        m_errorString = tr("Unhandled exception");
        LOG_ERROR(m_errorString);
        m_status = Status_Error;
    }

    m_wallet.startRefresh();
    return m_status == Status_Ok;
}

uint64_t PendingTransactionImpl::amount() const
{
    uint64_t result = 0;
    for (const auto &ptx : m_pending_tx)   {
        for (const auto &dest : ptx.dests) {
            result += dest.amount;
        }
    }
    return result;
}

uint64_t PendingTransactionImpl::dust() const
{
    uint64_t result = 0;
    for (const auto & ptx : m_pending_tx) {
        result += ptx.dust;
    }
    return result;
}

uint64_t PendingTransactionImpl::fee() const
{
    uint64_t result = 0;
    for (const auto &ptx : m_pending_tx) {
        result += ptx.fee;
    }
    return result;
}

uint64_t PendingTransactionImpl::txCount() const
{
    return m_pending_tx.size();
}

std::vector<uint32_t> PendingTransactionImpl::subaddrAccount() const
{
    std::vector<uint32_t> result;
    for (const auto& ptx : m_pending_tx)
        result.push_back(ptx.construction_data.subaddr_account);
    return result;
}

std::vector<std::set<uint32_t>> PendingTransactionImpl::subaddrIndices() const
{
    std::vector<std::set<uint32_t>> result;
    for (const auto& ptx : m_pending_tx)
        result.push_back(ptx.construction_data.subaddr_indices);
    return result;
}

}

namespace Bitmonero = Monero;
