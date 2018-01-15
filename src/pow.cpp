// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "chain.h"
#include "chainparams.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"

static const int64_t nTargetTimespan = 45; // 45 seconds
static const int64_t nTargetSpacing = 45; // 45 seconds
static const int64_t nInterval = nTargetTimespan / nTargetSpacing; // Joulecoin: retarget every block

static const int64_t nHeightVer2 = 32000;
// static unsigned int nCheckpointTimeVer2 = 1380608826;

static const int64_t nHeightVer3 = 90000;
//static unsigned int nCheckpointTimeVer3 = 1384372762;

static const int64_t nAveragingInterval1 = nInterval * 160; // 160 blocks
static const int64_t nAveragingTargetTimespan1 = nAveragingInterval1 * nTargetSpacing; // 120 minutes

static const int64_t nAveragingInterval2 = nInterval * 8; // 8 blocks
static const int64_t nAveragingTargetTimespan2 = nAveragingInterval2 * nTargetSpacing; // 6 minutes

static const int64_t nAveragingInterval3 = nAveragingInterval2; // 8 blocks
static const int64_t nAveragingTargetTimespan3 = nAveragingTargetTimespan2; // 6 minutes

static const int64_t nMaxAdjustDown1 = 10; // 10% adjustment down
static const int64_t nMaxAdjustUp1 = 1; // 1% adjustment up

static const int64_t nMaxAdjustDown2 = 1; // 1% adjustment down
static const int64_t nMaxAdjustUp2 = 1; // 1% adjustment up

static const int64_t nMaxAdjustDown3 = 3; // 3% adjustment down
static const int64_t nMaxAdjustUp3 = 1; // 1% adjustment up

static const int64_t nTargetTimespanAdjDown1 = nTargetTimespan * (100 + nMaxAdjustDown1) / 100;
static const int64_t nTargetTimespanAdjDown2 = nTargetTimespan * (100 + nMaxAdjustDown2) / 100;
static const int64_t nTargetTimespanAdjDown3 = nTargetTimespan * (100 + nMaxAdjustDown3) / 100;

static const int64_t nMinActualTimespan1 = nAveragingTargetTimespan1 * (100 - nMaxAdjustUp1) / 100;
static const int64_t nMaxActualTimespan1 = nAveragingTargetTimespan1 * (100 + nMaxAdjustDown1) / 100;

static const int64_t nMinActualTimespan2 = nAveragingTargetTimespan2 * (100 - nMaxAdjustUp2) / 100;
static const int64_t nMaxActualTimespan2 = nAveragingTargetTimespan2 * (100 + nMaxAdjustDown2) / 100;

static const int64_t nMinActualTimespan3 = nAveragingTargetTimespan3 * (100 - nMaxAdjustUp3) / 100;
static const int64_t nMaxActualTimespan3 = nAveragingTargetTimespan3 * (100 + nMaxAdjustDown3) / 100;

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock)
{
    unsigned int nProofOfWorkLimit = Params().ProofOfWorkLimit().GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;
    if (pindexLast->nHeight+1 < nAveragingInterval1)
        return nProofOfWorkLimit;

    if (Params().AllowMinDifficultyBlocks())
    {
        // Special difficulty rule for testnet:
        // If the new block's timestamp is more than 2* 10 minutes
        // then allow mining of a min-difficulty block.
        if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + Params().TargetSpacing()*2)
            return nProofOfWorkLimit;
        else
        {
            // Return the last non-special-min-difficulty-rules-block
            const CBlockIndex* pindex = pindexLast;
            while (pindex->pprev && pindex->nHeight % Params().Interval() != 0 && pindex->nBits == nProofOfWorkLimit)
                pindex = pindex->pprev;
            return pindex->nBits;
        }
    }

    int64_t nAveragingInterval;
    int64_t nMinActualTimespan;
    int64_t nMaxActualTimespan;
    int64_t nAveragingTargetTimespan;

    if (pindexLast->nHeight+1 >= nHeightVer3)
    {
        nAveragingInterval = nAveragingInterval3;
        nMinActualTimespan = nMinActualTimespan3;
        nMaxActualTimespan = nMaxActualTimespan3;
        nAveragingTargetTimespan = nAveragingTargetTimespan3;
    }
    else
    {
        if (pindexLast->nHeight+1 >= nHeightVer2)
        {
            nAveragingInterval = nAveragingInterval2;
            nMinActualTimespan = nMinActualTimespan2;
            nMaxActualTimespan = nMaxActualTimespan2;
            nAveragingTargetTimespan = nAveragingTargetTimespan2;
        }
        else
        {
            nAveragingInterval = nAveragingInterval1;
            nMinActualTimespan = nMinActualTimespan1;
            nMaxActualTimespan = nMaxActualTimespan1;
            nAveragingTargetTimespan = nAveragingTargetTimespan1;
        }
    }

    // Go back by what we want to be nAveragingInterval worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < nAveragingInterval-1; i++)
        pindexFirst = pindexFirst->pprev;
    assert(pindexFirst);

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();
    LogPrintf("  nActualTimespan = %d  before bounds\n", nActualTimespan);
    if (nActualTimespan < nMinActualTimespan)
        nActualTimespan = nMinActualTimespan;
    if (nActualTimespan > nMaxActualTimespan)
        nActualTimespan = nMaxActualTimespan;

    // Retarget
    uint256 bnNew;
    uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    bnNew *= nActualTimespan;
    bnNew /= nAveragingTargetTimespan;

    if (bnNew > Params().ProofOfWorkLimit())
        bnNew = Params().ProofOfWorkLimit();

    /// debug print
    LogPrintf("GetNextWorkRequired RETARGET\n");
    LogPrintf("Params().TargetTimespan() = %d    nActualTimespan = %d\n", Params().TargetTimespan(), nActualTimespan);
    LogPrintf("Before: %08x  %s\n", pindexLast->nBits, bnOld.ToString());
    LogPrintf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString());

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits)
{
    bool fNegative;
    bool fOverflow;
    uint256 bnTarget;

    if (Params().SkipProofOfWorkCheck())
       return true;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > Params().ProofOfWorkLimit())
        return error("CheckProofOfWork() : nBits below minimum work");

    // Check proof of work matches claimed amount
    if (hash > bnTarget)
        return error("CheckProofOfWork() : hash doesn't match nBits");

    return true;
}

uint256 GetBlockProof(const CBlockIndex& block)
{
    uint256 bnTarget;
    bool fNegative;
    bool fOverflow;
    bnTarget.SetCompact(block.nBits, &fNegative, &fOverflow);
    if (fNegative || fOverflow || bnTarget == 0)
        return 0;
    // We need to compute 2**256 / (bnTarget+1), but we can't represent 2**256
    // as it's too large for a uint256. However, as 2**256 is at least as large
    // as bnTarget+1, it is equal to ((2**256 - bnTarget - 1) / (bnTarget+1)) + 1,
    // or ~bnTarget / (nTarget+1) + 1.
    return (~bnTarget / (bnTarget + 1)) + 1;
}
