// ============================================================
//  C++ UNO  (hot-seat, real players)  -  single-file version
//  Compile:  g++ -std=c++11 -o uno Uno.cpp      Run: ./uno
//  Give each input on its own line (number, then each name, ...).
// ============================================================
#include<iostream>
#include<string>
#include<random>
#include<cstdlib>
#include<stdexcept>

// ===== Utils.h =====
// Small input / random helpers shared by all files.

// How to hide the previous player's hand:
//   0 = system command (cls / clear)            -> real Command Prompt / Terminal
//   1 = print 100 blank lines                   -> works everywhere (default, fine for CLion)
//   2 = ANSI escape codes that wipe the screen  -> CLion with "Emulate terminal in output
//                                                  console" ticked, or CLion's Terminal tab
const int CLEAR_MODE = 1;

inline std::mt19937& rng() {
    static std::mt19937 gen(std::random_device{}());
    return gen;
}
// random integer in [0, n-1]
inline int randInt(int n) { return std::uniform_int_distribution<int>(0, n - 1)(rng()); }

inline void exitOnEOF() {
    if (std::cin.eof()) { std::cout << "\nInput closed. Exiting.\n"; std::exit(0); }
}

inline int readInt(const std::string& prompt, int lo, int hi) {
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v) {
            std::cin.ignore(10000, '\n');
            if (v >= lo && v <= hi) return v;
        } else {
            exitOnEOF();
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        std::cout << "  Please enter a number from " << lo << " to " << hi << ".\n";
    }
}

inline bool readYesNo(const std::string& prompt) {
    std::string s;
    while (true) {
        std::cout << prompt;
        if (std::cin >> s) {
            std::cin.ignore(10000, '\n');
            if (s[0] == 'y' || s[0] == 'Y') return true;
            if (s[0] == 'n' || s[0] == 'N') return false;
        } else {
            exitOnEOF();
            std::cin.clear();
        }
        std::cout << "  Please answer y or n.\n";
    }
}

// ===== DynArray.h =====
// Class template: custom dynamic container on the heap (new[] / delete[]).
// Implements the Rule of Three (destructor, copy ctor, copy assignment).

// Function template
template <typename T>
void swapValues(T& a, T& b) { T tmp = a; a = b; b = tmp; }

template <typename T>
class DynArray {
private:
    T* data;
    int sz;
    int cap;

    void grow() {
        int newCap = cap * 2;
        T* nd = new T[newCap];
        for (int i = 0; i < sz; ++i) nd[i] = data[i];
        delete[] data;
        data = nd;
        cap = newCap;
    }

public:
    explicit DynArray(int c = 8) : data(new T[c > 0 ? c : 1]), sz(0), cap(c > 0 ? c : 1) {}

    // Copy constructor (deep copy)
    DynArray(const DynArray& o) : data(new T[o.cap]), sz(o.sz), cap(o.cap) {
        for (int i = 0; i < sz; ++i) data[i] = o.data[i];
    }

    // Destructor
    ~DynArray() { delete[] data; }

    // Copy assignment (deep copy, self-assignment safe)
    DynArray& operator=(const DynArray& o) {
        if (this != &o) {
            T* nd = new T[o.cap];
            for (int i = 0; i < o.sz; ++i) nd[i] = o.data[i];
            delete[] data;
            data = nd;
            sz = o.sz;
            cap = o.cap;
        }
        return *this;
    }

    void push_back(const T& v) {
        if (sz == cap) grow();
        data[sz++] = v;
    }
    DynArray& operator+=(const T& v) { push_back(v); return *this; }

    T pop_back() {
        if (sz == 0) throw std::out_of_range("DynArray::pop_back on empty array");
        return data[--sz];
    }

    void insertAt(int i, const T& v) {
        if (i < 0 || i > sz) throw std::out_of_range("DynArray::insertAt");
        if (sz == cap) grow();
        for (int k = sz; k > i; --k) data[k] = data[k - 1];
        data[i] = v;
        ++sz;
    }

