#pragma once
#include <vector>
#include <string>
#include "Card.h"
#include "Deck.h"
#include "Player.h"

class Game {
private:
    std::vector<Player> players;
    Deck* deck;
    int current;
    int dir;
    int dealer;
    Color activeColor;
    std::string events;
    static int gamesPlayed;

    int   nextIndex(int from, int steps = 1) const;
    void  say(const std::string& msg);
    void  clearScreen() const;
    void  setup();
    void  playGame();
    void  handleStartCard(const Card& start);
    int   takeTurn();
    void  showTable() const;
    int   chooseCard(const Player& p) const;
    Color chooseColor(const Player& p) const;
    int   applyCard(int idx, const Card& c, Color prevActive);
    void  drawCards(int idx, int n);
    void  unoPhase(int idx);
    void  endGame(int winner);

public:
    Game();
    Game(const Game& o);
    ~Game();
    Game& operator=(const Game& o);

    void run();
    static int getGamesPlayed() { return gamesPlayed; }
};
