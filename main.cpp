#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Game.h"

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::cout << "=====================================\n"
              << "            C++  U N O\n"
              << "=====================================\n"
              << "First player to empty their hand wins (sudden death).\n\n";
    Game game;
    game.run();
    return 0;
}
