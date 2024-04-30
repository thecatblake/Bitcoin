//
// Created by ryousuke kaga on 2024/04/28.
//

#include "Tx.h"
#include <format>

TxIn::TxIn() {

}

std::string TxIn::toString() {
    return std::string();
}

TxOut::TxOut() {

}

std::string TxOut::toString() {
    return std::string();
}

Tx::Tx(): version(1), lockTime(0) {

}

std::string Tx::toString() {
    std::string tx_ins;
    for (auto& input : inputs) {
        tx_ins += input.toString() + "\n";
    }
    std::string tx_outs;
    for(auto& output : outputs) {
        tx_outs += output.toString() + "\n";
    }
    return std::format("tx: \nversion: {}\ntx_ins:\n{}tx_outs:\n{}locktime: {}", version, tx_ins, tx_outs, lockTime);
}

std::vector<unsigned char> Tx::serialize() {
    return std::vector<unsigned char>();
}

Tx Tx::parse(std::vector<unsigned char> serialization) {
    Tx tx;
    memcpy((unsigned char*)&tx.version, &serialization[0], 4);
    return tx;
}
