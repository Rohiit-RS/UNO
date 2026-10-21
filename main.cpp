#include <iostream>
#include "Game.h"

int main() {
    std::cout << "=====================================\n"
              << "            C++  U N O\n"
              << "=====================================\n"
              << "First player to 500 points wins.\n\n";
    Game game(500);
    game.run();
    return 0;
}

