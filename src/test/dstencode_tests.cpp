// Copyright (c) 2017-2019 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <key_io.h>

#include <chainparams.h>
#include <config.h>

#include <test/util/setup_common.h>

#include <boost/test/unit_test.hpp>

namespace {

class DummyCashAddrConfig : public DummyConfig {
public:
    DummyCashAddrConfig()
        : DummyConfig(CBaseChainParams::MAIN), useCashAddr(false) {}
    void SetCashAddrEncoding(bool b) override { useCashAddr = b; }
    bool UseCashAddrEncoding() const override { return useCashAddr; }

private:
    bool useCashAddr;
};

} // namespace

BOOST_FIXTURE_TEST_SUITE(dstencode_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(test_addresses) {
    std::vector<uint8_t> hash = {118, 160, 64,  83,  189, 160, 168,
                                 139, 218, 81,  119, 184, 106, 21,
                                 195, 178, 159, 85,  152, 115};

    const CTxDestination dstKey = PKHash(uint160(hash));
    const CTxDestination dstScript = ScriptHash(uint160(hash));

    std::string cashaddr_pubkey =
        "bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a";
    std::string cashaddr_script =
        "bitcoincash:ppm2qsznhks23z7629mms6s4cwef74vcwvn0h829pq";
    std::string lotus_pubkey = "lotus_16PSJLk9W86KAZp26x3uM176w6N9vUU8YNQLVBwUQ";
    std::string lotus_script = "lotus_1PrQz5R11Ae1YcbvUpGDSvzPP2GsVw6EDCvZqy";

    DummyCashAddrConfig config;

    // Check encoding
    config.SetCashAddrEncoding(true);
    BOOST_CHECK_EQUAL(cashaddr_pubkey, EncodeDestination(dstKey, config));
    BOOST_CHECK_EQUAL(cashaddr_script, EncodeDestination(dstScript, config));
    config.SetCashAddrEncoding(false);
    BOOST_CHECK_EQUAL(lotus_pubkey, EncodeDestination(dstKey, config));
    BOOST_CHECK_EQUAL(lotus_script, EncodeDestination(dstScript, config));

    // Check decoding
    const CChainParams &params = config.GetChainParams();
    BOOST_CHECK(dstKey == DecodeDestination(cashaddr_pubkey, params));
    BOOST_CHECK(dstScript == DecodeDestination(cashaddr_script, params));
    BOOST_CHECK(dstKey == DecodeDestination(lotus_pubkey, params));
    BOOST_CHECK(dstScript == DecodeDestination(lotus_script, params));

    // Validation
    BOOST_CHECK(IsValidDestinationString(cashaddr_pubkey, params));
    BOOST_CHECK(IsValidDestinationString(cashaddr_script, params));
    BOOST_CHECK(IsValidDestinationString(lotus_pubkey, params));
    BOOST_CHECK(IsValidDestinationString(lotus_script, params));
    BOOST_CHECK(!IsValidDestinationString("notvalid", params));
}

BOOST_AUTO_TEST_SUITE_END()
