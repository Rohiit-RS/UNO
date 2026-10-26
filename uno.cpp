// ============================================================
//  C++ UNO  (hot-seat, real players, first empty hand wins)
//  Compile:  g++ -std=c++11 -o uno Uno.cpp      Run: ./uno
//  Give each input on its own line (number, then each name, ...).
//
//  Card codes (no enums):
//    Color: 'R' Red, 'Y' Yellow, 'G' Green, 'B' Blue, 'W' Wild
//    Value: '0'-'9', 'S' Skip, 'R' Reverse, 'D' Draw Two,
//           'W' Wild, '4' Wild Draw Four (only when color == 'W')
// ============================================================
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <utility>

// ===== Utils.h =====
// How to hide the previous player's hand:
//   0 = system command (cls / clear)
//   1 = print 100 blank lines (default, fine for CLion)
//   2 = ANSI escape codes that wipe the screen
const int CLEAR_MODE = 1;

inline int randInt(int n) {
    return std::rand() % n;
}

inline int readInt(const std::string& prompt, int lo, int hi) {
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v) {
            std::cin.ignore(10000, '\n');
            if (v >= lo && v <= hi) return v;
        } else {
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
            std::cin.clear();
        }
        std::cout << "  Please answer y or n.\n";
    }
}

// ===== Function templates =====
template <typename T>
void swapValues(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

template <typename T>
void shuffleArray(std::vector<T>& arr) {
    for (std::size_t i = arr.size(); i > 1; --i) {
        std::size_t j = std::rand() % i;
        std::swap(arr[i - 1], arr[j]);
    }
}

// ===== Card.h =====

typedef char Color;   // 'R','Y','G','B','W'
typedef char Value;   // '0'-'9','S','R','D','W','4'

class Card {
private:
    Color color;
    Value value;
    static int cardsCreated;

    static int colorIndex(Color c);
    static int valueRank(const Card& c);

public:
    Card();
    Card(Color c, Value v);

    Color getColor() const { return color; }
    Value getValue() const { return value; }

    bool isWild() const;
    bool isWildDrawFour() const;
    bool isNumber() const;
    bool isAction() const;
    bool matches(const Card& top, Color activeColor) const;
    std::string toString() const;

    bool operator==(const Card& o) const;
    bool operator<(const Card& o) const;

    static const char* colorName(Color c);
    static int getCardsCreated();

    friend std::ostream& operator<<(std::ostream& os, const Card& c);
};

// ===== Player.h =====

class Player {
private:
    std::string name;
    std::vector<Card> hand;

public:
    Player();
    explicit Player(const std::string& n);

    const std::string& getName() const { return name; }
    int  handSize() const { return static_cast<int>(hand.size()); }
    const Card& cardAt(int i) const { return hand[i]; }

    Player& addCard(const Card& c);
    Player& sortHand();
    Player& clearHand();

    Card playCard(int i);
    bool hasColor(Color c) const;
    bool canPlay(int i, const Card& top, Color active) const;
    bool hasPlayable(const Card& top, Color active) const;
    void showHand(const Card* top = 0, Color active = 'W') const;

    Player& operator+=(const Card& c) { return addCard(c); }
    bool operator<(const Player& o) const { return hand.size() < o.hand.size(); }
    bool operator>(const Player& o) const { return hand.size() > o.hand.size(); }

    friend std::ostream& operator<<(std::ostream& os, const Player& p);
    friend std::istream& operator>>(std::istream& is, Player& p);
    friend class Game;
};

// ===== Deck.h =====

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

// ===== Game.h =====

class Game {
private:
    std::vector<Player> players;
    Deck* deck;
    int current;
    int dir;
    int dealer;
    Color activeColor;
    std::string events;
    static int gamesPlayed;

    int   nextIndex(int from, int steps = 1) const;
    void  say(const std::string& msg);
    void  clearScreen() const;
    void  setup();
    void  playGame();
    void  handleStartCard(const Card& start);
    int   takeTurn();
    void  showTable() const;
    int   chooseCard(const Player& p) const;
    Color chooseColor(const Player& p) const;
    int   applyCard(int idx, const Card& c, Color prevActive);
    void  drawCards(int idx, int n);
    void  unoPhase(int idx);
    void  endGame(int winner);

public:
    Game();
    Game(const Game& o);
    ~Game();
    Game& operator=(const Game& o);

    void run();
    static int getGamesPlayed() { return gamesPlayed; }
};

// ===== Card.cpp =====

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

// ===== Deck.cpp =====

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

// ===== Player.cpp =====

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

// ===== Game.cpp =====

int Game::gamesPlayed = 0;

Game::Game() {
    deck = new Deck();
    current = 0;
    dir = 1;
    dealer = 0;
    activeColor = 'R';
}

Game::Game(const Game& o) {
    players = o.players;
    deck = new Deck(*o.deck);
    current = o.current;
    dir = o.dir;
    dealer = o.dealer;
    activeColor = o.activeColor;
    events = o.events;
}

Game::~Game() { 
    delete deck; 
}

Game& Game::operator=(const Game& o) {
    if (this != &o) {
        Deck* nd = new Deck(*o.deck);
        delete deck;
        deck = nd;
        players = o.players;
        current = o.current; 
        dir = o.dir; 
        dealer = o.dealer;
        activeColor = o.activeColor;
        events = o.events;
    }
    return *this;
}

// ---------- helpers ----------
int Game::nextIndex(int from, int steps) const {
    int n = static_cast<int>(players.size());
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
        std::cout << "\033[3J" << std::flush;
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
        if (deck->draw(c)) { 
            players[idx].addCard(c); 
            ++got; 
        }
    }
    say("  " + players[idx].getName() + " draws " + std::to_string(got) + " card(s).\n");
}

