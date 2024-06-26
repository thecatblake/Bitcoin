//
// Created by ryousuke kaga on 2024/04/29.
//

#include "utils.h"
#include "address.h"
#include <secp256k1.h>

int main(int argc, char** argv) {
    unsigned char hash[32];
    auto* passphrase = (unsigned char*)argv[1];
    sha256(passphrase, strlen(argv[1]), hash);

    auto* ctx = create_randomized_context();
    secp256k1_pubkey pubkey;
    if(!secp256k1_ec_pubkey_create(ctx, &pubkey, hash)) {
        std::cout << "Failed to generate public key" << std::endl;
        return 1;
    }

    unsigned char pubkey_compressed[33];
    size_t len = sizeof(pubkey_compressed);
    secp256k1_ec_pubkey_serialize(ctx, pubkey_compressed, &len, &pubkey, SECP256K1_EC_COMPRESSED);

    std::cout << "Secret key: " << string_hex(hash, 32) << std::endl;
    std::cout << "Compressed Public key: " << string_hex(pubkey_compressed, len) << std::endl;
    std::cout << "Address: " << GetAddress(ctx, &pubkey, true, false) << std::endl;
    return 0;
}