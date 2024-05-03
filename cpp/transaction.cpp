//
// Created by ryousuke kaga on 2024/04/28.
//

#include "transaction.h"
#include "utils.h"
#include <format>

TxIn::TxIn() {

}

std::string TxIn::toString() {
    return std::string();
}

TxIn TxIn::parse(unsigned char* serialization, int* len) {
    TxIn txin;
    size_t i = 0;
    std::reverse_copy(serialization+i, serialization+32, txin.prev_tx);
    i += 32;
    memcpy((unsigned char*)&txin.prev_index, serialization+i, sizeof(txin.prev_index));
    i += sizeof(txin.prev_index);
    int var_len;
    return txin;
}

TxOut::TxOut() {

}

std::string TxOut::toString() {
    return std::string();
}

transaction::transaction(): version(1), lockTime(0) {

}

std::string transaction::toString() {
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

std::vector<unsigned char> transaction::serialize() {
    return std::vector<unsigned char>();
}

transaction transaction::parse(std::vector<unsigned char> serialization) {
    transaction tx;
    size_t i = 0;
    memcpy((unsigned char*)&tx.version, &serialization[i], 4);
    i += 4;
    int var_len;
    auto num_inp = decode_varint(&serialization[i], &var_len);
    i += var_len;
    long long n;
    memcpy((unsigned char*)&n, num_inp.data(), sizeof(long long));
    tx.inputs.resize(n);
    return tx;
}
