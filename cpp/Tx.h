//
// Created by ryousuke kaga on 2024/04/28.
//

#ifndef BITCOIN_TX_H
#define BITCOIN_TX_H

#include <vector>

struct TxIn {
    TxIn();
    std::string toString();
};

struct TxOut {
    TxOut();
    std::string toString();
};

class Tx {
public:
    static const int32_t CURRENT_VERSION = 2;
    Tx();
    std::vector<TxIn> inputs;
    std::vector<TxOut> outputs;
    const int32_t version;
    const uint32_t lockTime;

    std::string toString();

    std::vector<unsigned char> serialize();
    static Tx parse(std::vector<unsigned char> serialization);
};

#endif //BITCOIN_TX_H
