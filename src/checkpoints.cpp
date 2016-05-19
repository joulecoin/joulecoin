// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

#include <stdint.h>

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    // How many times we expect transactions after the last checkpoint to
    // be slower. This number is a compromise, as it can't be accurate for
    // every system. When reindexing from a fast disk with a slow CPU, it
    // can be up to 20, while when downloading from a slow network with a
    // fast multicore CPU, it won't be much higher than 1.
    static const double SIGCHECK_VERIFICATION_FACTOR = 5.0;

    struct CCheckpointData {
        const MapCheckpoints *mapCheckpoints;
        int64_t nTimeLastCheckpoint;
        int64_t nTransactionsLastCheckpoint;
        double fTransactionsPerDay;
    };

    bool fEnabled = true;

    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    static MapCheckpoints mapCheckpoints =
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
        ;
    static const CCheckpointData data = {
        &mapCheckpoints,
        1463664285, // * UNIX timestamp of last checkpoint block
        2348961,    // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        1920        // * estimated number of transactions per day after checkpoint
    };

    static MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        ( 0, uint256("0x00000b82bf616429efb8ef55f10da775bf4a6ea54e72ce9c3d6510dd8af1616e"))
        ;
    static const CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1379797212,
        0,
        1920
    };

    static MapCheckpoints mapCheckpointsRegtest =
        boost::assign::map_list_of
        ( 0, uint256("0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"))
        ;
    static const CCheckpointData dataRegtest = {
        &mapCheckpointsRegtest,
        0,
        0,
        0
    };

    const CCheckpointData &Checkpoints() {
        if (Params().NetworkID() == CChainParams::TESTNET)
            return dataTestnet;
        else if (Params().NetworkID() == CChainParams::MAIN)
            return data;
        else
            return dataRegtest;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (!fEnabled)
            return true;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex, bool fSigchecks) {
        if (pindex==NULL)
            return 0.0;

        int64_t nNow = time(NULL);

        double fSigcheckVerificationFactor = fSigchecks ? SIGCHECK_VERIFICATION_FACTOR : 1.0;
        double fWorkBefore = 0.0; // Amount of work done before pindex
        double fWorkAfter = 0.0;  // Amount of work left after pindex (estimated)
        // Work is defined as: 1.0 per transaction before the last checkpoint, and
        // fSigcheckVerificationFactor per transaction after.

        const CCheckpointData &data = Checkpoints();

        if (pindex->nChainTx <= data.nTransactionsLastCheckpoint) {
            double nCheapBefore = pindex->nChainTx;
            double nCheapAfter = data.nTransactionsLastCheckpoint - pindex->nChainTx;
            double nExpensiveAfter = (nNow - data.nTimeLastCheckpoint)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore;
            fWorkAfter = nCheapAfter + nExpensiveAfter*fSigcheckVerificationFactor;
        } else {
            double nCheapBefore = data.nTransactionsLastCheckpoint;
            double nExpensiveBefore = pindex->nChainTx - data.nTransactionsLastCheckpoint;
            double nExpensiveAfter = (nNow - pindex->nTime)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore + nExpensiveBefore*fSigcheckVerificationFactor;
            fWorkAfter = nExpensiveAfter*fSigcheckVerificationFactor;
        }

        return fWorkBefore / (fWorkBefore + fWorkAfter);
    }

    int GetTotalBlocksEstimate()
    {
        if (!fEnabled)
            return 0;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (!fEnabled)
            return NULL;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
