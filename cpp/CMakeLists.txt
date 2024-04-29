cmake_minimum_required(VERSION 3.28)
project(bitcoin)

set(CMAKE_CXX_STANDARD 20)

include_directories(/opt/homebrew/include/)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(bitcoin utils.cpp
        base58.cpp
        base58.h
        uint256.cpp
        uint256.h
        strencodings.cpp
        strencodings.h
        span.h
        address.cpp
        address.h
        Tx.cpp
        Tx.h)
target_link_libraries(bitcoin secp256k1 crypto ssl)

add_executable(test test.cpp)
target_link_libraries(test bitcoin GTest::gtest_main)

add_executable(genAddress genAddress.cpp)
target_link_libraries(genAddress bitcoin)

include(GoogleTest)
gtest_discover_tests(test)

