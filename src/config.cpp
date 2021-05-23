// Copyright (c) 2017-2019 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <config.h>

#include <chainparams.h>
#include <consensus/consensus.h> // DEFAULT_MAX_BLOCK_SIZE

GlobalConfig::GlobalConfig()
    : useCashAddr(false), enableMinerFund(MinerFundStatus::Unset),
      nMaxBlockSize(DEFAULT_MAX_BLOCK_SIZE) {}

bool GlobalConfig::SetMaxBlockSize(uint64_t maxBlockSize) {
    // Do not allow maxBlockSize to be set below historic 1MB limit
    // It cannot be equal either because of the "must be big" UAHF rule.
    if (maxBlockSize <= LEGACY_MAX_BLOCK_SIZE) {
        return false;
    }

    nMaxBlockSize = maxBlockSize;
    return true;
}

uint64_t GlobalConfig::GetMaxBlockSize() const {
    return nMaxBlockSize;
}

const CChainParams &GlobalConfig::GetChainParams() const {
    return Params();
}

static GlobalConfig gConfig;

const Config &GetConfig() {
    return gConfig;
}

void GlobalConfig::SetCashAddrEncoding(bool c) {
    useCashAddr = c;
}
bool GlobalConfig::UseCashAddrEncoding() const {
    return useCashAddr;
}

void GlobalConfig::SetEnableMinerFund(bool c) {
    enableMinerFund = c ? MinerFundStatus::Enabled : MinerFundStatus::Disabled;
}
bool GlobalConfig::EnableMinerFund() const {
    return enableMinerFund == MinerFundStatus::Unset
               ? GetChainParams().GetConsensus().enableMinerFund
               : enableMinerFund == MinerFundStatus::Enabled;
}

DummyConfig::DummyConfig()
    : chainParams(CreateChainParams(CBaseChainParams::REGTEST)),
      enableMinerFund(chainParams->GetConsensus().enableMinerFund) {}

DummyConfig::DummyConfig(std::string net)
    : chainParams(CreateChainParams(net)),
      enableMinerFund(chainParams->GetConsensus().enableMinerFund) {}

DummyConfig::DummyConfig(std::unique_ptr<CChainParams> chainParamsIn)
    : chainParams(std::move(chainParamsIn)),
      enableMinerFund(chainParams->GetConsensus().enableMinerFund) {}

void DummyConfig::SetChainParams(std::string net) {
    chainParams = CreateChainParams(net);
}

void GlobalConfig::SetExcessUTXOCharge(Amount fee) {
    excessUTXOCharge = fee;
}

Amount GlobalConfig::GetExcessUTXOCharge() const {
    return excessUTXOCharge;
}

void DummyConfig::SetEnableMinerFund(bool c) {
    enableMinerFund = c;
}
bool DummyConfig::EnableMinerFund() const {
    return enableMinerFund;
}
