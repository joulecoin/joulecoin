// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "assert.h"
#include "core.h"
#include "protocol.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

//
// Main network
//

unsigned int pnSeed[] =
{
    0x12345678
};

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xa5;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0x79;
        pchMessageStart[3] = 0x55;
        vAlertPubKey = ParseHex("0404648d5a731fee50c1d7c8044c3c05010a24815d95551de533d63b37f0ee713c2c7a46440dfe3c4d441c3c7353d7291fbacab272406567fa5fa2194d59ef2494");
        nDefaultPort = 26789;
        nRPCPort = 8844;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);
        nSubsidyHalvingInterval = 1401600; // 2 years

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        const char* pszTimestamp = "Slashdot 22 Sep 2013 - RSA Warns Developers Not To Use RSA Products";
        CTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CBigNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1379833106;
        genesis.nBits    = 0x1e0fffff;
        genesis.nNonce   = 2092237480;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x0000077e5cce889f0920029bf89e8ecb16f7be38e1019c3e21c26d4687ce11f5"));
        assert(genesis.hashMerkleRoot == uint256("0x2d089dcbae340c48fef8b956bfe63806c0ffd592d1d333082bfa8da6c4158e83"));

        vSeeds.push_back(CDNSSeedData("seed1.joulecoin.org", "seed1.joulecoin.org"));
        vSeeds.push_back(CDNSSeedData("seed2.joulecoin.org", "seed2.joulecoin.org"));
        vSeeds.push_back(CDNSSeedData("seed3.joulecoin.org", "seed3.joulecoin.org"));
        vSeeds.push_back(CDNSSeedData("seed4.joulecoin.org", "seed4.joulecoin.org"));
        vSeeds.push_back(CDNSSeedData("seed5.joulecoin.org", "seed5.joulecoin.org"));
        vSeeds.push_back(CDNSSeedData("seed6.joulecoin.org", "seed6.joulecoin.org"));
        vSeeds.push_back(CDNSSeedData("seed7.joulecoin.org", "seed7.joulecoin.org"));
        vSeeds.push_back(CDNSSeedData("seed8.joulecoin.org", "seed8.joulecoin.org"));
        vSeeds.push_back(CDNSSeedData("joulecoin1.chickenkiller.com", "joulecoin1.chickenkiller.com"));
        vSeeds.push_back(CDNSSeedData("joulecoin2.crabdance.com", "joulecoin2.crabdance.com"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(43);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(11);
        base58Prefixes[SECRET_KEY] =     list_of(143);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

        // Convert the pnSeeds array into usable address objects.
        for (unsigned int i = 0; i < ARRAYLEN(pnSeed); i++)
        {
            // It'll only connect to one or two seed nodes because once it connects,
            // it'll get a pile of addresses with newer timestamps.
            // Seed nodes are given a random 'last seen time' of between one and two
            // weeks ago.
            const int64_t nOneWeek = 2 * 24 * 60 * 60;
            struct in_addr ip;
            memcpy(&ip, &pnSeed[i], sizeof(ip));
            CAddress addr(CService(ip, GetDefaultPort()));
            addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
            vFixedSeeds.push_back(addr);
        }
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet (v3)
//
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x0a;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0x73;
        pchMessageStart[3] = 0x12;
        vAlertPubKey = ParseHex("048b3afe0ed78e22b6f4cc42f83c287400a253361ae858efdcbe7d24e426e4cca2692408f78aff8406e75c66ff83cef3e1c4cc1552ddd23215d9ff4cd61315e418");
        nDefaultPort = 26783;
        nRPCPort = 8844;
        strDataDir = "testnet";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1379797212;
        genesis.nNonce = 415605766;
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000b82bf616429efb8ef55f10da775bf4a6ea54e72ce9c3d6510dd8af1616e"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("testseed1.joulecoin.org", "testseed1.joulecoin.org"));        	

        base58Prefixes[PUBKEY_ADDRESS] = list_of(83);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(13);
        base58Prefixes[SECRET_KEY]     = list_of(212);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94);
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0x0f;
        pchMessageStart[2] = 0xc5;
        pchMessageStart[3] = 0x5c;
        nSubsidyHalvingInterval = 150;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1296688602;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 3;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18444;
        strDataDir = "regtest";
        // assert(hashGenesisBlock == uint256("0x0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
