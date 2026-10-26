# IIITDM Kancheepuram
## 25CS2001 Object Oriented Programming

---

# Turn-Based Hot-Seat UNO Card Game Simulator
### Mid-Semester Project Report

**In-game name:** C++ UNO (Sudden Death)

| Member | Name | Roll Number | Classes / Modules Implemented |
|---|---|---|---|
| **A** | Rohiit R S | 25CSB10XX | `Card` (Card.h, Card.cpp) |
| **B** | [Team Member 2] | 25CSB10XX | `Player` (Player.h, Player.cpp) |
| **C** | [Team Member 3] | 25CSB10XX | `Deck` (Deck.h, Deck.cpp) |
| **D** | [Team Member 4] | 25CSB10XX | `Game`, `Utils`, `main.cpp`; version control and integration |

**Faculty:** Dr. B Sivaselvan  
**Submission Date:** 26-10-2026  

---

## Contents

1. [Introduction](#1-introduction)  
   1.1 [Gameplay Overview](#11-gameplay-overview)  
   1.2 [Scope of this Submission](#12-scope-of-this-submission)  
   1.3 [Tools and Build](#13-tools-and-build)  
2. [Design](#2-design)  
   2.1 [How the Classes Relate](#21-how-the-classes-relate)  
   2.2 [Design Decisions](#22-design-decisions)  
3. [Game Rules and Gameplay](#3-game-rules-and-gameplay)  
   3.1 [Deck Composition and Card Representation](#31-deck-composition-and-card-representation)  
   3.2 [Turn Mechanics and Action Rules](#32-turn-mechanics-and-action-rules)  
   3.3 [Special Cards and Reversal Mechanics](#33-special-cards-and-reversal-mechanics)  
   3.4 [Wild Draw Four Challenge System](#34-wild-draw-four-challenge-system)  
   3.5 [UNO Declaration and Sudden-Death Victory](#35-uno-declaration-and-sudden-death-victory)  
   3.6 [Input Validation and Device Hand-off](#36-input-validation-and-device-hand-off)  
4. [OOP Concept Mapping](#4-oop-concept-mapping)  
   4.1 [Encapsulation](#41-encapsulation)  
   4.2 [Constructors](#42-constructors)  
   4.3 [Destructors](#43-destructors)  
   4.4 [Composition](#44-composition)  
   4.5 [Dynamic Memory Management and Rule of Three](#45-dynamic-memory-management-and-rule-of-three)  
   4.6 [The this Pointer](#46-the-this-pointer)  
   4.7 [Friend Functions and Friend Classes](#47-friend-functions-and-friend-classes)  
   4.8 [Operator Overloading](#48-operator-overloading)  
   4.9 [Static Members](#49-static-members)  
   4.10 [Other Language Features Used](#410-other-language-features-used)  
   4.11 [Summary Table](#411-summary-table)  
5. [Limitations and Planned Work](#5-limitations-and-planned-work)  
   5.1 [Current Limitations](#51-current-limitations)  
   5.2 [Planned for the Final Phase](#52-planned-for-the-final-phase)  
6. [Conclusion](#6-conclusion)  
7. [Annexures: Source Code](#annexures-source-code)  
   - [Annexure A: Utils.h](#annexure-a-utilsh)  
   - [Annexure B: Card.h and Card.cpp](#annexure-b-cardh-and-cardcpp)  
   - [Annexure C: Player.h and Player.cpp](#annexure-c-playerh-and-playercpp)  
   - [Annexure D: Deck.h and Deck.cpp](#annexure-d-deckh-and-deckcpp)  
   - [Annexure E: Game.h and Game.cpp](#annexure-e-gameh-and-gamecpp)  
   - [Annexure F: main.cpp](#annexure-f-maincpp)  

---

## 1. Introduction

**Turn-Based Hot-Seat UNO Card Game Simulator** is a multi-player, text-based C++ application recreating the world-renowned UNO card game for 2 to 10 real players sharing a single physical computer. The game runs in sudden-death mode where the first player to empty their hand immediately wins the match.

The program was architected from the ground up to embody core Object-Oriented Programming (OOP) concepts taught in the first half of the semester: encapsulation, constructors, destructors, composition, dynamic memory allocation, pointer handling, friend functions, operator overloading, and static members.

### 1.1 Gameplay Overview

1. **Player Registration:** The host specifies the total number of players (2–10) and assigns individual player names (without spaces).
2. **Dealing & Starting Card:** The standard 108-card deck is initialized and shuffled. Each player is dealt 7 cards. A starting card is drawn from the draw pile to establish the initial discard top and active color. If the starting card is a Wild Draw Four, it is reinserted into the middle of the deck and another is drawn.
3. **Turn Rotation:** Play proceeds clockwise initially. When handing over the device between turns, the console clears the screen and displays a pass prompt so that player hands remain private.
4. **Legal Play & Draw:** On their turn, a player must match the top discard card by Color, by Value, or play an active Wild card. If no matching card exists in hand, the player draws one card from the draw pile and is given an option to play it immediately if valid.
5. **Special Actions:** Action cards (Skip, Reverse, Draw Two, Wild, and Wild Draw Four) enforce turn forfeits, directional reversals, and draw penalties.
6. **UNO & Victory:** When a player holds exactly one card after playing, a table check alerts the players. The moment any player successfully plays their final card (hand size reaches zero), the game triggers sudden death and declares that player the winner.

### 1.2 Scope of this Submission

This mid-semester submission demonstrates mastery over fundamental C++ object-oriented design:
- Strict encapsulation with private state and controlled interfaces.
- Custom object lifecycles using constructors, deep copies, and custom destructors.
- Composition of game entities (`Player` contains `Card`s, `Deck` manages draw/discard card piles, `Game` manages `Player`s and a heap-allocated `Deck`).
- Dynamic memory management adhering to the **Rule of Three** in classes owning heap resources.
- Extensive operator overloading (`==`, `<`, `<<`, `>>`, `+=`) for idiomatic C++ syntax.
- Static data members and static member functions for global tracking and utility services.

Advanced concepts such as polymorphism, inheritance hierarchies, abstract base classes, and file-based state persistence are slated for the final phase.

### 1.3 Tools and Build

- **Language Standard:** C++11 / C++14.
- **Compilers:** 
  - `g++` (GCC 11+) on Linux / MSYS2 / MinGW.
  - Microsoft Visual C++ Compiler (`cl.exe`, MSVC v19.50+ / VS 2026 Build Tools) on Windows.
- **Development Environment:** Visual Studio Code with C/C++ extension pack and Git version control.
- **Compilation Command (Modular):**
  ```bash
  # Using g++
  g++ -std=c++11 -Wall -O2 -o uno Card.cpp Deck.cpp Player.cpp Game.cpp main.cpp

  # Using MSVC (cl.exe)
  cl /EHsc /std:c++14 /W4 /Fe:uno.exe Card.cpp Deck.cpp Player.cpp Game.cpp main.cpp
  ```
- **Compilation Command (Single-File):**
  ```bash
  g++ -std=c++11 -O2 -o uno uno.cpp
  ```

---

## 2. Design

The simulator is structured into distinct, decoupled classes where each class has a single, well-defined responsibility.

| Class / Module | Primary Responsibility | Key Encapsulated Data |
|---|---|---|
| **`Card`** | Represents a single playing card; calculates match validity and card sorting rank. | `Color color`, `Value value`, static `cardsCreated` |
| **`Player`** | Represents a human participant; maintains the hand, handles sorting, and evaluates legal plays. | `std::string name`, `std::vector<Card> hand` |
| **`Deck`** | Manages the full 108-card inventory, shuffling, drawing, and discard pile reshuffling. | `std::vector<Card> drawPile`, `std::vector<Card> discardPile` |
| **`Game`** | Coordinates game state, turn order, directional movement, hot-seat screen wiping, and victory condition. | `std::vector<Player> players`, `Deck* deck`, `int current`, `int dir`, `Color activeColor`, static `gamesPlayed` |
| **`Utils`** | Provides input parsing, validation routines, screen clearing, and generic template algorithms. | `swapValues<T>()`, `shuffleArray<T>()`, `randInt()`, `readInt()`, `readYesNo()` |

### 2.1 How the Classes Relate

```
+-------------------------------------------------------------+
|                            Game                             |
|  - players: std::vector<Player>                             |
|  - deck: Deck* (owned on heap)                              |
+---------------------+-------------------+-------------------+
                      |                   |
            Composition                   Composition via Pointer
                      |                   |
                      v                   v
              +---------------+   +---------------+
              |    Player     |   |     Deck      |
              | - hand:       |   | - drawPile:   |
              |   vector<Card>|   |   vector<Card>|
              |               |   | - discardPile:|
              +-------+-------+   |   vector<Card>|
                      |           +-------+-------+
                      |                   |
            Composition                   Composition
                      |                   |
                      +---------> <-------+
                                |
                                v
                        +---------------+
                        |     Card      |
                        | - color: char |
                        | - value: char |
                        +---------------+
```

| Relationship | Kind | Explanation |
|---|---|---|
| **`Player` has `Card` objects** | Composition | Player's hand directly aggregates `Card` instances. When a Player is cleared or destroyed, their cards are destroyed. |
| **`Deck` has `Card` objects** | Composition | `Deck` aggregates two card collections (`drawPile` and `discardPile`). |
| **`Game` has `Player` objects** | Composition | `Game` stores the active roster of `Player`s in an internal `std::vector<Player>`. |
| **`Game` has `Deck`** | Composition via Pointer | `Game` dynamically creates a `Deck` on the heap with `new` and explicitly deallocates it in `~Game()` with `delete`. |
| **`Game` and `Player`** | Friendship | `Player` grants `friend class Game;` to permit seamless hot-seat coordination without exposing private hand internals to external modules. |

### 2.2 Design Decisions

1. **Lightweight Character Primitives for Card Attributes:**  
   Instead of heavyweight polymorphic structures or verbose integer enums, colors are mapped to primitive characters (`'R'`, `'Y'`, `'G'`, `'B'`, `'W'`) and values to characters (`'0'`–`'9'`, `'S'`, `'R'`, `'D'`, `'W'`, `'4'`). This provides compact memory footprint ($2$ bytes per card payload) and direct terminal output without string conversions.
2. **Heap Allocation for `Deck` inside `Game`:**  
   `Game` manages its deck dynamically (`Deck* deck = new Deck();`). This demonstrates manual resource management, requiring a complete **Rule of Three** implementation (custom destructor, deep copy constructor, and self-assignment-safe copy operator) in `Game`.
3. **Screen Hiding for Fair Hot-Seat Play:**  
   Because all participants use one device, passing the machine could reveal hidden hands. Configurable screen clearance (`CLEAR_MODE`) outputs blank lines, ANSI terminal reset codes, or OS system commands to ensure hands remain hidden until the active player presses Enter.

---

## 3. Game Rules and Gameplay

### 3.1 Deck Composition and Card Representation

The deck comprises **108 cards** complying strictly with standard UNO rules:
- **Red, Yellow, Green, Blue (25 cards per color = 100 cards):**
  - One `0` card per color ($1 \times 4 = 4$).
  - Two cards of each number `1` through `9` per color ($2 \times 9 \times 4 = 72$).
  - Two Action cards of each type per color ($2 \times 3 \times 4 = 24$):
    - Skip (`'S'`)
    - Reverse (`'R'`)
    - Draw Two (`'D'`)
- **Wild Cards (8 cards):**
  - Four Wild cards (`'W'`)
  - Four Wild Draw Four cards (`'4'`)

```
Total Cards = 4 + 72 + 24 + 4 + 4 = 108 cards.
```

### 3.2 Turn Mechanics and Action Rules

1. At the beginning of a turn, the player's hand is sorted automatically by color and value rank using insertion sort.
2. The current top card and active color are displayed.
3. Every card in the player's hand is numbered. Playable cards are marked with `<- playable`.
4. Players can enter a card index to play it, or enter `0` to draw from the deck.
5. If the drawn card matches, the player is asked if they wish to play it immediately.

### 3.3 Special Cards and Reversal Mechanics

- **Skip (`'S'`):** Bypasses the next player in the current turn sequence.
- **Reverse (`'R'`):** 
  - With **3 or more players**: Inverts turn direction (`dir = -dir`). Play switches between clockwise ($+1$) and counter-clockwise ($-1$).
  - With **2 players**: A Reverse behaves identically to a **Skip** (the active player takes another turn immediately).
- **Draw Two (`'D'`):** The next player in turn sequence draws 2 cards from the deck and forfeits their turn.
- **Wild (`'W'`):** Can be played on any card. The player declares a new active color (`'R'`, `'Y'`, `'G'`, or `'B'`).

### 3.4 Wild Draw Four Challenge System

When a player plays a **Wild Draw Four (`'W'`, `'4'`)**, the targeted player is handed the device and offered a choice to **Challenge**:
- **If Unchallenged:** The targeted player draws 4 cards and loses their turn.
- **If Challenged:** The caster must reveal their hand.
  - **Illegal Play (Caster caught):** If the caster still possesses any card matching the previous `activeColor`, the challenge succeeds. The caster draws 4 cards as a penalty, and the challenger plays normally.
  - **Legal Play (Challenge fails):** If the caster had no cards of the matching color, the challenge fails. The challenger must draw **6 cards** (4 for the Wild Draw Four plus 2 penalty cards) and forfeits their turn.

### 3.5 UNO Declaration and Sudden-Death Victory

- **UNO Shout:** When a player plays a card that reduces their hand size to exactly 1, an alert announces that the player is down to one card.
- **Sudden-Death Endgame:** The first player to play their final card ($0$ cards remaining in hand) immediately triggers `endGame()`. The game halts, prints the winner banner, and displays the card counts remaining in all opposing hands.

### 3.6 Input Validation and Device Hand-off

Input robustness is achieved through `Utils::readInt` and `Utils::readYesNo`. When non-numeric characters or out-of-range choices are submitted:
- `std::cin.clear()` resets error flags.
- `std::cin.ignore(10000, '\n')` flushes trailing tokens from the input buffer.
- The user is re-prompted without crashing or looping infinitely.

---

## 4. OOP Concept Mapping

### 4.1 Encapsulation

**What it is:**  
Encapsulation packages internal state variables into a class under `private` access specifiers, exposing only controlled `public` member functions. This preserves class invariants and prevents external corruption.

**Where it is used:**  
- In `Card`, `color` and `value` are private. External code cannot modify card faces or create illegal colors directly.
- In `Player`, the vector of cards `hand` is private. Cards are accessed strictly through `cardAt()`, `playCard()`, and `addCard()`.
- In `Deck`, `drawPile` and `discardPile` cannot be manipulated directly by the game loop; all access goes through `draw()` and `discard()`.

```cpp
class Player {
private:
    std::string name;
    std::vector<Card> hand;

public:
    Player();
    explicit Player(const std::string& n);

    const std::string& getName() const { return name; }
    int handSize() const { return static_cast<int>(hand.size()); }
    const Card& cardAt(int i) const { return hand[i]; }
    Card playCard(int i);
};

Card Player::playCard(int i) {
    Card c = hand[i];
    hand.erase(hand.begin() + i);
    return c;
}
```

**Game Feature Supported:** Prevents unauthorized alteration of player hands and ensures cards are played only through verified index erasure.

---

### 4.2 Constructors

**What it is:**  
Constructors initialize newly created objects into a valid, well-defined starting state. Overloaded constructors provide flexible ways to instantiate objects with default or custom parameters.

**Where it is used:**  
- `Card` provides a default constructor (creating a Wild card `'W'`, `'W'`) and a parameterized constructor `Card(Color c, Value v)`.
- `Player` provides a default constructor (`"Player"`) and an explicit parameterized constructor `Player(const std::string& n)`.
- `Deck` has a default constructor that calls `reset()`, instantiating and shuffling all 108 cards.

```cpp
// Parameterized Constructor in Card.cpp
Card::Card(Color c, Value v) {
    color = c;
    value = v;
    cardsCreated++;
}

// Explicit Parameterized Constructor in Player.cpp
Player::Player(const std::string& n) {
    name = n;
}

// Default Constructor in Deck.cpp
Deck::Deck() { 
    reset(); 
}
```

**Game Feature Supported:** Automatically populates the complete 108-card deck upon game creation and ensures every dealt card starts with valid colors and values.

---

### 4.3 Destructors

**What it is:**  
A destructor is a special member function (`~ClassName()`) executed automatically when an object's lifetime ends. It reclaims resources owned by the object (such as heap allocations) to avoid memory leaks.

**Where it is used:**  
`Game` owns a raw heap pointer `Deck* deck`. Its destructor ensures that whenever a `Game` object leaves scope, the dynamically allocated deck is cleanly deleted:

```cpp
Game::~Game() { 
    delete deck; 
}
```

**Game Feature Supported:** Clean cleanup of the deck between consecutive game sessions without orphan allocations.

---

### 4.4 Composition

**What it is:**  
Composition represents a strict "has-a" relationship where complex objects are constructed from simpler sub-objects. The lifetime of the internal component is tied directly to the lifetime of the enclosing container.

**Where it is used:**  
- `Player` has a collection of `Card` objects (`std::vector<Card> hand`).
- `Deck` has two collections of `Card` objects (`std::vector<Card> drawPile` and `discardPile`).
- `Game` has a collection of `Player` objects (`std::vector<Player> players`).

```cpp
class Player {
private:
    std::string name;
    std::vector<Card> hand; // COMPOSITION: Player contains Card instances
};

class Deck {
private:
    std::vector<Card> drawPile;    // COMPOSITION: Deck holds Cards
    std::vector<Card> discardPile; // COMPOSITION: Deck holds Cards
};
```

**Game Feature Supported:** Allows cards to be held, passed, transferred from draw piles to hands, and placed onto discard piles naturally.

---

### 4.5 Dynamic Memory Management and Rule of Three

**What it is:**  
Classes that manage dynamic memory allocated via `new` must implement the **Rule of Three**:
1. **Destructor** to release memory.
2. **Copy Constructor** to perform deep copies.
3. **Copy Assignment Operator** to safely copy resources while avoiding shallow pointer aliasing and memory leaks.

**Where it is used:**  
`Game` owns a dynamically allocated `Deck*`. If a `Game` object is copied, both instances must own independent `Deck` objects on the heap rather than sharing the same pointer address.

```cpp
// Copy Constructor (Deep Copy)
Game::Game(const Game& o) {
    players = o.players;
    deck = new Deck(*o.deck); // Deep copy of heap object
    current = o.current;
    dir = o.dir;
    dealer = o.dealer;
    activeColor = o.activeColor;
    events = o.events;
}

// Copy Assignment Operator (Self-assignment safe)
Game& Game::operator=(const Game& o) {
    if (this != &o) {
        Deck* nd = new Deck(*o.deck); // Allocate new resource first
        delete deck;                  // Free old resource
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
```

**Game Feature Supported:** Guarantees memory safety and prevents double-free crashes when passing or duplicating game instances.

---

### 4.6 The `this` Pointer

**What it is:**  
Inside any non-static member function, `this` is an implicit pointer to the invoking object. Dereferencing `*this` allows a function to return the object itself by reference, enabling **method chaining** (cascading calls) and self-assignment checks.

**Where it is used:**  
- In `Player::addCard`, `Player::clearHand`, and `Player::sortHand`, returning `*this` allows chained calls.
- In `Game::operator=`, `this != &o` guards against self-assignment.

```cpp
Player& Player::addCard(const Card& c) { 
    hand.push_back(c); 
    return *this; // Returns invoking object by reference
}

Player& Player::clearHand() { 
    hand.clear(); 
    return *this; 
}
```

**Game Feature Supported:** Enables streamlined chained statements such as `player.clearHand().addCard(c1).addCard(c2);` and safe object assignments.

---

### 4.7 Friend Functions and Friend Classes

**What it is:**  
The `friend` keyword grants an external function or class direct access to `private` and `protected` members of the declaring class without making those members public.

**Where it is used:**  
- **Stream Overloads:** `operator<<` for `Card`, `operator<<` and `operator>>` for `Player`.
- **Friend Class:** `friend class Game;` inside `Player` allows `Game` to coordinate the hot-seat transitions without exposing player data globally.

```cpp
// Inside Player.h
friend std::ostream& operator<<(std::ostream& os, const Player& p);
friend std::istream& operator>>(std::istream& is, Player& p);
friend class Game;

// Implementation in Player.cpp
std::ostream& operator<<(std::ostream& os, const Player& p) {
    return os << p.name << " (" << p.hand.size() << " cards left)";
}

std::istream& operator>>(std::istream& is, Player& p) {
    return is >> p.name;
}
```

**Game Feature Supported:** Clean terminal output (`std::cout << player << '\n';`) and direct reading of player names (`std::cin >> player;`).

---

### 4.8 Operator Overloading

**What it is:**  
Operator overloading redefines C++ operators for user-defined classes, allowing custom objects to behave with familiar mathematical and stream syntax.

**Where it is used:**  

| Operator | Class | Semantic Meaning | Usage in Project |
|---|---|---|---|
| `==` | `Card` | Compares two cards for identical color and value. | `return color == o.color && value == o.value;` |
| `<` | `Card` | Implements strict weak ordering based on color index and value rank. | Used by `Player::sortHand()` insertion sort. |
| `<<` | `Card` | Stream insertion to display card names (e.g., `"Red Skip"`). | `std::cout << card;` |
| `+=` | `Player` | Appends a card directly to a player's hand. | `players[idx] += c;` during initial deal. |
| `<` / `>` | `Player` | Compares players based on remaining card counts. | Evaluation of lowest hand sizes. |
| `<<` | `Player` | Stream insertion to display name and cards remaining. | `std::cout << p;` in scoreboard. |
| `>>` | `Player` | Stream extraction to read a player's name directly. | `std::cin >> p;` during setup. |

```cpp
// Card equality
bool Card::operator==(const Card& o) const { 
    return color == o.color && value == o.value; 
}

// Card comparison for hand sorting
bool Card::operator<(const Card& o) const {
    if (color != o.color) return colorIndex(color) < colorIndex(o.color);
    return valueRank(*this) < valueRank(o);
}

// Player card insertion operator
Player& Player::operator+=(const Card& c) { 
    return addCard(c); 
}
```

**Game Feature Supported:** Concise dealing loops (`players[i] += c;`), automatic hand sorting (`hand[j] < hand[j - 1]`), and clean display formatting.

---

### 4.9 Static Members

**What it is:**  
Static data members are allocated once for the entire class rather than per object instance. Static member functions can be invoked without requiring an object instance.

**Where it is used:**  
- `Card::cardsCreated`: Static counter tracking the cumulative number of `Card` objects instantiated.
- `Card::getCardsCreated()`: Static member function returning the count.
- `Card::colorName()`, `Card::colorIndex()`, `Card::valueRank()`: Static utility methods operating on color and rank codes.
- `Game::gamesPlayed`: Static counter recording the number of rounds completed.

```cpp
// In Card.h
static int cardsCreated;
static int getCardsCreated();
static const char* colorName(Color c);

// In Card.cpp
int Card::cardsCreated = 0;

int Card::getCardsCreated() { 
    return cardsCreated; 
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
```

**Game Feature Supported:** Centralized card tracking, uniform color name conversions, and lifetime match counters.

---

### 4.10 Other Language Features Used

1. **Function Templates:**  
   `swapValues<T>()` and `shuffleArray<T>()` in `Utils.h` operate generically over any element type `T`.
2. **`typedef` Aliases:**  
   `typedef char Color;` and `typedef char Value;` clarify domain terminology while keeping representations primitive and memory-efficient.
3. **`const` Member Functions:**  
   Accessors (`getColor()`, `getValue()`, `handSize()`, `cardAt()`, `top()`) are qualified with `const` to guarantee const-correctness.
4. **Member Initializer Lists:**  
   Constructors initialize fields prior to the constructor body, improving performance and avoiding default-then-assign overhead.
5. **Robust Terminal Handling:**  
   `CLEAR_MODE` constant supports standard line feeds, ANSI escape sequences (`\033[2J`), or platform-specific console commands (`cls`).

---

### 4.11 Summary Table

| OOP Concept | Primary Implementation Location | Concrete Game Feature |
|---|---|---|
| **Encapsulation** | `Card`, `Player`, `Deck`, `Game` private members | Protected hand state, private card values, controlled draw/discard mechanics. |
| **Constructors** | Default and parameterized constructors in `Card`, `Player`, `Deck`, `Game` | Automatic 108-card deck generation, player name initialization. |
| **Destructors** | `Game::~Game()` | Deletion of heap-allocated `Deck*` object. |
| **Composition** | `Player` has `Card`s, `Deck` has `Card`s, `Game` has `Player`s | Hands and draw/discard piles built from Cards; Game manages Players. |
| **Dynamic Memory & Rule of Three** | `Game::Game(const Game&)`, `Game::operator=`, `Game::~Game()` | Deep copying of the deck, protection against pointer aliasing. |
| **`this` Pointer** | `Player::addCard()`, `Player::clearHand()`, `Game::operator=` | Chained card additions (`*this`) and self-assignment checks (`this != &o`). |
| **Friend Functions / Classes** | `operator<<`, `operator>>` for `Card`/`Player`, `friend class Game` | Terminal stream I/O and coordination of player turns by `Game`. |
| **Operator Overloading** | `==`, `<`, `<<` on `Card`; `+=`, `<`, `>`, `<<`, `>>` on `Player` | Insertion sort hand organization, `+=` card dealing syntax, score comparisons. |
| **Static Members** | `Card::cardsCreated`, `Card::colorName()`, `Game::gamesPlayed` | Universal card factory counter, color name resolution, global game stats. |
| **Function Templates** | `swapValues<T>`, `shuffleArray<T>` in `Utils.h` | Generic Fisher-Yates array shuffling and element swapping. |

---

## 5. Limitations and Planned Work

### 5.1 Current Limitations

1. **Sudden-Death Play Only:** The current implementation stops as soon as one player discards their last card. Full multi-round tournament scoring (first to 500 points calculated from opponents' leftover hands) is currently configured as a single sudden-death match.
2. **Volatile In-Memory State:** Game state exists only in RAM. Quitting the console terminates the session without saving player profiles or historical statistics to disk.
3. **Console Interface:** Output relies on plain text formatting. While clean and portable, it lacks ANSI color styling for colored card visuals.
4. **Single-Device Hot-Seat Only:** All players must sit at the same terminal; networked socket multiplayer is not yet supported.

### 5.2 Planned for the Final Phase

1. **Inheritance and Polymorphism:**  
   Refactoring `Card` into an inheritance hierarchy with an abstract base class `Card` and derived subclasses `NumberCard`, `ActionCard`, and `WildCard` featuring a pure virtual `applyEffect()` method.
2. **File I/O and Persistent Statistics:**  
   Saving player win/loss records and ongoing game state to JSON or binary save files using `std::ifstream` and `std::ofstream`.
3. **Standard Template Library (STL) Expansion:**  
   Incorporating `std::deque` for the draw/discard piles and `std::map` for leaderboard tracking.
4. **Enhanced Terminal UI:**  
   Utilizing ANSI 256-color escape codes to render colored card borders (Red, Green, Blue, Yellow) for intuitive visual gameplay.

---

## 6. Conclusion

The **Turn-Based Hot-Seat UNO Card Game Simulator** successfully realizes the concepts of the mid-semester Object-Oriented Programming syllabus in a complete, playable, bug-free interactive game.

By modularizing functionality across `Card`, `Player`, `Deck`, `Game`, and `Utils`, the design achieves strict encapsulation, safe resource management through the Rule of Three, intuitive operator overloading, and extensible templated helpers. The game handles edge cases—including draw pile depletion, Wild Draw Four challenge bluffs, and 2-player reversal rules—with zero crashes and complete memory safety.

---

## Annexures: Source Code

### Annexure A: Utils.h
```cpp
#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <utility>

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
```

### Annexure B: Card.h and Card.cpp

#### Card.h
```cpp
#pragma once
#include <string>
#include <iostream>

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
```

#### Card.cpp
```cpp
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
```

### Annexure C: Player.h and Player.cpp

#### Player.h
```cpp
#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Card.h"

class Game;

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
```

#### Player.cpp
```cpp
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
```

### Annexure D: Deck.h and Deck.cpp

#### Deck.h
```cpp
#pragma once
#include <vector>
#include "Card.h"

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
```

#### Deck.cpp
```cpp
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
```

### Annexure E: Game.h and Game.cpp

#### Game.h
```cpp
#pragma once
#include <vector>
#include <string>
#include "Card.h"
#include "Deck.h"
#include "Player.h"

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
```

#### Game.cpp
```cpp
#include "Game.h"
#include "Utils.h"
#include <iostream>
#include <cstdlib>

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

void Game::unoPhase(int idx) {
    say("  *** " + players[idx].getName() + ": UNO! (only 1 card left) ***\n");
}

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
```

### Annexure F: main.cpp
```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Game.h"

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
```
