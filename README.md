# UNO in C++ (Multi-File Architecture)

A robust, object-oriented, hot-seat multiplayer implementation of the classic **UNO** card game written in modern C++ (C++14 / C++11 compatible).

---

## Features

- **Modular Architecture**: Cleanly separated into modular header and source files adhering to object-oriented programming best practices.
- **Custom Dynamic Container (`DynArray<T>`)**: Self-contained generic dynamic array template implementing the **Rule of Three** (custom copy constructor, destructor, and copy assignment operator) with bounds checking, dynamic doubling, and element insertion/removal.
- **Complete Official UNO Rules**:
  - Full standard 108-card deck (Number cards, Skip, Reverse, Draw Two, Wild, and Wild Draw Four).
  - Special 2-player rule support (Reverse behaves as a Skip).
  - Starting card resolution (including Draw Two, Skip, Reverse, and Wild).
  - Wild Draw Four legality check and challenge mechanism with proper penalties.
  - Interactive "UNO!" call and catch phase when a player is down to 1 card.
  - Scoring system: Number cards = face value, Action cards = 20 pts, Wild/Wild Draw 4 = 50 pts. First player to 500 points wins the match.
- **Hot-Seat Pass & Play**: Screen hiding mechanism so players can safely pass the device without exposing their hands.
- **Comprehensive Edge Case Test Suite**: Unit and integration test suite covering container edge cases, card classification, reshuffling, rule validation, and memory safety.

---

## File Structure

```
UNO/
├── Utils.h          # RNG utilities, screen clearing, and safe input parsing
├── DynArray.h       # Generic dynamic array template with Rule of Three & shuffle
├── Card.h           # Card class definition, Color & Value enumerations
├── Card.cpp         # Card implementation, point calculation, string formatting
├── Player.h         # Player class definition with hand management
├── Player.cpp       # Hand sorting, card validation, and scoring logic
├── Deck.h           # Deck class definition with draw and discard piles
├── Deck.cpp         # 108-card deck initialization, start card & reshuffle logic
├── Game.h           # Game loop controller, hot-seat coordinator, Rule of Three
├── Game.cpp         # Turn execution, card effect application, round transitions
├── main.cpp         # Application entry point
├── test_runner.cpp  # Edge case and unit test suite
├── CMakeLists.txt   # CMake build configuration
├── build.bat        # Windows one-click build script
└── README.md        # Documentation
```

---

## How to Build

### Option 1: Using CMake
```bash
cmake -B build
cmake --build build --config Release
```

### Option 2: Using MSVC (`cl.exe`)
From a Visual Studio Developer Command Prompt:
```cmd
cl /EHsc /std:c++14 /W4 /Fe:uno.exe Card.cpp Deck.cpp Player.cpp Game.cpp main.cpp
```

### Option 3: Using GCC / MinGW (`g++`)
```bash
g++ -std=c++14 -Wall -Wextra -O2 -o uno Card.cpp Deck.cpp Player.cpp Game.cpp main.cpp
```

### Option 4: Windows Build Script
```cmd
.\build.bat
```

---

## Running the Game

Launch the compiled executable:
```bash
./uno        # Linux / macOS
.\uno.exe    # Windows
```

Follow the on-screen prompts to set the number of players (2 to 10), player names, and play your cards hot-seat style!

---

## Running the Edge Case Test Suite

To build and run the test suite:
```cmd
# Using MSVC:
cl /EHsc /std:c++14 /W4 /Fe:test_runner.exe Card.cpp Deck.cpp Player.cpp Game.cpp test_runner.cpp
.\test_runner.exe

# Using GCC / Clang:
g++ -std=c++14 -O2 -o test_runner Card.cpp Deck.cpp Player.cpp Game.cpp test_runner.cpp
./test_runner
```

### Test Coverage Highlights:
- **`DynArray`**: Capacity expansion, deep copies, copy assignment, self-assignment, out-of-bounds safety, arbitrary index insertion and removal.
- **`Card`**: Action/Wild/Number categorization, point values, matching logic on active colors vs card faces.
- **`Deck`**: Exact 108-card distribution, draw depletion, automatic reshuffle of discard pile preserving the top card, and rejection of Wild Draw Four as starting card.
- **`Player & Rules`**: Wild Draw Four legality enforcement (disallowed if player holds active color), hand sorting via insertion sort, cascading method calls.
- **`Game`**: Rule of Three verification (deep copy, assignment, destructor).

