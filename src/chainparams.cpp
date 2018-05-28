// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"
#include "base58.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
	uint8_t addr[16];
	uint16_t port;
};

#include "chainparamsseeds.h"

//
// Main network
//

// Convert the pnSeeds array into usable address objects.
static void convertSeeds(std::vector<CAddress> &vSeedsOut, const unsigned int *data, unsigned int count, int port)
{
	// It'll only connect to one or two seed nodes because once it connects,
	// it'll get a pile of addresses with newer timestamps.
	// Seed nodes are given a random 'last seen time' of between one and two
	// weeks ago.
	const int64_t nOneWeek = 7*24*60*60;
	for (unsigned int k = 0; k < count; ++k)
	{
		struct in_addr ip;
		unsigned int i = data[k], t;

		// -- convert to big endian
		t =   (i & 0x000000ff) << 24u
			| (i & 0x0000ff00) << 8u
			| (i & 0x00ff0000) >> 8u
			| (i & 0xff000000) >> 24u;

		memcpy(&ip, &t, sizeof(ip));

		CAddress addr(CService(ip, port));
		addr.nTime = GetTime()-GetRand(nOneWeek)-nOneWeek;
		vSeedsOut.push_back(addr);
	}
}

class CMainParams : public CChainParams {
	public:
		CMainParams() {
			// The message start string is designed to be unlikely to occur in normal data.
			// The characters are rarely used upper ASCII, not valid as UTF-8, and produce
			// a large 4-byte int at any alignment.
			pchMessageStart[0] = 0xd7;
			pchMessageStart[1] = 0xb2;
			pchMessageStart[2] = 0xf1;
			pchMessageStart[3] = 0xa6;
			vAlertPubKey = ParseHex("042e1bd74fb91a10d9c49b8c2723bc628e49fa9ceae90010fda8a2f683234230970387880e1efc5703fb847a14ddc27870b3da0346fc014e69c6978fda7e96628b");
			nDefaultPort = 7119;
			nRPCPort = 7120;
			bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);

			// Build the genesis block.
			// transaction:  Coinbase(hash=3c8eb5ab2277426f2f48bd70eeb32416b1c94456a3e56e1871d15b568dde3e1e, nTime=1516608000, ver=1, vin.size=1, vout.size=1, nLockTime=0)
			// CTxIn(COutPoint(0000000000, 4294967295), coinbase 00012a284f63742032322c20323031373a20426974636f696e2070726963652061626f76652024362c303030)
			// CTxOut(empty)

			const char* pszTimestamp = "Not Everybody Was A Fan Of Meghan Markle's Wedding Veil";
			std::vector<CTxIn> vin;
			vin.resize(1);
			vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
			std::vector<CTxOut> vout;
			vout.resize(1);
			vout[0].SetEmpty();
			CTransaction txNew(1, 1527032810, vin, vout, 0);

			LogPrintf("Genesis mainnet Transaction:  %s\n", txNew.ToString().c_str());

			genesis.vtx.push_back(txNew);

			genesis.hashPrevBlock = 0;
			genesis.hashMerkleRoot = genesis.BuildMerkleTree();
			genesis.nVersion = 1;
			genesis.nTime    = 1527032810;
			genesis.nBits = bnProofOfWorkLimit.GetCompact(); //520159231
			genesis.nNonce   = 7569;


			/*
			   hashGenesisBlock = uint256("0x01");
			   if (true && genesis.GetHash() != hashGenesisBlock){
			   fprintf(stderr, "old mainnet genesis nonce: %d\n", genesis.nNonce);
			   fprintf(stderr, "old mainnet genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
			// deliberately empty for loop finds nonce value.
			for(genesis.nNonce = 0; CBigNum(genesis.GetHash()) > bnProofOfWorkLimit; genesis.nNonce++){ } 
			fprintf(stderr, "new mainnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
			fprintf(stderr, "new mainnet genesis nonce: %d\n", genesis.nNonce);
			fprintf(stderr, "new mainnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
			}
			*/

			hashGenesisBlock = genesis.GetHash();

			//fprintf(stderr, "hashGenesisBlock:  %s\n", hashGenesisBlock.ToString().c_str());
			//fprintf(stderr, "genesis.hashMerkleRoot:  %s\n", genesis.hashMerkleRoot.ToString().c_str());
			assert(hashGenesisBlock == uint256("0x000053fd966857ec6d481ac1d2566731ae1449487f9de9c2c61b20c887f8d9f8"));
			assert(genesis.hashMerkleRoot == uint256("0x56d98d5bcacf4c3298b7721e11f81b2ff235881f40c803c0aa1604e9dc3ca266"));


