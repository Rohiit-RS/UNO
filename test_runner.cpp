#include <iostream>
#include <cassert>
#include <sstream>
#include "Utils.h"
#include "DynArray.h"
#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "Game.h"

// Helper assertion macro with clear output
#define TEST_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "FAILED: " << msg << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
            std::exit(1); \
        } \
    } while (0)

void testDynArray() {
    std::cout << "[Test] DynArray Edge Cases...\n";

    // Capacity & push_back
    DynArray<int> arr(2);
    TEST_ASSERT(arr.size() == 0, "Initial size should be 0");
    TEST_ASSERT(arr.empty(), "Initial array should be empty");

    arr.push_back(10);
    arr += 20;
    arr.push_back(30); // triggers grow()
    TEST_ASSERT(arr.size() == 3, "Size should be 3 after 3 pushes");
    TEST_ASSERT(arr[0] == 10 && arr[1] == 20 && arr[2] == 30, "Values mismatch");

    // pop_back
    int popped = arr.pop_back();
    TEST_ASSERT(popped == 30, "Popped value should be 30");
    TEST_ASSERT(arr.size() == 2, "Size should be 2 after pop");

    // insertAt front, middle, back
    arr.insertAt(0, 5);      // [5, 10, 20]
    arr.insertAt(2, 15);     // [5, 10, 15, 20]
    arr.insertAt(4, 25);     // [5, 10, 15, 20, 25]
    TEST_ASSERT(arr.size() == 5, "Size should be 5 after inserts");
    TEST_ASSERT(arr[0] == 5 && arr[1] == 10 && arr[2] == 15 && arr[3] == 20 && arr[4] == 25,
                "Insert elements mismatch");

    // removeAt front, middle, back
    int r0 = arr.removeAt(0); // removes 5 -> [10, 15, 20, 25]
    TEST_ASSERT(r0 == 5 && arr.size() == 4 && arr[0] == 10, "RemoveAt(0) failed");
    int rMid = arr.removeAt(1); // removes 15 -> [10, 20, 25]
    TEST_ASSERT(rMid == 15 && arr.size() == 3 && arr[1] == 20, "RemoveAt(1) failed");
    int rLast = arr.removeAt(2); // removes 25 -> [10, 20]
    TEST_ASSERT(rLast == 25 && arr.size() == 2 && arr[1] == 20, "RemoveAt last failed");

    // Out-of-bounds exceptions
    bool threw = false;
    try { arr.insertAt(-1, 99); } catch (const std::out_of_range&) { threw = true; }
    TEST_ASSERT(threw, "insertAt(-1) should throw out_of_range");

    threw = false;
    try { arr.insertAt(10, 99); } catch (const std::out_of_range&) { threw = true; }
    TEST_ASSERT(threw, "insertAt(>sz) should throw out_of_range");

    threw = false;
    try { arr.removeAt(5); } catch (const std::out_of_range&) { threw = true; }
    TEST_ASSERT(threw, "removeAt(>=sz) should throw out_of_range");

    threw = false;
    try { arr[-1]; } catch (const std::out_of_range&) { threw = true; }
    TEST_ASSERT(threw, "operator[](-1) should throw out_of_range");

    threw = false;
    try { arr[2]; } catch (const std::out_of_range&) { threw = true; }
    TEST_ASSERT(threw, "operator[](2) should throw out_of_range on size 2");

    // Deep copy constructor
    DynArray<int> copy(arr);
    TEST_ASSERT(copy.size() == arr.size(), "Copy should have same size");
    copy[0] = 999;
    TEST_ASSERT(arr[0] == 10, "Original array must not be modified by copy modification");

    // Copy assignment & self-assignment
    DynArray<int> assigned;
    assigned = arr;
    TEST_ASSERT(assigned.size() == arr.size() && assigned[0] == 10, "Assignment copy failed");
    assigned = assigned; // self-assignment
    TEST_ASSERT(assigned.size() == arr.size() && assigned[0] == 10, "Self assignment broke array");

    // Clear
    arr.clear();
    TEST_ASSERT(arr.empty() && arr.size() == 0, "Array clear failed");
    threw = false;
    try { arr.pop_back(); } catch (const std::out_of_range&) { threw = true; }
    TEST_ASSERT(threw, "pop_back on empty array should throw out_of_range");

    std::cout << "  -> DynArray tests passed!\n";
}

