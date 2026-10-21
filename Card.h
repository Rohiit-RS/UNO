#pragma once
#include <string>
#include <iostream>

enum Color { RED, YELLOW, GREEN, BLUE, WILD_COLOR };
enum Value { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
             SKIP, REVERSE, DRAW_TWO, WILD, WILD_DRAW_FOUR };

class Card {
private:
    Color color;
    Value value;
    static int cardsCreated;          // static data member

public:
    Card();
    Card(Color c, Value v);

    // inline accessors (const member functions)
    Color getColor() const { return color; }
    Value getValue() const { return value; }

    bool isWild() const;
    bool isNumber() const;
    bool isAction() const;
    int  points() const;
    bool matches(const Card& top, Color activeColor) const;
    std::string toString() const;

    bool operator==(const Card& o) const;
    bool operator<(const Card& o) const;      // used for sorting a hand

    static const char* colorName(Color c);
    static int getCardsCreated();             // static member function

    friend std::ostream& operator<<(std::ostream& os, const Card& c);
};

