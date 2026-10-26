# UNO in C++ (Hot-Seat Multiplayer)

A clean, object-oriented hot-seat multiplayer implementation of the classic **UNO** card game in C++ (C++11 or higher).

In this version, gameplay is **sudden death**: the first player to empty their hand wins the game immediately.

---

## Project Structure

```
UNO/
├── Utils.h       # Random number generator, screen clearing, input parsing, swap & shuffle templates
├── Card.h        # Card class definition, Color & Value type definitions
├── Card.cpp      # Card method implementations, rank sorting, string conversion
├── Player.h      # Player class definition with std::vector<Card> hand management
├── Player.cpp    # Player actions, hand sorting, card matching
├── Deck.h        # Deck class definition with draw and discard piles
├── Deck.cpp      # 108-card deck initialization, start card & reshuffle logic
├── Game.h        # Game controller definition and turn management
├── Game.cpp      # Hot-seat turn execution, action card effects, game loop
├── main.cpp      # Application entry point
├── uno.cpp       # Single-file standalone version
└── README.md     # Documentation
```

---

## How to Compile and Run

### 1. Using `g++` (Linux, macOS, or MinGW on Windows)

#### Modular Version:
```bash
g++ -std=c++11 -O2 -o uno Card.cpp Deck.cpp Player.cpp Game.cpp main.cpp
./uno
```

#### Single-File Version:
```bash
g++ -std=c++11 -O2 -o uno uno.cpp
./uno
```

---

### 2. Using MSVC (`cl.exe`) on Windows

From a **Visual Studio Developer Command Prompt**:

#### Modular Version:
```cmd
cl /EHsc /std:c++14 /Fe:uno.exe Card.cpp Deck.cpp Player.cpp Game.cpp main.cpp
.\uno.exe
```

#### Single-File Version:
```cmd
cl /EHsc /std:c++14 /Fe:uno.exe uno.cpp
.\uno.exe
```

---

## Game Rules & Features

- **Players**: 2 to 10 players sharing one device in hot-seat mode.
- **Card Codes**:
  - **Colors**: `'R'` (Red), `'Y'` (Yellow), `'G'` (Green), `'B'` (Blue), `'W'` (Wild)
  - **Values**: `'0'`–`'9'`, `'S'` (Skip), `'R'` (Reverse), `'D'` (Draw Two), `'W'` (Wild), `'4'` (Wild Draw Four)
- **Official Mechanics**:
  - Starting card resolution (action cards take effect on the first turn).
  - 2-player Reverse acts as a Skip.
  - Wild Draw Four challenge mechanism with penalties.
  - Automatic reshuffle of the discard pile when the draw pile empties (preserving the active top card).
  - Sudden death victory: the first player to play their last card wins the match!
