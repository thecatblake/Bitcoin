//
// Created by ryousuke kaga on 2024/05/07.
//

#include "script.h"
#include "stream.h"
#include <utility>


 bool op_dup(std::stack<uint256>& stack) {
    if (stack.empty()) return false;
    stack.push(stack.top());
    return true;
}

Script::Script(std::stack<std::deque<unsigned char>> cmds): cmds(std::move(cmds)) {

}

Script Script::parse(std::deque<unsigned char> raw_script) {
    std::stack<std::deque<unsigned char>> stack;
    Stream stream(raw_script);
    int n = stream.read(1)[0];
}
