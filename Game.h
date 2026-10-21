#pragma once
#include <string>
#include "DynArray.h"
#include "Card.h"
#include "Deck.h"
#include "Player.h"

// Hot-seat Uno: every player is a real person sharing one device.
class Game {
private:
    DynArray<Player> players;
    Deck* deck;                       // heap object -> Rule of Three needed
    int current;                      // index of player whose turn it is
    int dir;                          // +1 clockwise, -1 counter-clockwise
    int dealer;
    int targetScore;
    Color activeColor;
    std::string events;               // public events shown to the next player
    static int roundsPlayed;          // static data member

    int   nextIndex(int from, int steps = 1) const;
    void  say(const std::string& msg);              // print + remember for next player
    void  clearScreen() const;
    void  setup();
    void  playRound();
    void  handleStartCard(const Card& start);
    int   takeTurn();                               // returns winner index, or -1
    void  showTable() const;
    int   chooseCard(const Player& p) const;        // card index, or -1 = draw
    Color chooseColor(const Player& p) const;
    int   applyCard(int idx, const Card& c, Color prevActive);   // returns extra players skipped
    void  drawCards(int idx, int n);
    void  unoPhase(int idx);
    void  endRound(int winner);

public:
    explicit Game(int target = 500);
    Game(const Game& o);
    ~Game();
    Game& operator=(const Game& o);

    void run();
    static int getRoundsPlayed() { return roundsPlayed; }
};
