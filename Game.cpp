#include "Game.h"
#include "Utils.h"
#include <iostream>
#include <cstdlib>

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
