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

	Parts of this file are originally Copyright (c) 2012-2017 The CryptoNote developers, The Bytecoin developers
***/

#include "ConfigurationManager.h"

#include <fstream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include "Common/CommandLine.h"
#include "Common/Util.h"
#include "version.h"

namespace PaymentService {

namespace po = boost::program_options;

ConfigurationManager::ConfigurationManager() {
  startInprocess = false;
}

bool ConfigurationManager::init(int argc, char** argv) {
  po::options_description cmdGeneralOptions("Common Options");

  cmdGeneralOptions.add_options()
      ("config,c", po::value<std::string>(), "configuration file");

  po::options_description confGeneralOptions;
  confGeneralOptions.add(cmdGeneralOptions).add_options()
      ("testnet", po::bool_switch(), "")
      ("local", po::bool_switch(), "");

  cmdGeneralOptions.add_options()
      ("help,h", "produce this help message and exit")
      ("local", po::bool_switch(), "start with local node (remote is default)")
      ("testnet", po::bool_switch(), "testnet mode")
      ("version", "Output version information");

  command_line::add_arg(cmdGeneralOptions, command_line::arg_data_dir, Tools::getDefaultDataDirectory());
  command_line::add_arg(confGeneralOptions, command_line::arg_data_dir, Tools::getDefaultDataDirectory());

  Configuration::initOptions(cmdGeneralOptions);
  Configuration::initOptions(confGeneralOptions);

  po::options_description netNodeOptions("Local Node Options");
  CryptoNote::NetNodeConfig::initOptions(netNodeOptions);
  
  po::options_description remoteNodeOptions("Remote Node Options");
  RpcNodeConfiguration::initOptions(remoteNodeOptions);
  po::options_description coinBaseOptions("Coin Base Options");
  CoinBaseConfiguration::initOptions(coinBaseOptions);

  po::options_description cmdOptionsDesc;
  cmdOptionsDesc.add(cmdGeneralOptions).add(remoteNodeOptions).add(netNodeOptions);

  po::options_description confOptionsDesc;
  confOptionsDesc.add(confGeneralOptions).add(remoteNodeOptions).add(netNodeOptions).add(coinBaseOptions);

  po::variables_map cmdOptions;
  po::store(po::parse_command_line(argc, argv, cmdOptionsDesc), cmdOptions);
  po::notify(cmdOptions);

  if (cmdOptions.count("help")) {
    std::cout << cmdOptionsDesc << std::endl;
    return false;
  }

  if (cmdOptions.count("version") > 0) {
    std::cout << "walletd v" << PROJECT_VERSION_LONG;
    return false;
  }

po::variables_map confOptions;
  if (cmdOptions.count("config")) {
    std::ifstream confStream(cmdOptions["config"].as<std::string>(), std::ifstream::in);
    if (!confStream.good()) {
      throw ConfigurationError("Cannot open configuration file");
    }


    po::store(po::parse_config_file(confStream, confOptionsDesc, true), confOptions);
    po::notify(confOptions);

    std::string default_data_dir = Tools::getDefaultDataDirectory();
    if (!coinBaseConfig.CRYPTONOTE_NAME.empty()) {
      boost::replace_all(default_data_dir, CryptoNote::CRYPTONOTE_NAME, coinBaseConfig.CRYPTONOTE_NAME);
    }
    netNodeConfig.setConfigFolder(default_data_dir);
    gateConfiguration.init(confOptions);
    netNodeConfig.init(confOptions);
    remoteNodeConfig.init(confOptions);
    coinBaseConfig.init(confOptions);

    netNodeConfig.setTestnet(confOptions["testnet"].as<bool>());
    startInprocess = confOptions["local"].as<bool>();
  }

  //command line options should override options from config file
  gateConfiguration.init(cmdOptions);
  netNodeConfig.init(cmdOptions);
  remoteNodeConfig.init(cmdOptions);
  dataDir = command_line::get_arg(cmdOptions, command_line::arg_data_dir);

  if (!(cmdOptions["BYTECOIN_NETWORK"].as<std::string>().compare("11100111-1100-0101-1011-001210110110"))) {
    netNodeConfig.setNetworkId(boost::lexical_cast<boost::uuids::uuid>(confOptions["BYTECOIN_NETWORK"].as<std::string>()));
  }

  if (!(cmdOptions["P2P_STAT_TRUSTED_PUB_KEY"].as<std::string>().compare(""))) {
    netNodeConfig.setP2pStatTrustedPubKey(confOptions["P2P_STAT_TRUSTED_PUB_KEY"].as<std::string>());
  }

  if (cmdOptions["testnet"].as<bool>()) {
    netNodeConfig.setTestnet(true);
  }

  if (cmdOptions["local"].as<bool>()) {
    startInprocess = true;
  }

  return true;
}

} //namespace PaymentService
