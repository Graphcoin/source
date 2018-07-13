// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2015-2018 The Graphcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
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
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x000004dea769a63c5cd6af558fac02e139056502c828f57228cff06f94c6de93")) //gen block
    (13, uint256("0x00000122e4b9b92e93ace1b71f140baa4763da4406385e59c394f73865c742dd")) 
    (25, uint256("0x000003804c4d826e200070fedc65473e4a5c7b476df3689c930e4c75863135a5"))
    (50, uint256("0x000000b32dc6d2424df6208b3cfa7e3c7022c1868e2d2d94e3e9acb39e0e9644"))
    (100, uint256("0x0000009db21c27d26e76c8a7cb3b1f3459ea9c434cbf434e6b18d926a1821f78"))
    (200, uint256("0x000000b7b1f9ef56f15c854a3bc4079cb5c876867b67fe4201c9240dbb467354"))
    (400, uint256("0x000000495a380f0d098992257f2b0faac1812951a3733d849707e9b9fcf24aac"))
    (410, uint256("0x05bb3d62d49ae41df38babc71ba7f3a8ceb5847189408c57084ee78a2f831f9a"))
    (755, uint256("0x5df3bde188d090f9c5000fa600d611bb5c00d1ede58d713b0dbc369d3c1b9d6e"))
    (1500, uint256("0x9e6da3edf2222efdc8ea50be06b4126389346960d13b70f42daa6e9c153b0ccb"))
    (2503, uint256("0xcd194792a9bf5c71dab912228355ed89cd755a1e08fd8a295ee8e45f64449838"))
    (3502, uint256("0x5a6304431dc4a5a29236d6866ec88afeb5cf6d3670caef5279ae3e4351b20b86"))
    (4229, uint256("0x4880872f30841337e24de1e35794be8886698d358bf4997c31129fcbd1e5b1a8"))
      ;