void testCard() {
    std::cout << "[Test] Card Edge Cases...\n";

    Card c1(RED, SEVEN);
    Card c2(RED, THREE);
    Card c3(BLUE, SEVEN);
    Card c4(GREEN, SKIP);
    Card c5(GREEN, REVERSE);
    Card c6(YELLOW, DRAW_TWO);
    Card wild(WILD_COLOR, WILD);
    Card wdf(WILD_COLOR, WILD_DRAW_FOUR);

    // Classification tests
    TEST_ASSERT(c1.isNumber(), "RED 7 should be number");
    TEST_ASSERT(!c1.isAction() && !c1.isWild(), "RED 7 is not action or wild");
    TEST_ASSERT(c4.isAction() && !c4.isNumber() && !c4.isWild(), "SKIP should be action");
    TEST_ASSERT(c5.isAction(), "REVERSE should be action");
    TEST_ASSERT(c6.isAction(), "DRAW_TWO should be action");
    TEST_ASSERT(wild.isWild() && !wild.isNumber() && !wild.isAction(), "WILD should be wild");
    TEST_ASSERT(wdf.isWild() && !wdf.isNumber() && !wdf.isAction(), "WDF should be wild");

    // Points tests
    TEST_ASSERT(c1.points() == 7, "RED 7 should be worth 7 points");
    Card zeroCard(YELLOW, ZERO);
    TEST_ASSERT(zeroCard.points() == 0, "ZERO should be worth 0 points");
    TEST_ASSERT(c4.points() == 20, "SKIP should be worth 20 points");
    TEST_ASSERT(c5.points() == 20, "REVERSE should be worth 20 points");
    TEST_ASSERT(c6.points() == 20, "DRAW_TWO should be worth 20 points");
    TEST_ASSERT(wild.points() == 50, "WILD should be worth 50 points");
    TEST_ASSERT(wdf.points() == 50, "WDF should be worth 50 points");

    // Matching tests
    // 1. Same color
    TEST_ASSERT(c1.matches(c2, RED), "RED 7 matches RED 3 with active RED");
    // 2. Same value
    TEST_ASSERT(c1.matches(c3, BLUE), "RED 7 matches BLUE 7 with active BLUE");
    // 3. Different color and value
    TEST_ASSERT(!c1.matches(c4, GREEN), "RED 7 does not match GREEN SKIP with active GREEN");
    // 4. Wild always matches
    TEST_ASSERT(wild.matches(c1, RED), "WILD matches RED 7");
    TEST_ASSERT(wdf.matches(c1, RED), "WDF matches RED 7");
    // 5. Card matches active color after Wild
    Card greenThree(GREEN, THREE);
    TEST_ASSERT(greenThree.matches(wild, GREEN), "GREEN 3 matches WILD if active color is GREEN");
    TEST_ASSERT(!c1.matches(wild, GREEN), "RED 7 does not match WILD if active color is GREEN");

    // Operator == and <
    Card c1Clone(RED, SEVEN);
    TEST_ASSERT(c1 == c1Clone, "Same color and value should be equal");
    TEST_ASSERT(!(c1 == c2), "Different value should not be equal");
    TEST_ASSERT(!(c1 == c3), "Different color should not be equal");
    TEST_ASSERT(c2 < c1, "RED 3 should be < RED 7");
    TEST_ASSERT(c1 < c6, "RED should be < YELLOW");

    // String representations
    TEST_ASSERT(c1.toString() == "Red 7", "RED 7 toString mismatch");
    TEST_ASSERT(c4.toString() == "Green Skip", "Green Skip toString mismatch");
    TEST_ASSERT(wild.toString() == "Wild", "Wild toString mismatch");
    TEST_ASSERT(wdf.toString() == "Wild Draw Four", "WDF toString mismatch");

    std::cout << "  -> Card tests passed!\n";
}

void testDeck() {
    std::cout << "[Test] Deck Edge Cases...\n";

    Deck deck;
    TEST_ASSERT(deck.drawSize() == 108, "New deck should have 108 cards");

    // Verify all 108 cards can be drawn
    int cardCount = 0;
    Card c;
    while (deck.draw(c)) {
        ++cardCount;
    }
    TEST_ASSERT(cardCount == 108, "Should have drawn exactly 108 cards");
    TEST_ASSERT(!deck.draw(c), "Drawing from completely empty deck should return false");

    // Reset deck and test starting card
    deck.reset();
    TEST_ASSERT(deck.drawSize() == 108, "Reset deck should have 108 cards");
    Card start = deck.startCard();
    TEST_ASSERT(start.getValue() != WILD_DRAW_FOUR, "Starting card can never be WILD_DRAW_FOUR");
    TEST_ASSERT(deck.top() == start, "Top card of discard pile should be start card");
    TEST_ASSERT(deck.drawSize() == 107, "Draw pile should have 107 cards remaining");

    // Test refill reshuffle mechanism
    // Draw all 107 cards from draw pile and discard them onto discard pile
    for (int i = 0; i < 107; ++i) {
        bool ok = deck.draw(c);
        TEST_ASSERT(ok, "Drawing from draw pile should succeed");
        deck.discard(c);
    }
    TEST_ASSERT(deck.drawSize() == 0, "Draw pile should be empty before refill");

    // The top card of the discard pile right before refill
    Card topBeforeRefill = deck.top();

    // The 108th draw triggers refill()
    bool drawOk = deck.draw(c);
    TEST_ASSERT(drawOk, "Draw should succeed after reshuffle from discard pile");
    TEST_ASSERT(deck.drawSize() == 106, "Draw pile should have 106 cards after refill and 1 draw");
    TEST_ASSERT(deck.top() == topBeforeRefill, "Top card of discard pile must remain unchanged after refill");

    std::cout << "  -> Deck tests passed!\n";
}

