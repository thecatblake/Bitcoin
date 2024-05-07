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

TxIn TxIn::parse(unsigned char* serialization, long long *len) {
    TxIn txin;
    long long i = 0;
    std::reverse_copy(serialization+i, serialization+32, txin.prev_tx);
    i += 32;
    memcpy((unsigned char*)&txin.prev_index, serialization+i, sizeof(txin.prev_index));
    i += sizeof(txin.prev_index);
    int var_len;
    auto _script_size = decode_varint(serialization+i, &var_len);
    long long script_size;
    memcpy((unsigned char*)&script_size, _script_size.data(), sizeof(script_size));
    i += var_len;
    txin.script_raw.resize(script_size);
    std::copy(serialization+i, serialization+i+script_size, txin.script_raw.begin());
    i += script_size;
    memcpy((unsigned char*)&txin.nSequence, serialization+i, sizeof(txin.nSequence));
    i += sizeof(txin.nSequence);
    *len = i;
    return txin;
}

TxOut::TxOut() {

}

std::string TxOut::toString() {
    return std::string();
}

TxOut TxOut::parse(unsigned char *serialization, long long *len) {
    TxOut txout;
    long long i = 0;
    memcpy((unsigned char*)&txout.amount, serialization+i, sizeof(txout.amount));
    i += sizeof(txout.amount);
    int var_len;
    auto _script_size = decode_varint(serialization+i, &var_len);
    long long script_size;
    memcpy((unsigned char*)&script_size, _script_size.data(), sizeof(script_size));
    i += var_len;
    txout.script_raw.resize(script_size);
    std::copy(serialization+i, serialization+i+script_size, txout.script_raw.begin());
    i += script_size;
    *len = i;
    return txout;
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
    memcpy((unsigned char*)&n, num_inp.data(), sizeof(n));
    tx.inputs.resize(n);
    for(auto j=0; j < n;j++) {
        long long txin_len;
        TxIn txin = TxIn::parse(&serialization[i], &txin_len);
        tx.inputs[j] = txin;
        i += txin_len;
    }
    num_inp = decode_varint(&serialization[i], &var_len);
    i += var_len;
    memcpy((unsigned char*)&n, num_inp.data(), sizeof(n));
    tx.outputs.resize(n);
    for(auto j=0;j < n; j++) {
        long long txout_len;
        TxOut txout = TxOut::parse(&serialization[i], &txout_len);
        tx.outputs[j] = txout;
        i += txout_len;
    }
    memcpy((unsigned char*)&tx.lockTime, &serialization[i], sizeof(tx.lockTime));
    return tx;
}

uint64_t transaction::GetOutAmount() {
    uint64_t amount = 0;
    for(const auto& output: outputs) {
        amount += output.amount;
    }
    return amount;
}