void Game::showTable() const {
    std::cout << "\nTop card: " << deck->top() << "   |   Active colour: "
              << Card::colorName(activeColor) << "   |   Direction: "
              << (dir == 1 ? "clockwise" : "counter-clockwise") << "\n";
    std::cout << "Cards in hand: ";
    for (int i = 0; i < static_cast<int>(players.size()); ++i)
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
        std::cin >> p;
        std::cin.ignore(10000, '\n');
        players.push_back(p);
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
        std::cout << "  That card doesn't match the top card. Try again.\n";
    }
}

Color Game::chooseColor(const Player& p) const {
    int v = readInt(p.getName() + ", choose colour: 1) Red  2) Yellow  3) Green  4) Blue : ", 1, 4);
    return "RYGB"[v - 1];
}

// ---------- card effects ----------
int Game::applyCard(int idx, const Card& c, Color prevActive) {
    deck->discard(c);
    if (!c.isWild()) activeColor = c.getColor();
    int n = static_cast<int>(players.size());
    int skips = 0;

    if (c.isWildDrawFour()) {
        activeColor = chooseColor(players[idx]);
        say("  " + players[idx].getName() + " chooses " + Card::colorName(activeColor) + ".\n");
        int t = nextIndex(idx);
        clearScreen();
        std::cout << "Pass the device to " << players[t].getName() << " and press Enter when ready...";
        std::cin.get();
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
                drawCards(idx, 4);
            } else {
                say("  Legal play - challenge fails. " + players[t].getName() + " draws 6.\n");
                drawCards(t, 6);
                skips = 1;
            }
        }
    } else if (c.isWild()) {
        activeColor = chooseColor(players[idx]);
        say("  " + players[idx].getName() + " chooses " + Card::colorName(activeColor) + ".\n");
    } else if (c.getValue() == 'S') {
        say("  " + players[nextIndex(idx)].getName() + " is skipped!\n");
        skips = 1;
    } else if (c.getValue() == 'R') {
        if (n == 2) { 
            say("  Reverse (acts as Skip with 2 players)!\n"); 
            skips = 1; 
        } else { 
            dir = -dir; 
            say("  Direction reversed!\n"); 
        }
    } else if (c.getValue() == 'D') {
        int t = nextIndex(idx);
        drawCards(t, 2);
        say("  " + players[t].getName() + " loses their turn.\n");
        skips = 1;
    }
    return skips;
}

// ---------- UNO shout ----------
void Game::unoPhase(int idx) {
    say("  *** " + players[idx].getName() + ": UNO! (only 1 card left) ***\n");
}

// ---------- one turn ----------
int Game::takeTurn() {
    Player& p = players[current];
    Color prev = activeColor;
    Card top = deck->top();

    clearScreen();
    std::cout << "Pass the device to " << p.getName() << " and press Enter when ready...";
    std::cin.get();
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
            int idx = p.handSize() - 1;
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
        if (p.handSize() == 0) return current;
        if (p.handSize() == 1) unoPhase(current);
    }
    std::cout << "\nPress Enter to hide the screen...";
    std::cin.get();
    current = nextIndex(current, 1 + skips);
    return -1;
}

// ---------- start card ----------
void Game::handleStartCard(const Card& start) {
    int left = nextIndex(dealer);
    activeColor = start.getColor();
    say("Starting card: " + start.toString() + "\n");
    if (start.isWild()) {
        activeColor = chooseColor(players[left]);
        say("  " + players[left].getName() + " declares " + Card::colorName(activeColor) + ".\n");
        current = left;
    } else if (start.getValue() == 'D') {
        say("  " + players[left].getName() + " draws 2 and forfeits the first turn.\n");
        drawCards(left, 2);
        current = nextIndex(left);
    } else if (start.getValue() == 'R') {
        dir = -1;
        current = dealer;
        say("  Direction is counter-clockwise; dealer " + players[dealer].getName() + " starts.\n");
    } else if (start.getValue() == 'S') {
        say("  " + players[left].getName() + " is skipped.\n");
        current = nextIndex(dealer, 2);
    } else {
        current = left;
    }
}

// ---------- round / game ----------
void Game::endGame(int winner) {
    ++gamesPlayed;
    std::cout << "\n==============================================\n"
              << "  " << players[winner].getName()
              << " played their last card and WINS THE GAME!\n"
              << "==============================================\n"
              << "Cards left in the other hands:\n";
    for (int i = 0; i < static_cast<int>(players.size()); ++i)
        if (i != winner) std::cout << "  " << players[i] << "\n";
}

void Game::playGame() {
    deck->reset();
    int n = static_cast<int>(players.size());
    for (int i = 0; i < n; ++i) players[i].clearHand();
    dealer = gamesPlayed % n;
    dir = 1;
    events.clear();

    for (int r = 0; r < 7; ++r)
        for (int i = 0; i < n; ++i) {
            Card c;
            deck->draw(c);
            players[nextIndex(dealer, i + 1)] += c;
        }

    std::cout << "\n############ NEW GAME (dealer: "
              << players[dealer].getName() << ") ############\n";
    handleStartCard(deck->startCard());

    int winner = -1;
    while (winner < 0) winner = takeTurn();
    endGame(winner);
}

void Game::run() {
    setup();
    do {
        playGame();
    } while (readYesNo("\nShuffle the whole deck and start a brand-new match? (y/n): "));
    std::cout << "Thanks for playing! Games played: " << gamesPlayed << "\n";
}

// ===== main.cpp =====

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::cout << "=====================================\n"
              << "            C++  U N O\n"
              << "=====================================\n"
              << "First player to empty their hand wins (sudden death).\n\n";
    Game game;
    game.run();
    return 0;
}