    T removeAt(int i) {
        if (i < 0 || i >= sz) throw std::out_of_range("DynArray::removeAt");
        T v = data[i];
        for (int k = i; k < sz - 1; ++k) data[k] = data[k + 1];
        --sz;
        return v;
    }

    T& operator[](int i) {
        if (i < 0 || i >= sz) throw std::out_of_range("DynArray index");
        return data[i];
    }
    const T& operator[](int i) const {
        if (i < 0 || i >= sz) throw std::out_of_range("DynArray index");
        return data[i];
    }

    int size() const { return sz; }
    bool empty() const { return sz == 0; }
    void clear() { sz = 0; }
};

// Function template: Fisher-Yates shuffle
template <typename T>
void shuffleArray(DynArray<T>& arr) {
    for (int i = arr.size() - 1; i > 0; --i)
        swapValues(arr[i], arr[randInt(i + 1)]);
}

// ===== Card.h =====

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

// ===== Player.h =====

class Player {
private:
    std::string name;
    int score;
    DynArray<Card> hand;

public:
    Player();
    explicit Player(const std::string& n);

    const std::string& getName() const { return name; }
    int  getScore() const { return score; }
    int  handSize() const { return hand.size(); }
    const Card& cardAt(int i) const { return hand[i]; }

    // Method cascading: these return *this by reference
    Player& addCard(const Card& c);
    Player& sortHand();
    Player& clearHand();
    Player& addScore(int pts);

    Card playCard(int i);                           // removes and returns card i
    bool hasColor(Color c) const;
    bool canPlay(int i, const Card& top, Color active) const;   // includes Wild Draw Four rule
    bool hasPlayable(const Card& top, Color active) const;
    int  handPoints() const;
    void showHand(const Card* top = 0, Color active = WILD_COLOR) const;

    Player& operator+=(const Card& c) { return addCard(c); }
    bool operator<(const Player& o) const { return score < o.score; }
    bool operator>(const Player& o) const { return score > o.score; }

    friend std::ostream& operator<<(std::ostream& os, const Player& p);
    friend std::istream& operator>>(std::istream& is, Player& p);   // reads the name
    friend class Game;
};

// ===== Deck.h =====

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

// ===== Game.h =====

// Hot-seat Uno: every player is a real person sharing one device.
class Game {
private:
    DynArray<Player> players;
    Deck* deck;                       // heap object -> Rule of Three needed
    int current;                      // index of player whose turn it is
    int dir;                          // +1 clockwise, -1 counter-clockwise
    int dealer;
    int targetScore;
    Color activeColor;
    std::string events;               // public events shown to the next player
    static int roundsPlayed;          // static data member

    int   nextIndex(int from, int steps = 1) const;
    void  say(const std::string& msg);              // print + remember for next player
    void  clearScreen() const;
    void  setup();
    void  playRound();
    void  handleStartCard(const Card& start);
    int   takeTurn();                               // returns winner index, or -1
    void  showTable() const;
    int   chooseCard(const Player& p) const;        // card index, or -1 = draw
    Color chooseColor(const Player& p) const;
    int   applyCard(int idx, const Card& c, Color prevActive);   // returns extra players skipped
    void  drawCards(int idx, int n);
    void  unoPhase(int idx);
    void  endRound(int winner);

public:
    explicit Game(int target = 500);
    Game(const Game& o);
    ~Game();
    Game& operator=(const Game& o);

    void run();
    static int getRoundsPlayed() { return roundsPlayed; }
};

// ===== Card.cpp =====

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

// ===== Deck.cpp =====

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

// ===== Player.cpp =====

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

// ===== Game.cpp =====

int Game::roundsPlayed = 0;

// ---------- Rule of Three ----------
Game::Game(int target)
    : deck(new Deck()), current(0), dir(1), dealer(0), targetScore(target), activeColor(RED) {}

