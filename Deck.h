#pragma once
#include "Card.h"
#include "DynArray.h"

class Deck {
private:
    DynArray<Card> drawPile;
    DynArray<Card> discardPile;
    void refill();                    // reshuffle discard pile into draw pile

public:
    Deck();
    void reset();                     // build the 108-card deck and shuffle
    bool draw(Card& out);             // false if no card is available at all
    Card startCard();                 // flips the starting card (Wild Draw Four is put back)
    void discard(const Card& c);
    const Card& top() const;
    int drawSize() const { return drawPile.size(); }
};

