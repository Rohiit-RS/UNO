#include "Player.h"
#include "Utils.h"
#include <iostream>

Player::Player() : name("Player"), score(0) {}
Player::Player(const std::string& n) : name(n), score(0) {}

Player& Player::addCard(const Card& c) { hand.push_back(c); return *this; }
Player& Player::clearHand()            { hand.clear();      return *this; }
Player& Player::addScore(int pts)      { score += pts;      return *this; }

Player& Player::sortHand() {               // insertion sort using Card::operator<
    for (int i = 1; i < hand.size(); ++i)
        for (int j = i; j > 0 && hand[j] < hand[j - 1]; --j)
            swapValues(hand[j], hand[j - 1]);
    return *this;
}

Card Player::playCard(int i) { return hand.removeAt(i); }

bool Player::hasColor(Color c) const {
    for (int i = 0; i < hand.size(); ++i)
        if (hand[i].getColor() == c) return true;
    return false;
}

bool Player::canPlay(int i, const Card& top, Color active) const {
    const Card& c = hand[i];
    if (c.getValue() == WILD_DRAW_FOUR) return !hasColor(active);   // only if no card of active colour
    return c.matches(top, active);
}

bool Player::hasPlayable(const Card& top, Color active) const {
    for (int i = 0; i < hand.size(); ++i)
        if (canPlay(i, top, active)) return true;
    return false;
}

int Player::handPoints() const {
    int total = 0;
    for (int i = 0; i < hand.size(); ++i) total += hand[i].points();
    return total;
}

void Player::showHand(const Card* top, Color active) const {
    for (int i = 0; i < hand.size(); ++i) {
        std::cout << "  " << (i + 1) << ") " << hand[i];
        if (top && canPlay(i, *top, active)) std::cout << "   <- playable";
        std::cout << "\n";
    }
}

std::ostream& operator<<(std::ostream& os, const Player& p) {
    return os << p.name << " (" << p.hand.size() << " cards, " << p.score << " pts)";
}

std::istream& operator>>(std::istream& is, Player& p) {
    return is >> p.name;
}

