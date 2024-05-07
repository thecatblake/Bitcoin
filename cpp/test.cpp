//
// Created by ryousuke kaga on 2024/04/28.
//

#include <gtest/gtest.h>
#include "utils.h"
#include "base58.h"
#include "address.h"
#include "Transaction.h"


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
            1, 0, 0, 0, 1, 129, 63, 121, 1, 26, 203, 128, 146, 93, 254, 105, 179, 222, 243, 85, 254, 145, 75, 209, 217, 106, 63, 95, 113, 191, 131, 3, 198, 169, 137, 199, 209, 0, 0, 0, 0, 107, 72, 48, 69, 2, 33, 0, 237, 129, 255, 25, 46, 117, 163, 253, 35, 4, 0, 77, 202, 219, 116, 111, 165, 226, 76, 80, 49, 204, 252, 242, 19, 32, 176, 39, 116, 87, 201, 143, 2, 32, 122, 152, 109, 149, 92, 110, 12, 179, 93, 68, 106, 137, 211, 245, 97, 0, 244, 215, 246, 120, 1, 195, 25, 103, 116, 58, 156, 142, 16, 97, 91, 237, 1, 33, 3, 73, 252, 78, 99, 30, 54, 36, 165, 69, 222, 63, 137, 245, 216, 104, 76, 123, 129, 56, 189, 148, 189, 213, 49, 210, 226, 19, 191, 1, 107, 39, 138, 254, 255, 255, 255, 2, 161, 53, 239, 1, 0, 0, 0, 0, 25, 118, 169, 20, 188, 59, 101, 77, 202, 126, 86, 176, 77, 202, 24, 242, 86, 108, 218, 240, 46, 141, 154, 218, 136, 172, 153, 195, 152, 0, 0, 0, 0, 0, 25, 118, 169, 20, 28, 75, 199, 98, 221, 84, 35, 227, 50, 22, 103, 2, 203, 117, 244, 13, 247, 159, 234, 18, 136, 172, 25, 67, 6, 0
    };
    Transaction tx = Transaction::parse(tx_raw);

    EXPECT_EQ(tx.version, 1);
    EXPECT_EQ(tx.inputs.size(), 1);

    unsigned char prev_tx[] = {
            209, 199, 137, 169, 198, 3, 131, 191, 113, 95, 63, 106, 217, 209, 75, 145, 254, 85, 243, 222, 179, 105, 254, 93, 146, 128, 203, 26, 1, 121, 63, 129
    };
    for (int i=0; i < sizeof(prev_tx); i++) {
        ASSERT_EQ(tx.inputs[0].prev_tx[i], prev_tx[i]);
    }
    EXPECT_EQ(tx.inputs[0].prev_index, 0);
    EXPECT_EQ(tx.inputs[0].nSequence, 0xfffffffe);
    EXPECT_EQ(tx.outputs.size(), 2);
    EXPECT_EQ(tx.outputs[0].amount, 32454049);
    EXPECT_EQ(tx.outputs[1].amount, 10011545);
    EXPECT_EQ(tx.lockTime, 410393);
}

TEST(Transaction, out_amount) {
    std::vector<unsigned char> tx_raw = {
            1, 0, 0, 0, 1, 129, 63, 121, 1, 26, 203, 128, 146, 93, 254, 105, 179, 222, 243, 85, 254, 145, 75, 209, 217, 106, 63, 95, 113, 191, 131, 3, 198, 169, 137, 199, 209, 0, 0, 0, 0, 107, 72, 48, 69, 2, 33, 0, 237, 129, 255, 25, 46, 117, 163, 253, 35, 4, 0, 77, 202, 219, 116, 111, 165, 226, 76, 80, 49, 204, 252, 242, 19, 32, 176, 39, 116, 87, 201, 143, 2, 32, 122, 152, 109, 149, 92, 110, 12, 179, 93, 68, 106, 137, 211, 245, 97, 0, 244, 215, 246, 120, 1, 195, 25, 103, 116, 58, 156, 142, 16, 97, 91, 237, 1, 33, 3, 73, 252, 78, 99, 30, 54, 36, 165, 69, 222, 63, 137, 245, 216, 104, 76, 123, 129, 56, 189, 148, 189, 213, 49, 210, 226, 19, 191, 1, 107, 39, 138, 254, 255, 255, 255, 2, 161, 53, 239, 1, 0, 0, 0, 0, 25, 118, 169, 20, 188, 59, 101, 77, 202, 126, 86, 176, 77, 202, 24, 242, 86, 108, 218, 240, 46, 141, 154, 218, 136, 172, 153, 195, 152, 0, 0, 0, 0, 0, 25, 118, 169, 20, 28, 75, 199, 98, 221, 84, 35, 227, 50, 22, 103, 2, 203, 117, 244, 13, 247, 159, 234, 18, 136, 172, 25, 67, 6, 0
    };
    Transaction tx = Transaction::parse(tx_raw);

    EXPECT_EQ(tx.GetOutAmount(), 42465594);
}