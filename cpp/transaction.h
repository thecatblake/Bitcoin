//
// Created by ryousuke kaga on 2024/04/28.
//

#ifndef BITCOIN_TRANSACTION_H
#define BITCOIN_TRANSACTION_H

#include <vector>

struct TxIn {
    unsigned char prev_tx[32];
    uint32_t prev_index;
    std::vector<unsigned char> script_raw;
    uint32_t nSequence;

    TxIn();
    std::string toString();
    static TxIn parse(unsigned char* serialization, long long* len);
};

struct TxOut {
    int64_t amount;
    std::vector<unsigned char> script_raw;

    TxOut();
    std::string toString();
    static TxOut parse(unsigned char* serialization, long long* len);
};

class transaction {
public:
    static const int32_t CURRENT_VERSION = 2;
    transaction();
    std::vector<TxIn> inputs;
    std::vector<TxOut> outputs;
    const int32_t version;
    const uint32_t lockTime;

    std::string toString();

    std::vector<unsigned char> serialize();
    static transaction parse(std::vector<unsigned char> serialization);
    uint64_t GetOutAmount();
};

#endif //BITCOIN_TRANSACTION_H
