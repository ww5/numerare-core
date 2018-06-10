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
#include <iostream>
#include <vector>
#include <thread>

#include "server.hpp"
#include "client.hpp"
#include "rpc_client.hpp"

#include "rpc/core_rpc_server_commands_defs.h"

using namespace cryptonote;

WebsocketServer wsServer;
WebsocketClient wsClient;

void start();
void startClient();

int main(int argc, char* argv[]) {
    // Check command line arguments.
    if (argc != 2)
    {
        std::cout << "missing number of miners";
            //"Usage: pool <port> <NUMERARE wallet address>\n" <<
            //"Example:\n" <<
            //"    pool 9999 WALLET-ADDRESS\n";
        return 0;
    }

    int reward = 1369863;
    int miners = std::atoi(argv[1]);

    double fee = 0.01;
    double dev = 0.0033;
    double finder = 0.1;

    int pool_fee = reward * fee;    
    int r_p1 = reward - pool_fee;

    int dev_fee = r_p1 * dev;
    int r_p2 = r_p1 - dev_fee;

    int finder_bonus = r_p2 * finder;
    int r_p3 = r_p2 - finder_bonus;

    int miners_reward = r_p3 / miners;
    int reward_dust = r_p3 - (miners_reward * miners);

    dev_fee += reward_dust;

    std::cout << "pool: " << pool_fee << std::endl
              << "dev: " << dev_fee << std::endl
              << "dust: " << reward_dust << std::endl
              << "bonus: " << finder_bonus << std::endl
              << "miners: " << miners_reward << std::endl << std::endl;

    std::cout << "total miners reward: " << miners_reward * miners << std::endl;
    std::cout << "total fee: " << pool_fee + dev_fee << std::endl << std::endl;

    std::cout << "it's " << reward << "? " << miners_reward * miners + finder_bonus + pool_fee + dev_fee << std::endl;

    //connect to node
    rpcClient n;
    bool r = n.init("http://localhost:31931");
    if(r) {
        bool t;
        std::thread c;

        wsClient.init(); 

        while(true) {
            auto cmd = std::cin.get();
            if(cmd == 'h') {
                std::cout << "get height..." << std::endl;

                COMMAND_RPC_GET_HEIGHT::request req;
                COMMAND_RPC_GET_HEIGHT::response res = boost::value_initialized<COMMAND_RPC_GET_HEIGHT::response>();
                if(n.invoke_http_json("/getheight", req, res)) {
                    std::cout << res.height << std::endl;
                }
            } else if(cmd == 'c') {
                std::cout << "start client..." << std::endl;
                //if(!c.joinable())
                    //c = std::thread(startClient);
                wsClient.run();
            } else if(cmd == 'd') {
                std::cout << "start ws server..." << std::endl;
                if(!t) {                
                    wsServer.init(); 
                    wsServer.run();
                }
            } else if(cmd == 's') {
                std::cout << "stop client..." << std::endl;
                wsClient.stop();
                //c.join();                
            } else if(cmd == 'q') {
                break;
            }
        }

        std::cout << "stop rpc..." << std::endl;
        n.deinit();

        std::cout << "stop ws client..." << std::endl;    
        wsClient.uninit();
        
        //if(t.joinable()) {
        std::cout << "stop ws server..." << std::endl;    
        wsServer.stop();
        //    t.join();
        //}

    } else {
        std::cout << "error RPC: node doesn't respond!" << std::endl;    
    }
    
    std::cout << "exit now!" << std::endl;

    return 0;
}

void start() {   
    
}

void startClient() {        
   
}