static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1529589600, // * UNIX timestamp of last checkpoint block
    1,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    10000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0000014bd7a053eb417c79a53bf206733927fab837ad1a5e06265b21f8d9e9e8"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1522920805,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("000007c6a68a4e47f0acdcf0b13d98f410ebd84394be5cb2b2852f93105a1ced"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1522922005,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x39;
        pchMessageStart[1] = 0xb4;
        pchMessageStart[2] = 0xad;
        pchMessageStart[3] = 0xb8;
        vAlertPubKey = ParseHex("042e1bd74fb91a10d9c49b8c2723bc628e49fa9ceae90010fda8a2f683234230970387880e1efc5703fb847a14ddc27870b3da0346fc014e69c6978fda7e96628b");
        nDefaultPort = 22629;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // Graphcoin starting difficulty is 1 / 2^12
        //nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Graphcoin: 1 day
        nTargetSpacing = 1 * 60;  // Graphcoin: 1 minute
        nMaturity = 10;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 88000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 400;
        nBlockRequireMNSync = 1000;
        nModifierUpdateBlock = 999999999;
        nZerocoinStartHeight = 401;
        nZerocoinStartTime = 1531019246; // Thursday, June 21, 2018 2:00:00 PM GMT
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = ~1; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = ~1; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = ~1; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 1; // Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 268200*COIN; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 405;//99999999; //!> The block that zerocoin v2 becomes active - roughly Tuesday, May 8, 2018 4:00:00 AM GMT
        nEnforceNewSporkKey = 1531019246; //!> Sporks signed after (GMT):Saturday, June 23, 2018 2:00:00 PM GMT must use the new spork key
        nRejectOldSporkKey = 1531019246; //!> Fully reject old spork key after (GMT):  Friday, June 22, 2018 10:00:00 AM
        
        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "Not Everybody Was A Fan Of Meghan Markle's Wedding Veil";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 1 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("01c10e83b2703ccf322f7dbd62dd5855ac7c10bd055814ce121ba32607d573b8810c02c0582aed05b4deb9c4b77b26d92428c61256cd42774babea0a073b2ed0c9") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1531019246;
        genesis.nBits = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 1262281;

        /*
   		   hashGenesisBlock = uint256("0x01");
   		   if (true && genesis.GetHash() != hashGenesisBlock){
     		   fprintf(stderr, "old mainnet genesis nonce: %d\n", genesis.nNonce);
     		   fprintf(stderr, "old mainnet genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
     		   // deliberately empty for loop finds nonce value.
     		   for(genesis.nNonce = 0; genesis.GetHash() > bnProofOfWorkLimit; genesis.nNonce++){ } 
     		   fprintf(stderr, "new mainnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
     		   fprintf(stderr, "new mainnet genesis nonce: %d\n", genesis.nNonce);
     		   fprintf(stderr, "new mainnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
   		   }
         */

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x000004dea769a63c5cd6af558fac02e139056502c828f57228cff06f94c6de93"));
        assert(genesis.hashMerkleRoot == uint256("0xc5fde2cb354cbb473bb6f9a3148b7a3aae399fc9c2c090a5d54edfb430fe3cca"));
        
        
        vSeeds.push_back(CDNSSeedData("0", "188.166.156.88"));              
        vSeeds.push_back(CDNSSeedData("1", "167.99.179.227"));
        vSeeds.push_back(CDNSSeedData("2", "192.241.137.186"));           
	      vSeeds.push_back(CDNSSeedData("3", "128.199.95.1"));           
	      vSeeds.push_back(CDNSSeedData("4", "37.139.16.230"));           
	      vSeeds.push_back(CDNSSeedData("5", "107.170.231.203"));          
        vSeeds.push_back(CDNSSeedData("6", "206.189.53.32"));   
        
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 97);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 98);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 125);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x05)(0x88)(0x00)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x05)(0x88)(0x00)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x77)(0x77).convert_to_container<std::vector<unsigned char> >();
        

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "046242c3420f4092b78339ce21f34e827ce8f7193ed319a2e662d64ee07f73679e11d138a426f70e39c6baa8935386eab71abcd1aba186b58ac5c7fd9c05bc4669";
        strSporkKeyOld = "04B433E6598390C992F4F022F20D3B4CBBE691652EE7C48243B81701CBDB7CC7D7BF0EE09E154E6FCBF2043D65AF4E9E97B89B5DBAF830D83B9B7F469A6C45A717";
        strObfuscationPoolDummyAddress = "d87q2gC9j6nNrnzCsg4aY6bHMLsT9nUhEw";
        nStartMasternodePayments = 1529784000; //Saturday, June 23, 2018 8:00:00 PM GMT

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zgrph to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
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
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x50;
        pchMessageStart[1] = 0x79;
        pchMessageStart[2] = 0x55;
        pchMessageStart[3] = 0xcd;
        vAlertPubKey = ParseHex("000010e83b2703ccf322f7dbd62dd5855ac7c10bd055814ce121ba32607d573b8810c02c0582aed05b4deb9c4b77b26d92428c61256cd42774babea0a073b2ed0c9");
        nDefaultPort = 51484;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Graphcoin: 1 day
        nTargetSpacing = 1 * 60;  // Graphcoin: 1 minute
        nLastPOWBlock = 200;
        nBlockRequireMNSync = 1000;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;
        nZerocoinStartHeight = 201576;
        nZerocoinStartTime = 1501776000;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 9902850; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 444020; //!> The block that zerocoin v2 becomes active
        nEnforceNewSporkKey = 1521604800; //!> Sporks signed after Wednesday, March 21, 2018 4:00:00 AM GMT must use the new spork key
        nRejectOldSporkKey = 1522454400; //!> Reject old spork key after Saturday, March 31, 2018 12:00:00 AM GMT

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1522920805;
        genesis.nNonce = 1254338;

        hashGenesisBlock = genesis.GetHash();
        //assert(hashGenesisBlock == uint256("0x0000014bd7a053eb417c79a53bf206733927fab837ad1a5e06265b21f8d9e9e8"));

        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "graphcoin-testnet.seed.fuzzbawls.pw"));
        //vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "graphcoin-testnet.seed2.fuzzbawls.pw"));
        //vSeeds.push_back(CDNSSeedData("s3v3nh4cks.ddns.net", "s3v3nh4cks.ddns.net"));
        //vSeeds.push_back(CDNSSeedData("192.168.100.2", "192.168.100.2"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 129); // Testnet graphcoin addresses start with 't' or 'u'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 14);  // Testnet graphcoin script addresses start with '6' or '7'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet graphcoin BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x2a)(0x40)(0x71)(0xb0).convert_to_container<std::vector<unsigned char> >();
        // Testnet graphcoin BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x2a)(0x40)(0x78)(0x47).convert_to_container<std::vector<unsigned char> >();
        // Testnet graphcoin BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04A8B319388C0F8588D238B9941DC26B26D3F9465266B368A051C5C100F79306A557780101FE2192FE170D7E6DEFDCBEE4C8D533396389C0DAFFDBC842B002243C";
        strSporkKeyOld = "04348C2F50F90267E64FACC65BFDC9D0EB147D090872FB97ABAE92E9A36E6CA60983E28E741F8E7277B11A7479B626AC115BA31463AC48178A5075C5A9319D4A38";
        strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
        nStartMasternodePayments = 1524201152; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
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
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xb1;
        pchMessageStart[1] = 0xdf;
        pchMessageStart[2] = 0x5c;
        pchMessageStart[3] = 0xae;
        //nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Graphcoin: 1 day
        nTargetSpacing = 1 * 60;        // Graphcoin: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1522922005;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 155014;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 51486;
        //assert(hashGenesisBlock == uint256("0x000007c6a68a4e47f0acdcf0b13d98f410ebd84394be5cb2b2852f93105a1ced"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

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
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51488;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

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
    //virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
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

void SelectParams(CBaseChainParams::Network network)
{
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
