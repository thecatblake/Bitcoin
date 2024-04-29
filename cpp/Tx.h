//
// Created by ryousuke kaga on 2024/04/28.
//

#ifndef BITCOIN_TX_H
#define BITCOIN_TX_H

#include <vector>

struct TxIn {
    TxIn();
};

struct TxOut {
    TxOut();
};

class Tx {
public:
    static const int32_t CURRENT_VERSION = 2;
    Tx();
    std::vector<TxIn> inputs;
    std::vector<TxOut> outputs;
    const int32_t version;
    const uint32_t lockTime;
};

#endif //BITCOIN_TX_H
