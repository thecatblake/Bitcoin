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

uint256 decode_varint(unsigned char *s) {
    uint256 i;
    switch (s[0]) {
        case 0xfd:
            memcpy((unsigned char*)&i, s+1, 2);
            break;
        case 0xfe:
            memcpy((unsigned char*)&i, s+1, 4);
            break;
        case 0xff:
            memcpy((unsigned char*)&i, s+1, 8);
            break;
        default:
            memcpy((unsigned char*)&i, s, 1);
            break;
    }

    return i;
}

std::vector<unsigned char> encode_varint(uint256 i) {
    std::vector<unsigned char> s;
    unsigned char* data = i.data();
    if (data[4] > 0) {
        s.resize(8+1);
        s[0] = 0xff;
        memcpy(&s[1], data, 8);
    } else if (data[2] > 0) {
        s.resize(4+1);
        s[0] = 0xfe;
        memcpy(&s[1], data, 4);
    } else if (data[0] >= 0xfd || data[1] > 0) {
        s.resize(2+1);
        s[0] = 0xfd;
        memcpy(&s[1], data, 2);
    } else {
        s.resize(1);
        memcpy(&s[0], data, 1);
    }
    return s;
}
