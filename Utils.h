#pragma once
#include <iostream>
#include <string>
#include <random>
#include <cstdlib>

// How to hide the previous player's hand:
//   0 = system command (cls / clear)            -> real Command Prompt / Terminal
//   1 = print 100 blank lines                   -> works everywhere (default, fine for CLion)
//   2 = ANSI escape codes that wipe the screen  -> CLion with "Emulate terminal in output
//                                                  console" ticked, or CLion's Terminal tab
const int CLEAR_MODE = 1;

inline std::mt19937& rng() {
    static std::mt19937 gen(std::random_device{}());
    return gen;
}

// random integer in [0, n-1]
inline int randInt(int n) { return std::uniform_int_distribution<int>(0, n - 1)(rng()); }

inline void exitOnEOF() {
    if (std::cin.eof()) { std::cout << "\nInput closed. Exiting.\n"; std::exit(0); }
}

inline int readInt(const std::string& prompt, int lo, int hi) {
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v) {
            std::cin.ignore(10000, '\n');
            if (v >= lo && v <= hi) return v;
        } else {
            exitOnEOF();
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        std::cout << "  Please enter a number from " << lo << " to " << hi << ".\n";
    }
}

inline bool readYesNo(const std::string& prompt) {
    std::string s;
    while (true) {
        std::cout << prompt;
        if (std::cin >> s) {
            std::cin.ignore(10000, '\n');
            if (s[0] == 'y' || s[0] == 'Y') return true;
            if (s[0] == 'n' || s[0] == 'N') return false;
        } else {
            exitOnEOF();
            std::cin.clear();
        }
        std::cout << "  Please answer y or n.\n";
    }
}
