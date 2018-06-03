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
//

#include "misc_log_ex.h"
#include "log.hpp"

namespace hw {

    #ifdef WITH_DEVICE_LEDGER    
    namespace ledger {
    
    #undef MONERO_DEFAULT_LOG_CATEGORY
    #define MONERO_DEFAULT_LOG_CATEGORY "device.ledger"

    void buffer_to_str(char *to_buff,  size_t to_len, const char *buff, size_t len) {
      CHECK_AND_ASSERT_THROW_MES(to_len > (len*2), "destination buffer too short. At least" << (len*2+1) << " bytes required");
      for (size_t i=0; i<len; i++) {
        sprintf(to_buff+2*i, "%.02x", (unsigned char)buff[i]);
      }
    }

    void log_hexbuffer(std::string msg,  const char* buff, size_t len) {
      char logstr[1025];
      buffer_to_str(logstr, sizeof(logstr),  buff, len);
      MDEBUG(msg<< ": " << logstr);
    }

    void log_message(std::string msg,  std::string info ) {
      MDEBUG(msg << ": " << info);
    }

    #ifdef DEBUG_HWDEVICE
    extern crypto::secret_key dbg_viewkey;
    extern crypto::secret_key dbg_spendkey;


    void decrypt(char* buf, size_t len) {
      #ifdef IODUMMYCRYPT_HWDEVICE
      size_t i;
      if (len == 32) {
        //view key?
        for (i = 0; i<32; i++) {
          if (buf[i] != 0) break;
        }
        if (i == 32) {
          memmove(buf, hw::ledger::dbg_viewkey.data, 32);
          return;
        }
        //spend key?
        for (i = 0; i<32; i++) {
          if (buf[i] != (char)0xff) break;
        }
        if (i == 32) {
          memmove(buf, hw::ledger::dbg_spendkey.data, 32);
          return;
        }
      }
      //std decrypt: XOR.55h
      for (i = 0; i<len;i++) {
          buf[i] ^= 0x55;
        }
      #endif
    }

    crypto::key_derivation decrypt(const crypto::key_derivation &derivation) {
       crypto::key_derivation x = derivation;
       decrypt(x.data, 32);
       return x;
    }

    cryptonote::account_keys decrypt(const cryptonote::account_keys& keys) {
       cryptonote::account_keys x = keys;
       decrypt(x.m_view_secret_key.data, 32);
       decrypt(x.m_spend_secret_key.data, 32);
       return x;
    }


    crypto::secret_key decrypt(const crypto::secret_key &sec) {
       crypto::secret_key  x = sec;
       decrypt(x.data, 32);
       return x;
    }

    rct::key  decrypt(const rct::key &sec)  {
         rct::key  x = sec;
       decrypt((char*)x.bytes, 32);
       return x;
    }

    crypto::ec_scalar decrypt(const crypto::ec_scalar &res)  {
       crypto::ec_scalar  x = res;
       decrypt((char*)x.data, 32);
       return x;
    }

    rct::keyV decrypt(const rct::keyV &keys) {
        rct::keyV x ;
        for (unsigned int j = 0; j<keys.size(); j++) {
            x.push_back(decrypt(keys[j]));
        }
        return x;
    }

    static void check(std::string msg, std::string info, const char *h, const char *d, int len, bool crypted) {
      char dd[32];
      char logstr[128];

      memmove(dd,d,len);
      if (crypted) {
        CHECK_AND_ASSERT_THROW_MES(len<=32, "encrypted data greater than 32");
        decrypt(dd,len);
      }

      if (memcmp(h,dd,len)) {
          log_message("ASSERT EQ FAIL",  msg + ": "+ info );
          log_hexbuffer("    host  ", h, len);
          log_hexbuffer("    device", dd, len);

      } else {
        buffer_to_str(logstr, 128,  dd, len);
        log_message("ASSERT EQ OK",  msg + ": "+ info + ": "+ std::string(logstr) );
      }
    }

    void check32(std::string msg, std::string info, const char *h, const char *d, bool crypted) {
      check(msg, info, h, d, 32, crypted);
    }

    void check8(std::string msg, std::string info, const char *h, const char *d, bool crypted) {
      check(msg, info, h, d, 8, crypted);
    }
    #endif

  }
  #endif //WITH_DEVICE_LEDGER    

}
