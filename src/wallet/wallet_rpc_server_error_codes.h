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

#pragma  once


#define WALLET_RPC_ERROR_CODE_UNKNOWN_ERROR           -1
#define WALLET_RPC_ERROR_CODE_WRONG_ADDRESS           -2
#define WALLET_RPC_ERROR_CODE_DAEMON_IS_BUSY          -3
#define WALLET_RPC_ERROR_CODE_GENERIC_TRANSFER_ERROR  -4
#define WALLET_RPC_ERROR_CODE_WRONG_PAYMENT_ID        -5
#define WALLET_RPC_ERROR_CODE_TRANSFER_TYPE           -6
#define WALLET_RPC_ERROR_CODE_DENIED                  -7
#define WALLET_RPC_ERROR_CODE_WRONG_TXID              -8
#define WALLET_RPC_ERROR_CODE_WRONG_SIGNATURE         -9
#define WALLET_RPC_ERROR_CODE_WRONG_KEY_IMAGE        -10
#define WALLET_RPC_ERROR_CODE_WRONG_URI              -11
#define WALLET_RPC_ERROR_CODE_WRONG_INDEX            -12
#define WALLET_RPC_ERROR_CODE_NOT_OPEN               -13
#define WALLET_RPC_ERROR_CODE_ACCOUNT_INDEX_OUT_OF_BOUNDS -14
#define WALLET_RPC_ERROR_CODE_ADDRESS_INDEX_OUT_OF_BOUNDS -15
#define WALLET_RPC_ERROR_CODE_TX_NOT_POSSIBLE        -16
#define WALLET_RPC_ERROR_CODE_NOT_ENOUGH_MONEY       -17
#define WALLET_RPC_ERROR_CODE_TX_TOO_LARGE           -18
#define WALLET_RPC_ERROR_CODE_NOT_ENOUGH_OUTS_TO_MIX -19
#define WALLET_RPC_ERROR_CODE_ZERO_DESTINATION       -20
#define WALLET_RPC_ERROR_CODE_WALLET_ALREADY_EXISTS  -21
#define WALLET_RPC_ERROR_CODE_INVALID_PASSWORD       -22
#define WALLET_RPC_ERROR_CODE_NO_WALLET_DIR          -23
#define WALLET_RPC_ERROR_CODE_NO_TXKEY               -24
#define WALLET_RPC_ERROR_CODE_WRONG_KEY              -25
#define WALLET_RPC_ERROR_CODE_BAD_HEX                -26
#define WALLET_RPC_ERROR_CODE_BAD_TX_METADATA        -27
#define WALLET_RPC_ERROR_CODE_ALREADY_MULTISIG       -28
#define WALLET_RPC_ERROR_CODE_WATCH_ONLY             -29
#define WALLET_RPC_ERROR_CODE_BAD_MULTISIG_INFO      -30
#define WALLET_RPC_ERROR_CODE_NOT_MULTISIG           -31
#define WALLET_RPC_ERROR_CODE_WRONG_LR               -32
#define WALLET_RPC_ERROR_CODE_THRESHOLD_NOT_REACHED  -33
#define WALLET_RPC_ERROR_CODE_BAD_MULTISIG_TX_DATA   -34
#define WALLET_RPC_ERROR_CODE_MULTISIG_SIGNATURE     -35
#define WALLET_RPC_ERROR_CODE_MULTISIG_SUBMISSION    -36
#define WALLET_RPC_ERROR_CODE_NOT_ENOUGH_UNLOCKED_MONEY -37
#define WALLET_RPC_ERROR_CODE_NO_DAEMON_CONNECTION   -38
