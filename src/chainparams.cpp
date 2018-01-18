// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
static Checkpoints::MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (      0, uint256("0x0000077e5cce889f0920029bf89e8ecb16f7be38e1019c3e21c26d4687ce11f5"))
        (  21998, uint256("0x000000000029b7b1ea497dd917ba5bb78b8453759acc145918c6446205fd7e49"))
        (  34124, uint256("0x000000000019c6d8dbd8f693077b5b09dc0df722f4b548ca6cbc9e712aa94935"))
        (  68073, uint256("0x00000000000ed8b4432909814f2f0a6a699625833d0e4b88fe69a5807f116ae0"))
        (  90016, uint256("0x0000000000156c5c7e98211d60c1bd644ca6cfb0b8ef8b484adc663708d64e5b"))
        ( 189383, uint256("0x0000000000013e11c889a3d1ebb0d1833227ffa7fb6d06f7ce42e8d8e1fb7dce"))
        ( 245624, uint256("0x00000000000142ce3a43b4304ab6f886c6fd9a806d83a72c344b24a7d24da7fc"))
        ( 328190, uint256("0x000000000000f2f362d4f78cad2fa5c03452b90213a29b6be9c94827ce73e1b3"))
        ( 386194, uint256("0x000000000001bf145fa37e30dea68857ea0248548f71f02d413ec9a1dd5db3f5"))
        ( 531401, uint256("0x000000000000bd31475b4f382103a3f62202dbaf680decb86fa5f8193feda765"))
        ( 572400, uint256("0x000000000001d794102460c50af76523672953ca17797624a0c7d6076f7d0023"))
        ( 661339, uint256("0x000000000000f17fbee03e58700c625b4a1eca8d41fca6aef505c4d928a88aa4"))
        ( 957592, uint256("0x0000000000006461aa6dc976cb61e010b4b794e6ce904146ce3f781df0eeaf60"))
        (1082978, uint256("0x000000000000a55c12da9d532c5c19ac53ad7d25b4b67aca77adad8191752e6e"))
        (1150502, uint256("0x000000000000660efa747fee365969d424965bfdebbc6feb034863608241e2c3"))
        (1908153, uint256("0x00000000000148056ebc887282146af6e0cb267ecd83bb71105afddc5706f066"))
        (2600671, uint256("0x0000000000004c3bfb23dca6507f29805f4e42247542e433fdae49d8d5ca6bed"))
        ;
static const Checkpoints::CCheckpointData data = {
        &mapCheckpoints,
        1495074242, // * UNIX timestamp of last checkpoint block
        3142416,    // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        1920        // * estimated number of transactions per day after checkpoint
    };

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        ( 0, uint256("0x00000b82bf616429efb8ef55f10da775bf4a6ea54e72ce9c3d6510dd8af1616e"))
        ;
static const Checkpoints::CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1379797212,
        0,
        1920
    };

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
        boost::assign::map_list_of
        ( 0, uint256("0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"))
        ;
static const Checkpoints::CCheckpointData dataRegtest = {
        &mapCheckpointsRegtest,
        0,
        0,
        0
    };

class CMainParams : public CChainParams {
public:
    CMainParams() {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /** 
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xa5;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0x79;
        pchMessageStart[3] = 0x55;
        vAlertPubKey = ParseHex("0404648d5a731fee50c1d7c8044c3c05010a24815d95551de533d63b37f0ee713c2c7a46440dfe3c4d441c3c7353d7291fbacab272406567fa5fa2194d59ef2494");
        nDefaultPort = 26789;
        bnProofOfWorkLimit = ~uint256(0) >> 20;
        nSubsidyHalvingInterval = 1401600; // 2 years
        nEnforceBlockUpgradeMajority = 7500;
        nRejectBlockOutdatedMajority = 9500;
        nToCheckBlockUpgradeMajority = 10000;
        nMinerThreads = 0;
        nTargetTimespan = 45; 
        nTargetSpacing = 45;
        nMaxTipAge = 24 * 60 * 60;

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         * 
         * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
         *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
         *   vMerkleTree: 4a5e1e
         */
        const char* pszTimestamp = "Slashdot 22 Sep 2013 - RSA Warns Developers Not To Use RSA Products";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
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

        vSeeds.push_back(CDNSSeedData("seed1.jouleco.in", "seed1.jouleco.in"));
        vSeeds.push_back(CDNSSeedData("seed2.jouleco.in", "seed2.jouleco.in"));
        vSeeds.push_back(CDNSSeedData("seed3.jouleco.in", "seed3.jouleco.in"));
        vSeeds.push_back(CDNSSeedData("seed4.jouleco.in", "seed4.jouleco.in"));
        vSeeds.push_back(CDNSSeedData("joulecoin1.chickenkiller.com", "joulecoin1.chickenkiller.com"));
        vSeeds.push_back(CDNSSeedData("joulecoin2.crabdance.com", "joulecoin2.crabdance.com"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(43);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(11);
        base58Prefixes[SECRET_KEY] =     list_of(143);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);
        
        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x0a;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0x73;
        pchMessageStart[3] = 0x12;
        vAlertPubKey = ParseHex("048b3afe0ed78e22b6f4cc42f83c287400a253361ae858efdcbe7d24e426e4cca2692408f78aff8406e75c66ff83cef3e1c4cc1552ddd23215d9ff4cd61315e418");
        nDefaultPort = 26783;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 14 * 24 * 60 * 60; //! two weeks
        nTargetSpacing = 10 * 60;
        nMaxTipAge = 0x7fffffff;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1379797212;
        genesis.nNonce = 415605766;
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000b82bf616429efb8ef55f10da775bf4a6ea54e72ce9c3d6510dd8af1616e"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("testseed1.jouleco.in", "testseed1.jouleco.in"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(83);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(13);
        base58Prefixes[SECRET_KEY]     = list_of(212);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94);

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0x0f;
        pchMessageStart[2] = 0xc5;
        pchMessageStart[3] = 0x5c;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 14 * 24 * 60 * 60; //! two weeks
        nTargetSpacing = 10 * 60;
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        nMaxTipAge = 24 * 60 * 60;
        genesis.nTime = 1296688602;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 3;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18444;
        // assert(hashGenesisBlock == uint256("0x0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams {
public:
    CUnitTestParams() {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 18445;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval)  { nSubsidyHalvingInterval=anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority)  { nEnforceBlockUpgradeMajority=anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority)  { nRejectBlockOutdatedMajority=anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority)  { nToCheckBlockUpgradeMajority=anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks)  { fDefaultConsistencyChecks=afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) {  fAllowMinDifficultyBlocks=afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams *pCurrentParams = 0;

CModifiableParams *ModifiableParams()
{
   assert(pCurrentParams);
   assert(pCurrentParams==&unitTestParams);
   return (CModifiableParams*)&unitTestParams;
}

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        case CBaseChainParams::UNITTEST:
            return unitTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
