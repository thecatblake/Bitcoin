//
// Created by ryousuke kaga on 2024/04/28.
//

#include <gtest/gtest.h>
#include "utils.h"
#include "base58.h"
#include "address.h"
#include "Tx.h"


TEST(Serialization, SEC_uncompressed) {
    unsigned char seckey[32] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 234, 219, 238, 241, 35, 69
    };
    unsigned char uncompressed_pubkey[UNCOMPRESSED_SEC_SIZE];
    size_t uncompressed_pubkey_size = sizeof(uncompressed_pubkey);
    secp256k1_pubkey pubkey;
    secp256k1_context* ctx = create_randomized_context();
    ASSERT_TRUE(ctx);

    ASSERT_TRUE(secp256k1_ec_pubkey_create(ctx, &pubkey, seckey));

    secp256k1_ec_pubkey_serialize(ctx, uncompressed_pubkey, &uncompressed_pubkey_size, &pubkey, SECP256K1_EC_UNCOMPRESSED);
    EXPECT_EQ(uncompressed_pubkey_size, sizeof(uncompressed_pubkey));

    EXPECT_EQ(string_hex(uncompressed_pubkey, sizeof(uncompressed_pubkey)), "0x04d90cd625ee87dd38656dd95cf79f65f60f7273b67d3096e68bd81e4f5342691f842efa762fd59961d0e99803c61edba8b3e3f7dc3a341836f97733aebf987121");

    secp256k1_context_destroy(ctx);
}

TEST(Serialization, SEC_compressed) {
    unsigned char seckey[32] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 234, 219, 238, 245, 67, 33
    };
    unsigned char compressed_pubkey[COMPRESSED_SEC_SIZE];
    size_t compressed_pubkey_size = sizeof(compressed_pubkey);
    secp256k1_pubkey pubkey;
    secp256k1_context* ctx = create_randomized_context();
    ASSERT_TRUE(ctx);

    ASSERT_TRUE(secp256k1_ec_pubkey_create(ctx, &pubkey, seckey));

    secp256k1_ec_pubkey_serialize(ctx, compressed_pubkey, &compressed_pubkey_size, &pubkey, SECP256K1_EC_COMPRESSED);
    EXPECT_EQ(compressed_pubkey_size, sizeof(compressed_pubkey));

    EXPECT_EQ(string_hex(compressed_pubkey, sizeof(compressed_pubkey)), "0x0296be5b1292f6c856b3c5654e886fc13511462059089cdf9c479623bfcbe77690");

    secp256k1_context_destroy(ctx);
}

TEST(Serialization, DER) {
    secp256k1_context* ctx = create_randomized_context();
    ASSERT_TRUE(ctx);

    unsigned char sig_raw[64] = {
            198, 167, 71, 32, 40, 29, 232, 230, 232, 6, 140, 182, 141, 151, 196, 244, 122, 184, 103, 151, 203, 153, 73, 7, 88, 92, 153, 16, 6, 106, 32, 55,
            236, 218, 140, 115, 226, 200, 65, 200, 101, 250, 208, 230, 248, 91, 167, 201, 159, 17, 202, 236, 60, 208, 192, 174, 190, 126, 21, 193, 89, 55, 166, 140
    };
    secp256k1_ecdsa_signature sig;
    memcpy(sig.data, sig_raw, sizeof(sig_raw));

    unsigned char der[71];
    size_t der_len = sizeof(der);

    secp256k1_ecdsa_signature_serialize_der(ctx, der, &der_len, &sig);

    EXPECT_EQ(string_hex(der, sizeof(der)), "0x3045022037206a0610995c58074999cb9767b87af4c4978db68c06e8e6e81d282047a7c60221008ca63759c1157ebeaec0d03cecca119fc9a75bf8e6d0fa65c841c8e2738cdaec");

    secp256k1_context_destroy(ctx);
}

TEST(Serialization, Base58) {
    std::vector<unsigned char> h = {
            199, 32, 127, 238, 25, 125, 39, 198, 24, 174, 166, 33, 64, 111, 107, 245, 239, 111, 202, 56, 104, 29, 130, 178, 240, 111, 221, 189, 206, 111, 234, 182
    };
    std::string s = EncodeBase58(h);
    EXPECT_EQ(s, "EQJsjkd6JaGwxrjEhfeqPenqHwrBmPQZjJGNSCHBkcF7");
}

