#!/usr/bin/env python3
# Copyright (c) 2015-2019 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
"""Test BIP66 (DER SIG).

Test that the DERSIG soft-fork activates at (regtest) height 1251.
"""

from test_framework.blocktools import create_block, create_coinbase, create_transaction, prepare_block
from test_framework.messages import msg_block
from test_framework.mininode import P2PInterface
from test_framework.script import CScript
from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import assert_equal

DERSIG_HEIGHT = 1251

# A canonical signature consists of:
# <30> <total len> <02> <len R> <R> <02> <len S> <S> <hashtype>


def unDERify(tx):
    """
    Make the signature in vin 0 of a tx non-DER-compliant,
    by adding padding after the S-value.
    """
    scriptSig = CScript(tx.vin[0].scriptSig)
    newscript = []
    for i in scriptSig:
        if (len(newscript) == 0):
            newscript.append(i[0:-1] + b'\0' + i[-1:])
        else:
            newscript.append(i)
    tx.vin[0].scriptSig = CScript(newscript)


class BIP66Test(BitcoinTestFramework):
    def set_test_params(self):
        self.num_nodes = 1
        self.extra_args = [['-whitelist=noban@127.0.0.1',
                            '-allownonstdtxnconsensus=1']]
        self.setup_clean_chain = True
        self.rpc_timeout = 120

    def skip_test_if_missing_module(self):
        self.skip_if_no_wallet()

    def run_test(self):
        self.nodes[0].add_p2p_connection(P2PInterface())

        self.log.info("Mining {} blocks".format(DERSIG_HEIGHT - 1))
        self.coinbase_txids = [self.nodes[0].getblock(
            b)['tx'][0] for b in self.nodes[0].generate(DERSIG_HEIGHT - 1)]
        self.nodeaddress = self.nodes[0].getnewaddress()

        self.log.info(
            "Test that transactions with non-DER signatures cannot appear in a block")
        tip = self.nodes[0].getbestblockhash()
        block_time = self.nodes[0].getblockheader(tip)['mediantime'] + 1
        block = create_block(
            int(tip, 16), create_coinbase(DERSIG_HEIGHT), DERSIG_HEIGHT, block_time)
        spendtx = create_transaction(self.nodes[0], self.coinbase_txids[1],
                                     self.nodeaddress, amount=1.0, vout=1)
        unDERify(spendtx)
        spendtx.rehash()

        # First we show that this tx is valid except for DERSIG by getting it
        # rejected from the mempool for exactly that reason.
        assert_equal(
            [{'txid': spendtx.txid_hex, 'allowed': False,
                'reject-reason': 'mandatory-script-verify-flag-failed (Non-canonical DER signature)'}],
            self.nodes[0].testmempoolaccept(
                rawtxs=[spendtx.serialize().hex()], maxfeerate=0)
        )

        # Now we verify that a block with this transaction is also invalid.
        block.vtx.append(spendtx)
        prepare_block(block)

        with self.nodes[0].assert_debug_log(expected_msgs=['ConnectBlock {} failed, blk-bad-inputs'.format(block.hash)]):
            self.nodes[0].p2p.send_and_ping(msg_block(block))
            assert_equal(self.nodes[0].getbestblockhash(), tip)
            self.nodes[0].p2p.sync_with_ping()

        self.log.info(
            "Test that a version 3 block with a DERSIG-compliant transaction is accepted")
        block.vtx[1] = create_transaction(self.nodes[0],
                                          self.coinbase_txids[1], self.nodeaddress, amount=1.0, vout=1)
        prepare_block(block)

        self.nodes[0].p2p.send_and_ping(msg_block(block))
        assert_equal(int(self.nodes[0].getbestblockhash(), 16), block.sha256)


if __name__ == '__main__':
    BIP66Test().main()