			base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,38);
			base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,97);
			base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,83);
			base58Prefixes[STEALTH_ADDRESS] = std::vector<unsigned char>(1,23);
			base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x05)(0x88)(0x00)(0x1E).convert_to_container<std::vector<unsigned char> >();
			base58Prefixes[EXT_SECRET_KEY] = list_of(0x05)(0x88)(0x00)(0xE4).convert_to_container<std::vector<unsigned char> >();


			vSeeds.push_back(CDNSSeedData("0",  "167.99.190.27"));
			vSeeds.push_back(CDNSSeedData("1",  "206.189.42.114"));
			vSeeds.push_back(CDNSSeedData("2", "204.48.24.48"));
			vSeeds.push_back(CDNSSeedData("3", "146.185.160.244"));        
			vSeeds.push_back(CDNSSeedData("4", "206.189.214.9"));
			vSeeds.push_back(CDNSSeedData("5", "206.189.210.125"));
			vSeeds.push_back(CDNSSeedData("6", "206.189.210.93"));



			convertSeeds(vFixedSeeds, pnSeed, ARRAYLEN(pnSeed), nDefaultPort);

			//vFixedSeeds.clear();
			//vSeeds.clear();

			//getHardcodedSeeds(vFixedSeeds);

			nPoolMaxTransactions = 3;
			//strSporkKey = "046f78dcf911fbd61910136f7f0f8d90578f68d0b3ac973b5040fb7afb501b5939f39b108b0569dca71488f5bbf498d92e4d1194f6f941307ffd95f75e76869f0e";
			//strMasternodePaymentsPubKey = "046f78dcf911fbd61910136f7f0f8d90578f68d0b3ac973b5040fb7afb501b5939f39b108b0569dca71488f5bbf498d92e4d1194f6f941307ffd95f75e76869f0e";
			strDarksendPoolDummyAddress = "D8gZqgY4r2RoEdqYk3QsAqFckyf9pRHN6i";


			nLastPOWBlock = 2000;
			nPOSStartBlock = 1999;
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
// Testnet
//

class CTestNetParams : public CMainParams {
	public:
		CTestNetParams() {
			// The message start string is designed to be unlikely to occur in normal data.
			// The characters are rarely used upper ASCII, not valid as UTF-8, and produce
			// a large 4-byte int at any alignment.
			pchMessageStart[0] = 133;
			pchMessageStart[1] = 178;
			pchMessageStart[2] = 234;
			pchMessageStart[3] = 29;
			bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);

			vAlertPubKey = ParseHex("04ff6137a5ef697b866d0bd712abc623f29c7c39ce2eb6a6d55fee3941e5e0b6e2f332cff95e5f9b226c60f7f54404ac7c7f82c054e6d609481a1b59af8dd86c78");
			nDefaultPort = 27170;
			nRPCPort = 27171;
			strDataDir = "testnet";

			/*
			   hashGenesisBlock = uint256("0x01");
			   if (true && genesis.GetHash() != hashGenesisBlock){
			   Logprintf("recalculating params for mainnet.\n");
			   Logprintf("old mainnet genesis nonce: %s\n", genesis.nNonce.ToString().c_str());
			   Logprintf("old mainnet genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
			// deliberately empty for loop finds nonce value.
			for(genesis.nNonce == 0; genesis.GetHash() > bnProofOfWorkLimit; genesis.nNonce++){ } 
			Logprintf("new mainnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
			Logprintf("new mainnet genesis nonce: %s\n", genesis.nNonce.ToString().c_str());
			Logprintf("new mainnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
			}
			*/

			hashGenesisBlock = genesis.GetHash();
			//assert(hashGenesisBlock == uint256("0xcbda0a077461ef37e8f189e5a04a73b3047bcf8779075864ef811fc4a07162b4"));

			vFixedSeeds.clear();
			vSeeds.clear();

			base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,38);
			base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
			base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1,83);
			base58Prefixes[STEALTH_ADDRESS] = std::vector<unsigned char>(1,40);
			base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x05)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
			base58Prefixes[EXT_SECRET_KEY] = list_of(0x05)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

			convertSeeds(vFixedSeeds, pnTestnetSeed, ARRAYLEN(pnTestnetSeed), nDefaultPort);

			nLastPOWBlock = 1000;
		}
		virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


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
		default:
			assert(false && "Unimplemented network");
			return;
	}
}

bool SelectParamsFromCommandLine() {

	bool fTestNet = GetBoolArg("-testnet", false);

	if (fTestNet) {
		SelectParams(CChainParams::TESTNET);
	} else {
		SelectParams(CChainParams::MAIN);
	}
	return true;
}
