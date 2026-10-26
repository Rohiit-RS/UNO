#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <utility>

// ===== Utils.h =====
// How to hide the previous player's hand:
//   0 = system command (cls / clear)
//   1 = print 100 blank lines (default, fine for CLion)
//   2 = ANSI escape codes that wipe the screen
const int CLEAR_MODE = 1;

inline int randInt(int n) {
    return std::rand() % n;
}

inline int readInt(const std::string& prompt, int lo, int hi) {
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v) {
            std::cin.ignore(10000, '\n');
            if (v >= lo && v <= hi) return v;
        } else {
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
            std::cin.clear();
        }
        std::cout << "  Please answer y or n.\n";
    }
}

// ===== Function templates =====
template <typename T>
void swapValues(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

template <typename T>
void shuffleArray(std::vector<T>& arr) {
    for (std::size_t i = arr.size(); i > 1; --i) {
        std::size_t j = std::rand() % i;
        std::swap(arr[i - 1], arr[j]);
    }
}
