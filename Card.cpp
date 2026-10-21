#include "Card.h"

int Card::cardsCreated = 0;

Card::Card() : color(WILD_COLOR), value(WILD) { ++cardsCreated; }
Card::Card(Color c, Value v) : color(c), value(v) { ++cardsCreated; }

bool Card::isWild() const   { return value == WILD || value == WILD_DRAW_FOUR; }
bool Card::isNumber() const { return value <= NINE; }
bool Card::isAction() const { return value == SKIP || value == REVERSE || value == DRAW_TWO; }

int Card::points() const {
    if (isNumber()) return static_cast<int>(value);
    if (isAction()) return 20;
    return 50;
}

// Legal-match test (colour / number / symbol / wild). The extra Wild Draw Four
// restriction needs the whole hand, so it is checked in Player::canPlay.
bool Card::matches(const Card& top, Color activeColor) const {
    if (isWild()) return true;
    if (color == activeColor) return true;
    return value == top.value;
}

const char* Card::colorName(Color c) {
    static const char* names[] = { "Red", "Yellow", "Green", "Blue", "Wild" };
    return names[c];
}

std::string Card::toString() const {
    static const char* vnames[] = { "0","1","2","3","4","5","6","7","8","9",
                                    "Skip","Reverse","Draw Two","Wild","Wild Draw Four" };
    if (isWild()) return vnames[value];
    return std::string(colorName(color)) + " " + vnames[value];
}

bool Card::operator==(const Card& o) const { return color == o.color && value == o.value; }

bool Card::operator<(const Card& o) const {
    if (color != o.color) return color < o.color;
    return value < o.value;
}

int Card::getCardsCreated() { return cardsCreated; }

std::ostream& operator<<(std::ostream& os, const Card& c) {
    return os << c.toString();
}