TEST(Serialization, address_uncompressed_testnet) {
    unsigned char seckey[32] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 138
    };
    secp256k1_pubkey pubkey;
    secp256k1_context* ctx = create_randomized_context();
    ASSERT_TRUE(ctx);
    ASSERT_TRUE(secp256k1_ec_pubkey_create(ctx, &pubkey, seckey));

    std::string address = GetAddress(ctx, &pubkey, false, true);
    EXPECT_EQ(address, "mmTPbXQFxboEtNRkwfh6K51jvdtHLxGeMA");

    secp256k1_context_destroy(ctx);
}

TEST(Serialization, address_compressed_testnet) {
    unsigned char seckey[32] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 119, 124, 107, 22, 33, 100, 0
    };
    secp256k1_pubkey pubkey;
    secp256k1_context* ctx = create_randomized_context();
    ASSERT_TRUE(ctx);
    ASSERT_TRUE(secp256k1_ec_pubkey_create(ctx, &pubkey, seckey));

    std::string address = GetAddress(ctx, &pubkey, true, true);
    EXPECT_EQ(address, "mopVkxp8UhXqRYbCYJsbeE1h1fiF64jcoH");

    secp256k1_context_destroy(ctx);
}


TEST(Serialization, address_compressed_mainnet) {
    unsigned char seckey[32] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 35, 69, 222, 173, 190, 239
    };
    secp256k1_pubkey pubkey;
    secp256k1_context* ctx = create_randomized_context();
    ASSERT_TRUE(ctx);
    ASSERT_TRUE(secp256k1_ec_pubkey_create(ctx, &pubkey, seckey));

    std::string address = GetAddress(ctx, &pubkey, true, false);
    EXPECT_EQ(address, "1F1Pn2y6pDb68E5nYJJeba4TLg2U7B6KF1");

    secp256k1_context_destroy(ctx);
}

TEST(Utils, varint) {
    uint256 i;
    i.SetHex("0x10603eedc76d");
    std::vector<unsigned char> encoded = encode_varint(i);
    std::vector<unsigned char> encoded_t = {
            255, 109, 199, 237, 62, 96, 16, 0, 0
    };
    for(auto j=0; j < encoded.size(); j++) {
        ASSERT_EQ(encoded[j], encoded_t[j]);
    }
    auto decoded = decode_varint(&encoded[0]);
    EXPECT_EQ(i, decoded);

    i.SetHex("0x64");
    encoded = encode_varint(i);
    encoded_t = {
            0x64
    };
    for(auto j=0; j < encoded.size(); j++) {
        ASSERT_EQ(encoded[j], encoded_t[j]);
    }
    decoded = decode_varint(&encoded[0]);
    EXPECT_EQ(i, decoded);

    i.SetHex("0xff");
    encoded = encode_varint(i);
    encoded_t = {
            0xfd, 0xff, 0x00
    };
    for(auto j=0; j < encoded.size(); j++) {
        ASSERT_EQ(encoded[j], encoded_t[j]);
    }
    decoded = decode_varint(&encoded[0]);
    EXPECT_EQ(i, decoded);

    i.SetHex("0x22b");
    encoded = encode_varint(i);
    encoded_t = {
            0xfd, 0x2b, 0x02
    };
    for(auto j=0; j < encoded.size(); j++) {
        ASSERT_EQ(encoded[j], encoded_t[j]);
    }
    decoded = decode_varint(&encoded[0]);
    EXPECT_EQ(i, decoded);

    i.SetHex("0x1117f");
    encoded = encode_varint(i);
    encoded_t = {
            0xfe, 0x7f, 0x11, 0x01, 0x00
    };
    for(auto j=0; j < encoded.size(); j++) {
        ASSERT_EQ(encoded[j], encoded_t[j]);
    }
    decoded = decode_varint(&encoded[0]);
    EXPECT_EQ(i, decoded);
}


TEST(Transaction, parse) {
    std::vector<unsigned char> tx_raw = {
            02, 00, 00, 00
    };
    auto tx = Tx::parse(tx_raw);

    EXPECT_EQ(tx.version, 2);
}