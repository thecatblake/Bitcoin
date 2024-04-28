#include <iostream>
#include "utils.h"
#include <secp256k1.h>

int main() {
    unsigned char hash[32];
    unsigned char seckey[32] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 57
    };
    unsigned char randomize[32];
    unsigned char key[] = "Programming Bitcoin!";
    secp256k1_pubkey pubkey;
    secp256k1_ecdsa_signature sig;

    hash256(key, sizeof(key), hash);

    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
    if(!fill_random(randomize, sizeof(randomize))) {
        printf("Failed to generate randomness\n");
        return 1;
    }

    if(!secp256k1_context_randomize(ctx, randomize)) {
        printf("Failed to randomize the context\n");
        return 1;
    }

    if(!secp256k1_ec_seckey_verify(ctx, seckey)) {
        printf("Failed to verify the secret key");
        return 1;
    }

    if(!secp256k1_ec_pubkey_create(ctx, &pubkey, seckey)) {
        printf("Failed to create a pubkey");
        return 1;
    }

    if(!secp256k1_ecdsa_sign(ctx, &sig, hash, seckey, nullptr, nullptr)) {
        printf("Failed to sign the message");
        return 1;
    }

    bool valid = secp256k1_ecdsa_verify(ctx, &sig, hash, &pubkey);
    printf("Signature valid: %s\n", valid ? "true" : "false");
    printf("Signature: ");
    print_hex(sig.data, sizeof(sig.data));

    secp256k1_context_destroy(ctx);

    secure_erase(seckey, sizeof(seckey));

    return 0;
}
