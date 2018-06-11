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


/* Note about debug:
 * To debug Device you can def the following :
 * #define DEBUG_HWDEVICE
 *   Activate debug mechanism:
 *     - Add more trace
 *     - All computation done by device are checked by default device.
 *       Required IODUMMYCRYPT_HWDEVICE or IONOCRYPT_HWDEVICE for fully working
 * #define IODUMMYCRYPT_HWDEVICE 1
 *     - It assumes sensitive data encryption is is off on device side. a XOR with 0x55. This allow Ledger Class to make check on clear value
 * #define IONOCRYPT_HWDEVICE 1
 *     - It assumes sensitive data encryption is off on device side.
 */


#pragma once

#include "crypto/crypto.h"
#include "crypto/chacha.h"
#include "ringct/rctTypes.h"

#ifndef USE_DEVICE_LEDGER
#define USE_DEVICE_LEDGER 1
#endif

#if !defined(HAVE_PCSC) 
#undef  USE_DEVICE_LEDGER
#define USE_DEVICE_LEDGER 0
#endif

#if USE_DEVICE_LEDGER
#define WITH_DEVICE_LEDGER
#endif

// forward declaration needed because this header is included by headers in libcryptonote_basic which depends on libdevice
namespace cryptonote
{
    struct account_public_address;
    struct account_keys;
    struct subaddress_index;
}

namespace hw {
    namespace {
        //device funcion not supported
        #define dfns()  \
           throw std::runtime_error(std::string("device function not supported: ")+ std::string(__FUNCTION__) + \
                                    std::string(" (device.hpp line ")+std::to_string(__LINE__)+std::string(").")); \
           return false;
    }


    class device {
    protected:
        std::string  name;

    public:

        device()  {}
        device(const device &hwdev) {}
        virtual ~device()   {}

        explicit virtual operator bool() const = 0;
        enum device_mode {
            NONE,
            TRANSACTION_CREATE_REAL,
            TRANSACTION_CREATE_FAKE,
            TRANSACTION_PARSE
        };

        /* ======================================================================= */
        /*                              SETUP/TEARDOWN                             */
        /* ======================================================================= */
        virtual bool set_name(const std::string &name) = 0;
        virtual const std::string get_name() const = 0;

        virtual  bool init(void) = 0;
        virtual bool release() = 0;

        virtual bool connect(void) = 0;
        virtual bool disconnect(void) = 0;

        virtual bool  set_mode(device_mode mode) = 0;


        /* ======================================================================= */
        /*  LOCKER                                                                 */
        /* ======================================================================= */ 
        virtual void lock(void) = 0;
        virtual void unlock(void) = 0;
        virtual bool try_lock(void) = 0;


        /* ======================================================================= */
        /*                             WALLET & ADDRESS                            */
        /* ======================================================================= */
        virtual bool  get_public_address(cryptonote::account_public_address &pubkey) = 0;
        virtual bool  get_secret_keys(crypto::secret_key &viewkey , crypto::secret_key &spendkey)  = 0;
        virtual bool  generate_chacha_key(const cryptonote::account_keys &keys, crypto::chacha_key &key) = 0;

        /* ======================================================================= */
        /*                               SUB ADDRESS                               */
        /* ======================================================================= */
        virtual bool  derive_subaddress_public_key(const crypto::public_key &pub, const crypto::key_derivation &derivation, const std::size_t output_index,  crypto::public_key &derived_pub) = 0;
        virtual crypto::public_key  get_subaddress_spend_public_key(const cryptonote::account_keys& keys, const cryptonote::subaddress_index& index) = 0;
        virtual std::vector<crypto::public_key>  get_subaddress_spend_public_keys(const cryptonote::account_keys &keys, uint32_t account, uint32_t begin, uint32_t end) = 0;
        virtual cryptonote::account_public_address  get_subaddress(const cryptonote::account_keys& keys, const cryptonote::subaddress_index &index) = 0;
        virtual crypto::secret_key  get_subaddress_secret_key(const crypto::secret_key &sec, const cryptonote::subaddress_index &index) = 0;

