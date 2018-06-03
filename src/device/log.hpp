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

#pragma once

#include <cstddef>
#include <string>

#include "ringct/rctOps.h"
#include "crypto/crypto.h"
#include "cryptonote_basic/account.h"

#include "device.hpp"

namespace hw {

    #ifdef WITH_DEVICE_LEDGER    
    namespace ledger {

        void buffer_to_str(char *to_buff,  size_t to_len, const char *buff, size_t len) ;
        void log_hexbuffer(std::string msg,  const char* buff, size_t len);
        void log_message(std::string msg,  std::string info );
        #ifdef DEBUG_HWDEVICE
        #define TRACK printf("file %s:%d\n",__FILE__, __LINE__)
        //#define TRACK MCDEBUG("ledger"," At file " << __FILE__ << ":" << __LINE__)
        //#define TRACK while(0);

        void decrypt(char* buf, size_t len) ;
        crypto::key_derivation decrypt(const crypto::key_derivation &derivation) ;
        cryptonote::account_keys decrypt(const cryptonote::account_keys& keys) ;
        crypto::secret_key decrypt(const crypto::secret_key &sec) ;
        rct::key  decrypt(const rct::key &sec);
        crypto::ec_scalar decrypt(const crypto::ec_scalar &res);
        rct::keyV decrypt(const rct::keyV &keys);

        void check32(std::string msg, std::string info, const char *h, const char *d, bool crypted=false);
        void check8(std::string msg, std::string info, const char *h, const char *d,  bool crypted=false);

        void set_check_verbose(bool verbose);
        #endif
    }
    #endif
}
