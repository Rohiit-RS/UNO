#include "Deck.h"
#include "Utils.h"
#include <iostream>

Deck::Deck() { reset(); }

void Deck::reset() {
    drawPile.clear();
    discardPile.clear();
    for (int c = RED; c <= BLUE; ++c) {
        Color col = static_cast<Color>(c);
        drawPile += Card(col, ZERO);                          // one 0
        for (int v = ONE; v <= DRAW_TWO; ++v) {               // two of 1-9, Skip, Reverse, +2
            drawPile += Card(col, static_cast<Value>(v));
            drawPile += Card(col, static_cast<Value>(v));
        }
    }
    for (int i = 0; i < 4; ++i) {
        drawPile += Card(WILD_COLOR, WILD);
        drawPile += Card(WILD_COLOR, WILD_DRAW_FOUR);
    }
    shuffleArray(drawPile);                                   // 108 cards
}

void Deck::refill() {
    if (discardPile.size() <= 1) return;
    Card topCard = discardPile.pop_back();                    // top card stays in place
    while (!discardPile.empty()) drawPile.push_back(discardPile.pop_back());
    shuffleArray(drawPile);
    discardPile.push_back(topCard);
    std::cout << "  (Draw pile empty - discard pile reshuffled)\n";
}

bool Deck::draw(Card& out) {
    if (drawPile.empty()) refill();
    if (drawPile.empty()) return false;
    out = drawPile.pop_back();
    return true;
}

Card Deck::startCard() {
    Card c;
    while (true) {
        draw(c);
        if (c.getValue() == WILD_DRAW_FOUR) {                 // cannot start the game
            drawPile.insertAt(drawPile.size() / 2, c);        // back into the middle
            continue;
        }
        break;
    }
    discardPile.push_back(c);
    return c;
}

void Deck::discard(const Card& c) { discardPile.push_back(c); }

const Card& Deck::top() const { return discardPile[discardPile.size() - 1]; }
