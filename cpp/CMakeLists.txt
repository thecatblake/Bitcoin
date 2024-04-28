cmake_minimum_required(VERSION 3.28)
project(bitcoin)

set(CMAKE_CXX_STANDARD 20)

include_directories(/opt/homebrew/include/)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_library(bitcoin utils.cpp)
target_link_libraries(bitcoin secp256k1 crypto ssl)

add_executable(bitcoin_test main.cpp)
target_link_libraries(bitcoin_test bitcoin)
