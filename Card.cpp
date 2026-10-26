#include "Card.h"

int Card::cardsCreated = 0;

Card::Card() {
    color = 'W';
    value = 'W';
    cardsCreated++;
}

Card::Card(Color c, Value v) {
    color = c;
    value = v;
    cardsCreated++;
}

bool Card::isWild() const         { return color == 'W'; }
bool Card::isWildDrawFour() const { return color == 'W' && value == '4'; }
bool Card::isNumber() const       { return color != 'W' && value >= '0' && value <= '9'; }
bool Card::isAction() const       { return value == 'S' || value == 'R' || value == 'D'; }

bool Card::matches(const Card& top, Color activeColor) const {
    if (isWild()) return true;
    if (color == activeColor) return true;
    return !top.isWild() && value == top.value;
}

const char* Card::colorName(Color c) {
    switch (c) {
    case 'R': return "Red";
    case 'Y': return "Yellow";
    case 'G': return "Green";
    case 'B': return "Blue";
    default:  return "Wild";
    }
}

std::string Card::toString() const {
    if (isWildDrawFour()) return "Wild Draw Four";
    if (isWild())         return "Wild";
    std::string v;
    switch (value) {
    case 'S': v = "Skip";     break;
    case 'R': v = "Reverse";  break;
    case 'D': v = "Draw Two"; break;
    default:  v = std::string(1, value); break;
    }
    return std::string(colorName(color)) + " " + v;
}

bool Card::operator==(const Card& o) const { 
    return color == o.color && value == o.value; 
}

int Card::colorIndex(Color c) {
    switch (c) {
    case 'R': return 0;
    case 'Y': return 1;
    case 'G': return 2;
    case 'B': return 3;
    default:  return 4;
    }
}

int Card::valueRank(const Card& c) {
    if (c.isWildDrawFour()) return 14;
    switch (c.value) {
    case 'S': return 10;
    case 'R': return 11;
    case 'D': return 12;
    case 'W': return 13;
    default:  return c.value - '0';
    }
}

bool Card::operator<(const Card& o) const {
    if (color != o.color) return colorIndex(color) < colorIndex(o.color);
    return valueRank(*this) < valueRank(o);
}

int Card::getCardsCreated() { 
    return cardsCreated; 
}

std::ostream& operator<<(std::ostream& os, const Card& c) {
    return os << c.toString();
}
