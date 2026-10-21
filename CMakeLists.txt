cmake_minimum_required(VERSION 3.10)
project(UNO LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    Card.cpp
    Deck.cpp
    Player.cpp
    Game.cpp
)

set(HEADERS
    Card.h
    Deck.h
    DynArray.h
    Game.h
    Player.h
    Utils.h
)

# Main UNO game executable
add_executable(uno main.cpp ${SOURCES} ${HEADERS})

# Unit & edge case test suite
add_executable(uno_tests test_runner.cpp ${SOURCES} ${HEADERS})

enable_testing()
add_test(NAME uno_tests COMMAND uno_tests)

