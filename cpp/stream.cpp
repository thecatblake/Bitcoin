//
// Created by ryousuke kaga on 2024/05/30.
//

#include "stream.h"

#include <utility>

std::deque<unsigned char> Stream::read(int nBytes) {
    auto slice = std::deque<unsigned char>(data.begin(), data.begin() + nBytes);
    for(int i=0;i<nBytes;i++) data.pop_front();
    return slice;
}

void Stream::write(std::deque<unsigned char> bytes) {
    data.insert(data.end(), bytes.begin(), bytes.end());
}

int Stream::readInt() {
    auto slice = std::deque<unsigned char>(data.begin(), data.begin()+sizeof(int));
    for(int i=0;i<sizeof(int);i++) data.pop_front();
    int x;
    memcpy(&x, &slice[0], sizeof(int));
    return x;
}

void Stream::writeInt(int n) {
    data.insert(data.end(), static_cast<unsigned char*>(static_cast<void*>(&n)), static_cast<unsigned char*>(static_cast<void*>(&n)) + sizeof(n));
}

uint256 Stream::readUint256() {
    auto slice = std::deque<unsigned char>(data.begin(), data.begin()+32);
    for(int i=0;i<32;i++) data.pop_front();
    uint256 x;
    memcpy(x.data(), &slice[0], sizeof(x));
    return x;
}

void Stream::writeUint256(uint256 n) {
    data.insert(data.end(), n.data(), n.data()+32);
}

Stream::Stream(std::deque<unsigned char> data): data(std::move(data)) {

}