        /* ======================================================================= */
        /*                            DERIVATION & KEY                             */
        /* ======================================================================= */
        virtual bool  verify_keys(const crypto::secret_key &secret_key, const crypto::public_key &public_key) = 0;
        virtual bool  scalarmultKey(rct::key & aP, const rct::key &P, const rct::key &a) = 0;
        virtual bool  scalarmultBase(rct::key &aG, const rct::key &a) = 0;
        virtual bool  sc_secret_add( crypto::secret_key &r, const crypto::secret_key &a, const crypto::secret_key &b) = 0;
        virtual crypto::secret_key  generate_keys(crypto::public_key &pub, crypto::secret_key &sec, const crypto::secret_key& recovery_key = crypto::secret_key(), bool recover = false) = 0;
        virtual bool  generate_key_derivation(const crypto::public_key &pub, const crypto::secret_key &sec, crypto::key_derivation &derivation) = 0;
        virtual bool  conceal_derivation(crypto::key_derivation &derivation, const crypto::public_key &tx_pub_key, const std::vector<crypto::public_key> &additional_tx_pub_keys, const crypto::key_derivation &main_derivation, const std::vector<crypto::key_derivation> &additional_derivations) = 0;
        virtual bool  derivation_to_scalar(const crypto::key_derivation &derivation, const size_t output_index, crypto::ec_scalar &res) = 0;
        virtual bool  derive_secret_key(const crypto::key_derivation &derivation, const std::size_t output_index, const crypto::secret_key &sec,  crypto::secret_key &derived_sec) = 0;
        virtual bool  derive_public_key(const crypto::key_derivation &derivation, const std::size_t output_index, const crypto::public_key &pub,  crypto::public_key &derived_pub) = 0;
        virtual bool  secret_key_to_public_key(const crypto::secret_key &sec, crypto::public_key &pub) = 0;
        virtual bool  generate_key_image(const crypto::public_key &pub, const crypto::secret_key &sec, crypto::key_image &image) = 0;

        // alternative prototypes available in libringct
        rct::key scalarmultKey(const rct::key &P, const rct::key &a)
        {
            rct::key aP;
            scalarmultKey(aP, P, a);
            return aP;
        }

        rct::key scalarmultBase(const rct::key &a)
        {
            rct::key aG;
            scalarmultBase(aG, a);
            return aG;
        }

        /* ======================================================================= */
        /*                               TRANSACTION                               */
        /* ======================================================================= */

        virtual bool  open_tx(crypto::secret_key &tx_key) = 0;

        virtual bool  encrypt_payment_id(crypto::hash8 &payment_id, const crypto::public_key &public_key, const crypto::secret_key &secret_key) = 0;
        bool  decrypt_payment_id(crypto::hash8 &payment_id, const crypto::public_key &public_key, const crypto::secret_key &secret_key)
        {
            // Encryption and decryption are the same operation (xor with a key)
            return encrypt_payment_id(payment_id, public_key, secret_key);
        }

        virtual bool  ecdhEncode(rct::ecdhTuple & unmasked, const rct::key & sharedSec) = 0;
        virtual bool  ecdhDecode(rct::ecdhTuple & masked, const rct::key & sharedSec) = 0;

        virtual bool  add_output_key_mapping(const crypto::public_key &Aout, const crypto::public_key &Bout, const bool is_subaddress, const size_t real_output_index,
                                             const rct::key &amount_key,  const crypto::public_key &out_eph_public_key) = 0;


        virtual bool  mlsag_prehash(const std::string &blob, size_t inputs_size, size_t outputs_size, const rct::keyV &hashes, const rct::ctkeyV &outPk, rct::key &prehash) = 0;
        virtual bool  mlsag_prepare(const rct::key &H, const rct::key &xx, rct::key &a, rct::key &aG, rct::key &aHP, rct::key &rvII) = 0;
        virtual bool  mlsag_prepare(rct::key &a, rct::key &aG) = 0;
        virtual bool  mlsag_hash(const rct::keyV &long_message, rct::key &c) = 0;
        virtual bool  mlsag_sign(const rct::key &c, const rct::keyV &xx, const rct::keyV &alpha, const size_t rows, const size_t dsRows, rct::keyV &ss) = 0;

        virtual bool  close_tx(void) = 0;
    } ;

    struct reset_mode {
        device& hwref;
        reset_mode(hw::device& dev) : hwref(dev) { }
        ~reset_mode() { hwref.set_mode(hw::device::NONE);}
    };

    device& get_device(const std::string device_descriptor) ;
}

