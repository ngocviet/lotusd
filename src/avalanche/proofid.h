// Copyright (c) 2020 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_AVALANCHE_PROOFID_H
#define BITCOIN_AVALANCHE_PROOFID_H

#include <salteduint256hasher.h>
#include <uint256.h>

#include <string>

namespace avalanche {

struct ProofId : public uint256 {
    explicit ProofId() : uint256() {}
    explicit ProofId(const uint256 &b) : uint256(b) {}

    static ProofId fromHex(const std::string &str) {
        ProofId r;
        r.SetHex(str);
        return r;
    }
};

class SaltedProofIdHasher : private SaltedUint256Hasher {
public:
    SaltedProofIdHasher() : SaltedUint256Hasher() {}
    size_t operator()(const ProofId &proofid) const { return hash(proofid); }
};

} // namespace avalanche

#endif // BITCOIN_AVALANCHE_PROOFID_H
