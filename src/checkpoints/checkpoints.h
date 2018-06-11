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
#include <map>
#include <vector>
#include "misc_log_ex.h"
#include "crypto/hash.h"
#include "cryptonote_config.h"

#define ADD_CHECKPOINT(h, hash)  CHECK_AND_ASSERT(add_checkpoint(h,  hash), false);
#define JSON_HASH_FILE_NAME "checkpoints.json"


namespace cryptonote
{
  /**
   * @brief A container for blockchain checkpoints
   *
   * A checkpoint is a pre-defined hash for the block at a given height.
   * Some of these are compiled-in, while others can be loaded at runtime
   * either from a json file or via DNS from a checkpoint-hosting server.
   */
  class checkpoints
  {
  public:

    /**
     * @brief default constructor
     */
    checkpoints();

    /**
     * @brief adds a checkpoint to the container
     *
     * @param height the height of the block the checkpoint is for
     * @param hash_str the hash of the block, as a string
     *
     * @return false if parsing the hash fails, or if the height is a duplicate
     *         AND the existing checkpoint hash does not match the new one,
     *         otherwise returns true
     */
    bool add_checkpoint(uint64_t height, const std::string& hash_str);

    /**
     * @brief checks if there is a checkpoint in the future
     *
     * This function checks if the height passed is lower than the highest
     * checkpoint.
     *
     * @param height the height to check against
     *
     * @return false if no checkpoints, otherwise returns whether or not
     *         the height passed is lower than the highest checkpoint.
     */
    bool is_in_checkpoint_zone(uint64_t height) const;

    /**
     * @brief checks if the given height and hash agree with the checkpoints
     *
     * This function checks if the given height and hash exist in the
     * checkpoints container.  If so, it returns whether or not the passed
     * parameters match the stored values.
     *
     * @param height the height to be checked
     * @param h the hash to be checked
     * @param is_a_checkpoint return-by-reference if there is a checkpoint at the given height
     *
     * @return true if there is no checkpoint at the given height,
     *         true if the passed parameters match the stored checkpoint,
     *         false otherwise
     */
    bool check_block(uint64_t height, const crypto::hash& h, bool& is_a_checkpoint) const;

    /**
     * @overload
     */
    bool check_block(uint64_t height, const crypto::hash& h) const;

    /**
     * @brief checks if alternate chain blocks should be kept for a given height
     *
     * this basically says if the blockchain is smaller than the first
     * checkpoint then alternate blocks are allowed.  Alternatively, if the
     * last checkpoint *before* the end of the current chain is also before
     * the block to be added, then this is fine.
     *
     * @param blockchain_height the current blockchain height
     * @param block_height the height of the block to be added as alternate
     *
     * @return true if alternate blocks are allowed given the parameters,
     *         otherwise false
     */
    bool is_alternative_block_allowed(uint64_t blockchain_height, uint64_t block_height) const;

    /**
     * @brief gets the highest checkpoint height
     *
     * @return the height of the highest checkpoint
     */
    uint64_t get_max_height() const;

    /**
     * @brief gets the checkpoints container
     *
     * @return a const reference to the checkpoints container
     */
    const std::map<uint64_t, crypto::hash>& get_points() const;

    /**
     * @brief checks if our checkpoints container conflicts with another
     *
     * A conflict refers to a case where both checkpoint sets have a checkpoint
     * for a specific height but their hashes for that height do not match.
     *
     * @param other the other checkpoints instance to check against
     *
     * @return false if any conflict is found, otherwise true
     */
    bool check_for_conflicts(const checkpoints& other) const;

    /**
     * @brief loads the default main chain checkpoints
     * @param nettype network type
     *
     * @return true unless adding a checkpoint fails
     */
    bool init_default_checkpoints(network_type nettype);

    /**
     * @brief load new checkpoints
     *
     * Loads new checkpoints from the specified json file, as well as
     * (optionally) from DNS.
     *
     * @param json_hashfile_fullpath path to the json checkpoints file
     * @param nettype network type
     * @param dns whether or not to load DNS checkpoints
     *
     * @return true if loading successful and no conflicts
     */
    bool load_new_checkpoints(const std::string &json_hashfile_fullpath, network_type nettype=MAINNET, bool dns=true);

    /**
     * @brief load new checkpoints from json
     *
     * @param json_hashfile_fullpath path to the json checkpoints file
     *
     * @return true if loading successful and no conflicts
     */
    bool load_checkpoints_from_json(const std::string &json_hashfile_fullpath);

    /**
     * @brief load new checkpoints from DNS
     *
     * @param nettype network type
     *
     * @return true if loading successful and no conflicts
     */
    bool load_checkpoints_from_dns(network_type nettype = MAINNET);

  private:
    std::map<uint64_t, crypto::hash> m_points; //!< the checkpoints container
  };

}
