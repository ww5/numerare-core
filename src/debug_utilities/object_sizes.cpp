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

#include <map>
#include "cryptonote_basic/cryptonote_basic.h"
#include "cryptonote_basic/tx_extra.h"
#include "cryptonote_core/blockchain.h"
#include "p2p/p2p_protocol_defs.h"
#include "net/connection_basic.hpp"
#include "p2p/net_peerlist.h"
#include "p2p/net_node.h"
#include "cryptonote_protocol/cryptonote_protocol_handler.h"
#include "blockchain_db/lmdb/db_lmdb.h"
#include "wallet/wallet2.h"
#include "wallet/api/wallet.h"
#include "wallet/api/transaction_info.h"
#include "wallet/api/transaction_history.h"
#include "wallet/api/unsigned_transaction.h"
#include "wallet/api/pending_transaction.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "debugtools.objectsizes"

class size_logger
{
public:
  ~size_logger()
  {
    for (const auto &i: types)
      std::cout << std::to_string(i.first) << "\t" << i.second << std::endl;
  }
  void add(const char *type, size_t size) { types.insert(std::make_pair(size, type)); }
private:
  std::multimap<size_t, const std::string> types;
};
#define SL(type) sl.add(#type, sizeof(type))

int main(int argc, char* argv[])
{
  size_logger sl;

  tools::on_startup();

  mlog_configure("", true);

  SL(boost::thread);
  SL(boost::asio::io_service);
  SL(boost::asio::io_service::work);
  SL(boost::asio::deadline_timer);

  SL(cryptonote::DB_ERROR);
  SL(cryptonote::mdb_txn_safe);
  SL(cryptonote::mdb_threadinfo);

  SL(cryptonote::block_header);
  SL(cryptonote::block);
  SL(cryptonote::transaction_prefix);
  SL(cryptonote::transaction);

  SL(cryptonote::txpool_tx_meta_t);

  SL(epee::net_utils::ipv4_network_address);
  SL(epee::net_utils::network_address);
  SL(epee::net_utils::connection_context_base);
  SL(epee::net_utils::connection_basic);

  SL(nodetool::peerlist_entry);
  SL(nodetool::anchor_peerlist_entry);
  SL(nodetool::node_server<cryptonote::t_cryptonote_protocol_handler<cryptonote::core>>);
  SL(nodetool::p2p_connection_context_t<cryptonote::t_cryptonote_protocol_handler<cryptonote::core>::connection_context>);
  SL(nodetool::network_address_old);

  SL(tools::wallet2::transfer_details);
  SL(tools::wallet2::payment_details);
  SL(tools::wallet2::unconfirmed_transfer_details);
  SL(tools::wallet2::confirmed_transfer_details);
  SL(tools::wallet2::tx_construction_data);
  SL(tools::wallet2::pending_tx);
  SL(tools::wallet2::unsigned_tx_set);
  SL(tools::wallet2::signed_tx_set);

  SL(Monero::WalletImpl);
  SL(Monero::AddressBookRow);
  SL(Monero::TransactionInfoImpl);
  SL(Monero::TransactionHistoryImpl);
  SL(Monero::PendingTransactionImpl);
  SL(Monero::UnsignedTransactionImpl);

  return 0;
}
