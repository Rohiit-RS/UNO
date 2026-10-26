#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Card.h"

class Game;

class Player {
private:
    std::string name;
    std::vector<Card> hand;

public:
    Player();
    explicit Player(const std::string& n);

    const std::string& getName() const { return name; }
    int  handSize() const { return static_cast<int>(hand.size()); }
    const Card& cardAt(int i) const { return hand[i]; }

    Player& addCard(const Card& c);
    Player& sortHand();
    Player& clearHand();

    Card playCard(int i);
    bool hasColor(Color c) const;
    bool canPlay(int i, const Card& top, Color active) const;
    bool hasPlayable(const Card& top, Color active) const;
    void showHand(const Card* top = 0, Color active = 'W') const;

    Player& operator+=(const Card& c) { return addCard(c); }
    bool operator<(const Player& o) const { return hand.size() < o.hand.size(); }
    bool operator>(const Player& o) const { return hand.size() > o.hand.size(); }

    friend std::ostream& operator<<(std::ostream& os, const Player& p);
    friend std::istream& operator>>(std::istream& is, Player& p);
    friend class Game;
};
