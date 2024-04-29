//
// Created by ryousuke kaga on 2024/04/28.
//

#include "utils.h"

int sha256(unsigned char* message, size_t message_len, unsigned char* digest) {
    EVP_MD_CTX* ctx;

    if((ctx = EVP_MD_CTX_new()) == nullptr)
        return 0;

    if(!EVP_DigestInit(ctx, EVP_sha256()))
        return 0;

    if(!EVP_DigestUpdate(ctx, message, message_len))
        return 0;

    if(!EVP_DigestFinal(ctx, digest, nullptr))
        return 0;

    EVP_MD_CTX_free(ctx);

    return 1;
}

int hash256(unsigned char* message, size_t message_len, unsigned char* digest) {
    sha256(message, message_len, digest);
    return sha256(digest, SHA256_DIGEST_LENGTH, digest);
}

int ripemd160(unsigned char* message, size_t message_len, unsigned char* digest) {
    EVP_MD_CTX* ctx;

    if((ctx = EVP_MD_CTX_new()) == nullptr)
        return -1;

    if(!EVP_DigestInit(ctx, EVP_ripemd160()))
        return -1;

    if(!EVP_DigestUpdate(ctx, message, message_len))
        return -1;

    if(!EVP_DigestFinal(ctx, digest, nullptr))
        return -1;

    EVP_MD_CTX_free(ctx);

    return 0;
}

int hash160(unsigned char* message, size_t message_len, unsigned char* digest) {
    unsigned char out[SHA256_DIGEST_LENGTH];
    sha256(message, message_len, out);
    return ripemd160(out, SHA256_DIGEST_LENGTH, digest);
}

secp256k1_context *create_randomized_context() {
    unsigned char randomize[32];
    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
    if(!fill_random(randomize, sizeof(randomize))) {
        return nullptr;
    }

    if(!secp256k1_context_randomize(ctx, randomize)) {
        return nullptr;
    }

    return ctx;
}
