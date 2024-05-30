//
// Created by ryousuke kaga on 2024/05/30.
//

#ifndef BITCOIN_STREAM_H
#define BITCOIN_STREAM_H

#include <deque>
#include "uint256.h"

class Stream {
    std::deque<unsigned char> data;
public:
    Stream(std::deque<unsigned char> data);
    std::deque<unsigned char> read(int nBytes);
    void write(std::deque<unsigned char> bytes);
    int readInt();
    void writeInt(int n);
    uint256 readUint256();
    void writeUint256(uint256 n);
};

#endif //BITCOIN_STREAM_H
