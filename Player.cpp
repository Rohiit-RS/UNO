#include "Player.h"
#include "Utils.h"
#include <iostream>

Player::Player() {
    name = "Player";
}

Player::Player(const std::string& n) {
    name = n;
}

Player& Player::addCard(const Card& c) { 
    hand.push_back(c); 
    return *this; 
}

Player& Player::clearHand() { 
    hand.clear(); 
    return *this; 
}

Player& Player::sortHand() {
    for (int i = 1; i < handSize(); ++i)
        for (int j = i; j > 0 && hand[j] < hand[j - 1]; --j)
            swapValues(hand[j], hand[j - 1]);
    return *this;
}

Card Player::playCard(int i) {
    Card c = hand[i];
    hand.erase(hand.begin() + i);
    return c;
}

bool Player::hasColor(Color c) const {
    for (int i = 0; i < handSize(); ++i)
        if (hand[i].getColor() == c) return true;
    return false;
}

bool Player::canPlay(int i, const Card& top, Color active) const {
    return hand[i].matches(top, active);
}

bool Player::hasPlayable(const Card& top, Color active) const {
    for (int i = 0; i < handSize(); ++i)
        if (canPlay(i, top, active)) return true;
    return false;
}

void Player::showHand(const Card* top, Color active) const {
    for (int i = 0; i < handSize(); ++i) {
        std::cout << "  " << (i + 1) << ") " << hand[i];
        if (top && canPlay(i, *top, active)) std::cout << "   <- playable";
        std::cout << "\n";
    }
}

std::ostream& operator<<(std::ostream& os, const Player& p) {
    return os << p.name << " (" << p.hand.size() << " cards left)";
}

std::istream& operator>>(std::istream& is, Player& p) {
    return is >> p.name;
}