Game::Game(const Game& o)
    : players(o.players), deck(new Deck(*o.deck)), current(o.current), dir(o.dir),
      dealer(o.dealer), targetScore(o.targetScore), activeColor(o.activeColor),
      events(o.events) {}

Game::~Game() { delete deck; }

Game& Game::operator=(const Game& o) {
    if (this != &o) {
        Deck* nd = new Deck(*o.deck);
        delete deck;
        deck = nd;
        players = o.players;
        current = o.current; dir = o.dir; dealer = o.dealer;
        targetScore = o.targetScore; activeColor = o.activeColor;
        events = o.events;
    }
    return *this;
}

// ---------- helpers ----------
int Game::nextIndex(int from, int steps) const {
    int n = players.size();
    return (((from + dir * steps) % n) + n) % n;
}

void Game::say(const std::string& msg) {
    std::cout << msg;
    events += msg;
}

void Game::clearScreen() const {
    std::cout << std::flush;
    if (CLEAR_MODE == 0) {
#ifdef _WIN32
        std::system("cls");
#else
        std::cout << "\033[3J" << std::flush;     // also wipe the scroll-back
        if (std::system("clear") != 0) {}
#endif
    } else if (CLEAR_MODE == 2) {
        std::cout << "\033[2J\033[3J\033[H" << std::flush;
    } else {
        for (int i = 0; i < 100; ++i) std::cout << "\n";
    }
}

void Game::drawCards(int idx, int n) {
    int got = 0;
    for (int i = 0; i < n; ++i) {
        Card c;
        if (deck->draw(c)) { players[idx].addCard(c); ++got; }
    }
    say("  " + players[idx].getName() + " draws " + std::to_string(got) + " card(s).\n");
}

void Game::showTable() const {
    std::cout << "\nTop card: " << deck->top() << "   |   Active colour: "
              << Card::colorName(activeColor) << "   |   Direction: "
              << (dir == 1 ? "clockwise" : "counter-clockwise") << "\n";
    std::cout << "Cards in hand: ";
    for (int i = 0; i < players.size(); ++i)
        std::cout << (i == current ? "[" : "") << players[i].getName() << ":"
                  << players[i].handSize() << (i == current ? "]" : "") << "  ";
    std::cout << "\n";
}

// ---------- setup ----------
void Game::setup() {
    int n = readInt("Number of players (2-10): ", 2, 10);
    for (int i = 0; i < n; ++i) {
        Player p;
        std::cout << "Name of player " << (i + 1) << " (no spaces): ";
        if (!(std::cin >> p)) exitOnEOF();
        std::cin.ignore(10000, '\n');
        players += p;                                   // operator+= on DynArray
    }
}

// ---------- choices ----------
int Game::chooseCard(const Player& p) const {
    const Card& top = deck->top();
    std::cout << "\n" << p.getName() << ", your hand:\n";
    p.showHand(&top, activeColor);
    while (true) {
        int v = readInt("Enter card number to play (0 = draw a card): ", 0, p.handSize());
        if (v == 0) return -1;
        if (p.canPlay(v - 1, top, activeColor)) return v - 1;
        if (p.cardAt(v - 1).getValue() == WILD_DRAW_FOUR)
            std::cout << "  Illegal: Wild Draw Four only if you have no "
                      << Card::colorName(activeColor) << " card.\n";
        else
            std::cout << "  That card doesn't match the top card. Try again.\n";
    }
}

Color Game::chooseColor(const Player& p) const {
    int v = readInt(p.getName() + ", choose colour: 1) Red  2) Yellow  3) Green  4) Blue : ", 1, 4);
    return static_cast<Color>(v - 1);
}

