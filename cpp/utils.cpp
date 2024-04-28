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