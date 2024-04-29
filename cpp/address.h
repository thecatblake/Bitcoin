//
// Created by ryousuke kaga on 2024/04/28.
//

#ifndef BITCOIN_ADDRESS_H
#define BITCOIN_ADDRESS_H

#include "utils.h"

std::string GetAddress(secp256k1_context* ctx, secp256k1_pubkey* pubkey, bool compressed=true, bool testnet=false);

#endif //BITCOIN_ADDRESS_H