// ---------- card effects ----------
int Game::applyCard(int idx, const Card& c, Color prevActive) {
    deck->discard(c);
    if (!c.isWild()) activeColor = c.getColor();
    int n = players.size();
    int skips = 0;

    switch (c.getValue()) {
    case SKIP:
        say("  " + players[nextIndex(idx)].getName() + " is skipped!\n");
        skips = 1;
        break;
    case REVERSE:
        if (n == 2) { say("  Reverse (acts as Skip with 2 players)!\n"); skips = 1; }
        else { dir = -dir; say("  Direction reversed!\n"); }
        break;
    case DRAW_TWO: {
        int t = nextIndex(idx);
        drawCards(t, 2);
        say("  " + players[t].getName() + " loses their turn.\n");
        skips = 1;
        break;
    }
    case WILD:
        activeColor = chooseColor(players[idx]);
        say("  " + players[idx].getName() + " chooses " + Card::colorName(activeColor) + ".\n");
        break;
    case WILD_DRAW_FOUR: {
        activeColor = chooseColor(players[idx]);
        say("  " + players[idx].getName() + " chooses " + Card::colorName(activeColor) + ".\n");
        int t = nextIndex(idx);
        // hide the player's hand and hand the device to the targeted player
        clearScreen();
        std::cout << "Pass the device to " << players[t].getName() << " and press Enter when ready...";
        if (std::cin.get() == EOF) exitOnEOF();
        std::cout << "\n" << players[idx].getName() << " played Wild Draw Four and chose "
                  << Card::colorName(activeColor) << ".\n"
                  << "Cards in " << players[idx].getName() << "'s hand: " << players[idx].handSize() << "\n";
        bool challenge = readYesNo(players[t].getName() +
                                   ", do you challenge the Wild Draw Four? (y/n): ");
        if (!challenge) {
            drawCards(t, 4);
            skips = 1;
        } else {
            say("  " + players[t].getName() + " challenges! " + players[idx].getName() +
                " must reveal their hand:\n");
            players[idx].showHand();
            if (players[idx].hasColor(prevActive)) {
                say("  Illegal play! " + players[idx].getName() + " draws 4 instead.\n");
                drawCards(idx, 4);              // challenger takes their turn normally
            } else {
                say("  Legal play - challenge fails. " + players[t].getName() + " draws 6.\n");
                drawCards(t, 6);
                skips = 1;
            }
        }
        break;
    }
    default:
        break;
    }
    return skips;
}

// ---------- UNO shout ----------
void Game::unoPhase(int idx) {
    Player& p = players[idx];
    if (readYesNo("Table check: " + p.getName() + " has 1 card left. Did " + p.getName() + " say UNO in time? (y/n): ")) {
        say("  " + p.getName() + " shouts \"UNO!\"\n");
        return;
    }
    std::cout << "  " << p.getName() << " forgot to say UNO. Did anyone catch them before the next turn?\n";
    for (int i = 0; i < players.size(); ++i)
        if (i != idx) std::cout << "    " << (i + 1) << ") " << players[i].getName() << "\n";
    int who = readInt("  Enter the catcher's number (0 = nobody noticed): ", 0, players.size());
    if (who == 0 || who - 1 == idx) {
        say("  (Nobody noticed " + p.getName() + " forgot to say UNO.)\n");
        return;
    }
    say("  " + players[who - 1].getName() + " caught " + p.getName() + "! Penalty: 2 cards.\n");
    drawCards(idx, 2);
}

// ---------- one turn ----------
int Game::takeTurn() {
    Player& p = players[current];
    Color prev = activeColor;
    Card top = deck->top();

    // hide the previous player's hand, then hand over the device
    clearScreen();
    std::cout << "Pass the device to " << p.getName() << " and press Enter when ready...";
    if (std::cin.get() == EOF) exitOnEOF();
    std::cout << "\n=== " << p.getName() << "'s turn ===\n";
    if (!events.empty()) std::cout << "Since your last turn:\n" << events;
    events.clear();
    showTable();

    p.sortHand();
    int choice = chooseCard(p);

    Card played;
    bool didPlay = false;
    if (choice >= 0) {
        played = p.playCard(choice);
        didPlay = true;
    } else {
        Card drawn;
        if (deck->draw(drawn)) {
            p.addCard(drawn);
            int idx = p.handSize() - 1;                     // the card just drawn
            std::cout << "You drew: " << drawn << "\n";
            if (p.canPlay(idx, top, prev) &&
                readYesNo("Play it now? (y/n): ")) {
                played = p.playCard(idx);
                didPlay = true;
            }
            if (!didPlay) say(p.getName() + " draws a card and passes.\n");
        } else {
            say(p.getName() + " cannot draw (no cards left) and passes.\n");
        }
    }

    int skips = 0;
    if (didPlay) {
        say(p.getName() + " plays " + played.toString() + ".\n");
        skips = applyCard(current, played, prev);
        if (p.handSize() == 0) return current;              // round over (penalties already applied)
        if (p.handSize() == 1) unoPhase(current);
    }
    std::cout << "\nPress Enter to hide the screen...";
    if (std::cin.get() == EOF) exitOnEOF();
    current = nextIndex(current, 1 + skips);
    return -1;
}

