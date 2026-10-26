#pragma once
#include <vector>
#include "Card.h"

class Deck {
private:
    std::vector<Card> drawPile;
    std::vector<Card> discardPile;
    void refill();

public:
    Deck();
    void reset();
    bool draw(Card& out);
    Card startCard();
    void discard(const Card& c);
    const Card& top() const;
    int drawSize() const { return static_cast<int>(drawPile.size()); }
};
