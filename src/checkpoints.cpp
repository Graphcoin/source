// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 5000;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (0,	Params().HashGenesisBlock() )
        (11, uint256("0xb3798d23f3e6bade2230c63728f7e16536fa9cc7f23be13d96218ab35155ea5e"))
		    (26, uint256("0xab8bdf39be113cb94d2b22979263d9bd68d312cacf2cf9df8d1ca193a6d9dfaf"))
		    (42, uint256("0x273a26405772b3d81fec192432539260fdc4b925b8b52940a0f6d28d0b2aad51"))
		    (123, uint256("0xd6afa60569f3b3029b0e7e19d3590ea2b16d68cad438971f4d32c9a9c153c2a7"))
		    (527, uint256("0x329cb270f5c494dca1b682fdeb84fb07ae60595c54b406562d503d1f8c251711"))
		    (1248, uint256("0xe8fbcf6ac5798c99933d06f30ff96e95dede60e65149f953f232305e52cf5fbe"))
		    (1733, uint256("0x02c8e877e2f5b27e86f885cf75eb4625d99cf0382f05894a90c46c03be073200"))
		    (2115, uint256("0x495f003837b31e0732977b6e195dcda156d658488188a77281f2d9e7d5dfc1e0"))
  		  (2180, uint256("0x843338333f76dd5dcfe3418aec769272d7e244a1260b2310fe993048e70f56b1"))
          ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