void testPlayerAndRules() {
    std::cout << "[Test] Player & UNO Rules Edge Cases...\n";

    Player p("Alice");
    TEST_ASSERT(p.getName() == "Alice", "Player name mismatch");
    TEST_ASSERT(p.handSize() == 0, "New player hand should be empty");
    TEST_ASSERT(p.getScore() == 0, "New player score should be 0");

    Card r5(RED, FIVE);
    Card b5(BLUE, FIVE);
    Card g9(GREEN, NINE);
    Card wild(WILD_COLOR, WILD);
    Card wdf(WILD_COLOR, WILD_DRAW_FOUR);

    p += r5;
    p += b5;
    p += g9;
    p += wild;
    p += wdf;

    TEST_ASSERT(p.handSize() == 5, "Hand size should be 5");
    TEST_ASSERT(p.hasColor(RED), "Player should have RED");
    TEST_ASSERT(p.hasColor(BLUE), "Player should have BLUE");
    TEST_ASSERT(p.hasColor(GREEN), "Player should have GREEN");
    TEST_ASSERT(!p.hasColor(YELLOW), "Player should not have YELLOW");

    // Hand points
    // 5 + 5 + 9 + 50 + 50 = 119
    TEST_ASSERT(p.handPoints() == 119, "Hand points calculation mismatch");

    // CanPlay edge cases
    // Top card is RED 1, active color is RED
    Card topRed1(RED, ONE);
    TEST_ASSERT(p.canPlay(0, topRed1, RED), "RED 5 can play on RED 1");
    TEST_ASSERT(!p.canPlay(1, topRed1, RED), "BLUE 5 cannot play on RED 1 (diff color & value)");
    TEST_ASSERT(!p.canPlay(2, topRed1, RED), "GREEN 9 cannot play on RED 1");
    TEST_ASSERT(p.canPlay(3, topRed1, RED), "WILD can always play");

    // CRITICAL UNO RULE: Wild Draw Four legality check!
    // Player HAS a RED card (RED 5), active color is RED.
    // Therefore, Wild Draw Four is ILLEGAL!
    TEST_ASSERT(!p.canPlay(4, topRed1, RED), "WDF must be illegal when player has active color");

    // If active color is YELLOW (which player has none):
    // Wild Draw Four should now be LEGAL!
    Card topYellow1(YELLOW, ONE);
    TEST_ASSERT(p.canPlay(4, topYellow1, YELLOW), "WDF must be legal when player has no active color");

    // Sorting hand
    p.sortHand();
    for (int i = 0; i < p.handSize() - 1; ++i) {
        TEST_ASSERT(p.cardAt(i) < p.cardAt(i + 1) || p.cardAt(i) == p.cardAt(i + 1),
                    "Hand must be sorted in non-descending order");
    }

    // Play card (removeAt)
    int initialSize = p.handSize();
    Card played = p.playCard(0);
    TEST_ASSERT(p.handSize() == initialSize - 1, "Hand size should decrease by 1");

    // Clear hand & score cascade
    p.clearHand();
    TEST_ASSERT(p.handSize() == 0, "Hand size should be 0 after clear");
    p.addScore(50).addScore(25);
    TEST_ASSERT(p.getScore() == 75, "Score should be 75 after chained addScore");

    std::cout << "  -> Player & Rules tests passed!\n";
}

void testGameRuleOfThreeAndMechanics() {
    std::cout << "[Test] Game Mechanics & Rule of Three...\n";

    // Game instantiation
    Game g1(250);
    // Copy constructor test
    Game g2(g1);
    // Copy assignment test
    Game g3(100);
    g3 = g1;
    // Self-assignment test
    g3 = g3;

    TEST_ASSERT(Game::getRoundsPlayed() >= 0, "Rounds played should be non-negative");

    std::cout << "  -> Game mechanics & Rule of Three passed!\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "   RUNNING COMPREHENSIVE UNO UNIT TESTS\n";
    std::cout << "========================================\n\n";

    testDynArray();
    testCard();
    testDeck();
    testPlayerAndRules();
    testGameRuleOfThreeAndMechanics();

    std::cout << "\n========================================\n";
    std::cout << "   ALL TESTS PASSED SUCCESSFULLY! (100%)\n";
    std::cout << "========================================\n";
    return 0;
}