// ---------- start card ----------
void Game::handleStartCard(const Card& start) {
    int left = nextIndex(dealer);
    activeColor = start.getColor();
    say("Starting card: " + start.toString() + "\n");
    switch (start.getValue()) {
    case DRAW_TWO:
        say("  " + players[left].getName() + " draws 2 and forfeits the first turn.\n");
        drawCards(left, 2);
        current = nextIndex(left);
        break;
    case REVERSE:
        dir = -1;
        current = dealer;
        say("  Direction is counter-clockwise; dealer " + players[dealer].getName() + " starts.\n");
        break;
    case SKIP:
        say("  " + players[left].getName() + " is skipped.\n");
        current = nextIndex(dealer, 2);
        break;
    case WILD:
        activeColor = chooseColor(players[left]);
        say("  " + players[left].getName() + " declares " + Card::colorName(activeColor) + ".\n");
        current = left;
        break;
    default:
        current = left;
    }
}

// ---------- round / game ----------
void Game::endRound(int winner) {
    int total = 0;
    for (int i = 0; i < players.size(); ++i)
        if (i != winner) total += players[i].handPoints();
    players[winner].addScore(total);
    ++roundsPlayed;
    std::cout << "\n=== " << players[winner].getName() << " wins the round and scores "
              << total << " points! ===\nScoreboard:\n";
    for (int i = 0; i < players.size(); ++i) std::cout << "  " << players[i] << "\n";
}

void Game::playRound() {
    deck->reset();
    int n = players.size();
    for (int i = 0; i < n; ++i) players[i].clearHand();
    dealer = roundsPlayed % n;
    dir = 1;
    events.clear();

    for (int r = 0; r < 7; ++r)
        for (int i = 0; i < n; ++i) {
            Card c;
            deck->draw(c);
            players[nextIndex(dealer, i + 1)] += c;
        }

    std::cout << "\n############ ROUND " << (roundsPlayed + 1) << " (dealer: "
              << players[dealer].getName() << ") ############\n";
    handleStartCard(deck->startCard());

    int winner = -1;
    while (winner < 0) winner = takeTurn();
    endRound(winner);
}

void Game::run() {
    setup();
    while (true) {
        playRound();
        int best = 0;
        for (int i = 1; i < players.size(); ++i)
            if (players[i] > players[best]) best = i;
        if (players[best].getScore() >= targetScore) {
            std::cout << "\n*** " << players[best].getName() << " reaches "
                      << players[best].getScore() << " points and WINS THE GAME after "
                      << roundsPlayed << " round(s)! ***\n";
            break;
        }
        std::cout << "\n(First to " << targetScore << " points wins.)\n";
        std::cout << "Press Enter to start the next round...";
        if (std::cin.get() == EOF) exitOnEOF();
    }
    std::cout << "Total Card objects constructed: " << Card::getCardsCreated() << "\n";
}

// ===== main.cpp =====

int main() {
    std::cout << "=====================================\n"
              << "            C++  U N O\n"
              << "=====================================\n"
              << "First player to 500 points wins.\n\n";
    Game game(500);
    game.run();
    return 0;
}
