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

#pragma once

#include "device.hpp"

namespace hw {

    namespace core {

        void register_all(std::map<std::string, std::unique_ptr<device>> &registry);

        class device_default : public hw::device {
        public:
            device_default();
            ~device_default();

            device_default(const device_default &device) = delete;
            device_default& operator=(const device_default &device) = delete;

            explicit operator bool() const override { return false; };

             /* ======================================================================= */
            /*                              SETUP/TEARDOWN                             */
            /* ======================================================================= */
            bool set_name(const std::string &name) override;
            const std::string get_name() const override;

            bool init(void) override;
            bool release() override;

            bool connect(void) override;
            bool disconnect() override;
 
            bool set_mode(device_mode mode) override;

            /* ======================================================================= */
            /*  LOCKER                                                                 */
            /* ======================================================================= */ 
            void lock(void)  override;
            void unlock(void) override;
            bool try_lock(void) override;
            
            /* ======================================================================= */
            /*                             WALLET & ADDRESS                            */
            /* ======================================================================= */
            bool  get_public_address(cryptonote::account_public_address &pubkey) override;
            bool  get_secret_keys(crypto::secret_key &viewkey , crypto::secret_key &spendkey) override;
            bool  generate_chacha_key(const cryptonote::account_keys &keys, crypto::chacha_key &key) override;
 
            /* ======================================================================= */
            /*                               SUB ADDRESS                               */
            /* ======================================================================= */
            bool  derive_subaddress_public_key(const crypto::public_key &pub, const crypto::key_derivation &derivation, const std::size_t output_index,  crypto::public_key &derived_pub) override;
            crypto::public_key  get_subaddress_spend_public_key(const cryptonote::account_keys& keys, const cryptonote::subaddress_index& index) override;
            std::vector<crypto::public_key>  get_subaddress_spend_public_keys(const cryptonote::account_keys &keys, uint32_t account, uint32_t begin, uint32_t end) override;
            cryptonote::account_public_address  get_subaddress(const cryptonote::account_keys& keys, const cryptonote::subaddress_index &index) override;
            crypto::secret_key  get_subaddress_secret_key(const crypto::secret_key &sec, const cryptonote::subaddress_index &index) override;

            /* ======================================================================= */
            /*                            DERIVATION & KEY                             */
            /* ======================================================================= */
            bool  verify_keys(const crypto::secret_key &secret_key, const crypto::public_key &public_key)  override;
            bool  scalarmultKey(rct::key & aP, const rct::key &P, const rct::key &a) override;
            bool  scalarmultBase(rct::key &aG, const rct::key &a) override;
            bool  sc_secret_add(crypto::secret_key &r, const crypto::secret_key &a, const crypto::secret_key &b) override;
            crypto::secret_key  generate_keys(crypto::public_key &pub, crypto::secret_key &sec, const crypto::secret_key& recovery_key = crypto::secret_key(), bool recover = false) override;
            bool  generate_key_derivation(const crypto::public_key &pub, const crypto::secret_key &sec, crypto::key_derivation &derivation) override;
            bool  conceal_derivation(crypto::key_derivation &derivation, const crypto::public_key &tx_pub_key, const std::vector<crypto::public_key> &additional_tx_pub_keys, const crypto::key_derivation &main_derivation, const std::vector<crypto::key_derivation> &additional_derivations) override;
            bool  derivation_to_scalar(const crypto::key_derivation &derivation, const size_t output_index, crypto::ec_scalar &res) override;
            bool  derive_secret_key(const crypto::key_derivation &derivation, const std::size_t output_index, const crypto::secret_key &sec,  crypto::secret_key &derived_sec) override;
            bool  derive_public_key(const crypto::key_derivation &derivation, const std::size_t output_index, const crypto::public_key &pub,  crypto::public_key &derived_pub) override;
            bool  secret_key_to_public_key(const crypto::secret_key &sec, crypto::public_key &pub) override;
            bool  generate_key_image(const crypto::public_key &pub, const crypto::secret_key &sec, crypto::key_image &image) override;


            /* ======================================================================= */
            /*                               TRANSACTION                               */
            /* ======================================================================= */

            bool  open_tx(crypto::secret_key &tx_key) override;

            bool  encrypt_payment_id(crypto::hash8 &payment_id, const crypto::public_key &public_key, const crypto::secret_key &secret_key) override;

            bool  ecdhEncode(rct::ecdhTuple & unmasked, const rct::key & sharedSec) override;
            bool  ecdhDecode(rct::ecdhTuple & masked, const rct::key & sharedSec) override;

            bool  add_output_key_mapping(const crypto::public_key &Aout, const crypto::public_key &Bout, const bool is_subaddress, const size_t real_output_index,
                                         const rct::key &amount_key,  const crypto::public_key &out_eph_public_key) override;


            bool  mlsag_prehash(const std::string &blob, size_t inputs_size, size_t outputs_size, const rct::keyV &hashes, const rct::ctkeyV &outPk, rct::key &prehash) override;
            bool  mlsag_prepare(const rct::key &H, const rct::key &xx, rct::key &a, rct::key &aG, rct::key &aHP, rct::key &rvII) override;
            bool  mlsag_prepare(rct::key &a, rct::key &aG) override;
            bool  mlsag_hash(const rct::keyV &long_message, rct::key &c) override;
            bool  mlsag_sign(const rct::key &c, const rct::keyV &xx, const rct::keyV &alpha, const size_t rows, const size_t dsRows, rct::keyV &ss) override;

            bool  close_tx(void) override;
        };

    }



}
