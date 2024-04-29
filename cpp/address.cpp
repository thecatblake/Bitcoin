//
// Created by ryousuke kaga on 2024/04/28.
//

#include "address.h"
#include "base58.h"

std::string GetAddress(secp256k1_context *ctx, secp256k1_pubkey *pubkey, bool compressed, bool testnet) {
    unsigned char sec[compressed ? COMPRESSED_SEC_SIZE : UNCOMPRESSED_SEC_SIZE];
    size_t len = sizeof(sec);
    secp256k1_ec_pubkey_serialize(ctx, sec, &len, pubkey, compressed ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED);
    unsigned char hash[20 + 1];
    hash[0] = testnet ? 0x6f : 0x00;
    hash160(sec, len, hash+1);
    return EncodeBase58Check(hash);
}
