@echo off
setlocal enabledelayedexpansion

echo ====================================
echo Building UNO (C++)
echo ====================================

where cl >nul 2>nul
if %ERRORLEVEL% equ 0 (
    echo Using MSVC cl...
    cl /EHsc /std:c++14 /W4 /Fe:uno.exe Card.cpp Deck.cpp Player.cpp Game.cpp main.cpp
    cl /EHsc /std:c++14 /W4 /Fe:test_runner.exe Card.cpp Deck.cpp Player.cpp Game.cpp test_runner.cpp
    goto done
)

if exist "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" (
    echo Initializing MSVC BuildTools environment...
    call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
    cl /EHsc /std:c++14 /W4 /Fe:uno.exe Card.cpp Deck.cpp Player.cpp Game.cpp main.cpp
    cl /EHsc /std:c++14 /W4 /Fe:test_runner.exe Card.cpp Deck.cpp Player.cpp Game.cpp test_runner.cpp
    goto done
)

where g++ >nul 2>nul
if %ERRORLEVEL% equ 0 (
    echo Using g++...
    g++ -std=c++14 -Wall -Wextra -O2 -o uno Card.cpp Deck.cpp Player.cpp Game.cpp main.cpp
    g++ -std=c++14 -Wall -Wextra -O2 -o test_runner Card.cpp Deck.cpp Player.cpp Game.cpp test_runner.cpp
    goto done
)

echo ERROR: Neither cl.exe nor g++ found. Please run within Developer Command Prompt or install MinGW.
exit /b 1

:done
echo Build finished successfully!
echo Run 'uno.exe' to play, or 'test_runner.exe' to run tests.

