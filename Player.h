#pragma once
#include <string>
#include <iostream>
#include "Card.h"
#include "DynArray.h"

class Game;

class Player {
private:
    std::string name;
    int score;
    DynArray<Card> hand;

public:
    Player();
    explicit Player(const std::string& n);

    const std::string& getName() const { return name; }
    int  getScore() const { return score; }
    int  handSize() const { return hand.size(); }
    const Card& cardAt(int i) const { return hand[i]; }

    // Method cascading: these return *this by reference
    Player& addCard(const Card& c);
    Player& sortHand();
    Player& clearHand();
    Player& addScore(int pts);

    Card playCard(int i);                           // removes and returns card i
    bool hasColor(Color c) const;
    bool canPlay(int i, const Card& top, Color active) const;   // includes Wild Draw Four rule
    bool hasPlayable(const Card& top, Color active) const;
    int  handPoints() const;
    void showHand(const Card* top = nullptr, Color active = WILD_COLOR) const;

    Player& operator+=(const Card& c) { return addCard(c); }
    bool operator<(const Player& o) const { return score < o.score; }
    bool operator>(const Player& o) const { return score > o.score; }

    friend std::ostream& operator<<(std::ostream& os, const Player& p);
    friend std::istream& operator>>(std::istream& is, Player& p);   // reads the name
    friend class Game;
};

