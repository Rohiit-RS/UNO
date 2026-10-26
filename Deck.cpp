#include "Deck.h"
#include "Utils.h"
#include <iostream>
#include <string>

Deck::Deck() { 
    reset(); 
}

void Deck::reset() {
    drawPile.clear();
    discardPile.clear();
    const std::string colors = "RYGB";
    const std::string values = "123456789SRD";
    for (std::size_t c = 0; c < colors.size(); ++c) {
        drawPile.push_back(Card(colors[c], '0'));
        for (std::size_t v = 0; v < values.size(); ++v) {
            drawPile.push_back(Card(colors[c], values[v]));
            drawPile.push_back(Card(colors[c], values[v]));
        }
    }
    for (int i = 0; i < 4; ++i) {
        drawPile.push_back(Card('W', 'W'));
        drawPile.push_back(Card('W', '4'));
    }
    shuffleArray(drawPile);
}

void Deck::refill() {
    if (discardPile.size() <= 1) return;
    Card topCard = discardPile.back();
    discardPile.pop_back();
    while (!discardPile.empty()) {
        drawPile.push_back(discardPile.back());
        discardPile.pop_back();
    }
    shuffleArray(drawPile);
    discardPile.push_back(topCard);
    std::cout << "  (Draw pile empty - discard pile reshuffled)\n";
}

bool Deck::draw(Card& out) {
    if (drawPile.empty()) refill();
    if (drawPile.empty()) return false;
    out = drawPile.back();
    drawPile.pop_back();
    return true;
}

Card Deck::startCard() {
    Card c;
    while (true) {
        draw(c);
        if (c.isWildDrawFour()) {
            drawPile.insert(drawPile.begin() + drawPile.size() / 2, c);
            continue;
        }
        break;
    }
    discardPile.push_back(c);
    return c;
}

void Deck::discard(const Card& c) { 
    discardPile.push_back(c); 
}

const Card& Deck::top() const { 
    return discardPile.back(); 
}
