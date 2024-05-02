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
    int len;
    i.SetHex("0x10603eedc76d");
    std::vector<unsigned char> encoded = encode_varint(i);
    std::vector<unsigned char> encoded_t = {
            255, 109, 199, 237, 62, 96, 16, 0, 0
    };
    for(auto j=0; j < encoded.size(); j++) {
        ASSERT_EQ(encoded[j], encoded_t[j]);
    }
    auto decoded = decode_varint(&encoded[0], &len);
    EXPECT_EQ(i, decoded);

    i.SetHex("0x64");
    encoded = encode_varint(i);
    encoded_t = {
            0x64
    };
    for(auto j=0; j < encoded.size(); j++) {
        ASSERT_EQ(encoded[j], encoded_t[j]);
    }
    decoded = decode_varint(&encoded[0], &len);
    EXPECT_EQ(i, decoded);

    i.SetHex("0xff");
    encoded = encode_varint(i);
    encoded_t = {
            0xfd, 0xff, 0x00
    };
    for(auto j=0; j < encoded.size(); j++) {
        ASSERT_EQ(encoded[j], encoded_t[j]);
    }
    decoded = decode_varint(&encoded[0], &len);
    EXPECT_EQ(i, decoded);

    i.SetHex("0x22b");
    encoded = encode_varint(i);
    encoded_t = {
            0xfd, 0x2b, 0x02
    };
    for(auto j=0; j < encoded.size(); j++) {
        ASSERT_EQ(encoded[j], encoded_t[j]);
    }
    decoded = decode_varint(&encoded[0], &len);
    EXPECT_EQ(i, decoded);

    i.SetHex("0x1117f");
    encoded = encode_varint(i);
    encoded_t = {
            0xfe, 0x7f, 0x11, 0x01, 0x00
    };
    for(auto j=0; j < encoded.size(); j++) {
        ASSERT_EQ(encoded[j], encoded_t[j]);
    }
    decoded = decode_varint(&encoded[0], &len);
    EXPECT_EQ(i, decoded);
}


TEST(Transaction, parse) {
    std::vector<unsigned char> tx_raw = {
            2, 0, 0, 0, 4, 86, 145, 153, 96, 172, 105, 23, 99, 104, 141, 61, 59, 206, 169, 173, 110, 202, 248, 117, 223, 83, 57, 225, 72, 161, 252, 97, 198, 237, 122, 6, 158, 1, 0, 0, 0, 106, 71, 48, 68, 2, 32, 69, 133, 188, 222, 248, 94, 107, 28, 106, 245, 194, 102, 157, 72, 48, 255, 134, 228, 45, 210, 5, 192, 224, 137, 188, 42, 130, 22, 87, 233, 81, 192, 2, 32, 16, 36, 161, 3, 102, 7, 127, 135, 214, 188, 225, 247, 16, 10, 216, 207, 168, 160, 100, 179, 157, 78, 143, 228, 234, 19, 167, 183, 26, 168, 24, 15, 1, 33, 2, 240, 218, 87, 232, 94, 236, 41, 52, 168, 42, 88, 94, 163, 55, 206, 47, 73, 152, 181, 10, 230, 153, 221, 121, 245, 136, 14, 37, 61, 175, 175, 183, 254, 255, 255, 255, 235, 143, 81, 244, 3, 141, 193, 126, 99, 19, 207, 131, 29, 79, 2, 40, 28, 42, 70, 139, 222, 15, 175, 211, 127, 27, 248, 130, 114, 158, 127, 211, 0, 0, 0, 0, 106, 71, 48, 68, 2, 32, 120, 153, 83, 26, 82, 213, 154, 109, 226, 0, 23, 153, 40, 202, 144, 2, 84, 163, 107, 141, 255, 139, 183, 95, 95, 93, 113, 177, 205, 194, 97, 37, 2, 32, 8, 180, 34, 105, 11, 132, 97, 203, 82, 195, 204, 48, 51, 11, 35, 213, 116, 53, 24, 114, 183, 195, 97, 233, 170, 227, 100, 144, 113, 193, 167, 22, 1, 33, 3, 93, 92, 147, 217, 172, 150, 136, 31, 25, 186, 31, 104, 111, 21, 240, 9, 222, 215, 198, 46, 254, 133, 168, 114, 230, 161, 155, 67, 193, 90, 41, 55, 254, 255, 255, 255, 86, 123, 244, 5, 149, 17, 157, 27, 184, 163, 3, 124, 53, 110, 253, 86, 23, 11, 100, 203, 204, 22, 15, 176, 40, 250, 16, 112, 75, 69, 215, 117, 0, 0, 0, 0, 106, 71, 48, 68, 2, 32, 76, 124, 120, 24, 66, 76, 127, 121, 17, 218, 108, 221, 197, 150, 85, 167, 10, 241, 203, 94, 175, 23, 198, 157, 173, 191, 199, 79, 250, 11, 102, 47, 2, 32, 117, 153, 224, 139, 200, 2, 54, 147, 173, 78, 149, 39, 220, 66, 195, 66, 16, 247, 167, 209, 209, 221, 252, 132, 146, 182, 84, 161, 30, 118, 32, 160, 1, 33, 2, 21, 139, 70, 251, 223, 246, 93, 1, 114, 183, 152, 154, 236, 136, 80, 170, 13, 174, 73, 171, 251, 132, 200, 26, 230, 229, 178, 81, 165, 138, 206, 92, 254, 255, 255, 255, 214, 58, 94, 108, 22, 230, 32, 248, 111, 55, 89, 37, 178, 28, 171, 175, 115, 108, 119, 159, 136, 253, 4, 220, 173, 81, 210, 102, 144, 247, 243, 69, 1, 0, 0, 0, 106, 71, 48, 68, 2, 32, 6, 51, 234, 13, 51, 20, 190, 160, 217, 91, 60, 216, 218, 219, 46, 247, 158, 168, 51, 31, 254, 30, 97, 247, 98, 192, 246, 218, 234, 15, 171, 222, 2, 32, 41, 242, 59, 62, 156, 48, 240, 128, 68, 97, 80, 178, 56, 82, 2, 135, 81, 99, 93, 206, 226, 190, 102, 156, 42, 22, 134, 164, 181, 237, 243, 4, 1, 33, 3, 255, 214, 244, 166, 126, 148, 171, 163, 83, 160, 8, 130, 229, 99, 255, 39, 34, 235, 76, 255, 10, 214, 0, 110, 134, 238, 32, 223, 231, 82, 13, 85, 254, 255, 255, 255, 2, 81, 67, 15, 0, 0, 0, 0, 0, 25, 118, 169, 20, 171, 12, 11, 46, 152, 177, 171, 109, 191, 103, 212, 117, 11, 10, 86, 36, 73, 72, 168, 121, 136, 172, 0, 90, 98, 2, 0, 0, 0, 0, 25, 118, 169, 20, 60, 130, 215, 223, 54, 78, 182, 199, 91, 232, 200, 13, 242, 179, 237, 168, 219, 87, 57, 112, 136, 172, 70, 67, 6, 0
    };
    auto tx = Tx::parse(tx_raw);

    EXPECT_EQ(tx.version, 2);
    EXPECT_EQ(tx.inputs.size(), 4);
}