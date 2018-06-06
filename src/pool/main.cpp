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
    double dev = 0.003;
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
              << "miners: " << miners_reward << std::endl;
}