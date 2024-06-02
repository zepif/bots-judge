# Rock-Paper-Scissors Engine

This is an example of a simple Rock-Paper-Scissors (RPS) game engine that can be used with bots-judge for matches between different bots or engines.

## Overview

The RPS Engine game engine is implemented in C++ and matches the interaction protocol used by bots-judge. It conducts rounds between two players, determining a winner or a tie after a given number of rounds.

## Bots

This example provides several bots for demonstration purposes:

- **botscissors.cpp**: This bot always chooses "scissors" (SCISSORS) as its move.
- **botrock.cpp**: This bot always chooses "rock" (ROCK) as its move.
- **botrandom.cpp**: This bot chooses a random move ("rock", "scissors" or "paper") each round. It uses the standard `rand()` algorithm with the ability to set the initial seed value via a command line argument.
- **botnoop.cpp**: This bot does not choose anything and serves to check if the engine works correctly in different situations.

## Usage

1. Compile the engine and bot source files:
    ```sh
    make
    ```

2. Ensure that the engine (`rsp_engine`) and bot executables (`botscissors`, `botrandom`) are in your system `PATH` variable so that bots-judge can find and run them.
    For example:
    ```sh
    export PATH=$PATH:$PWD
    ```

3. Run bots-judge, passing the paths to the bot executables as arguments:
    ```sh
    ./rsp_engine random scissors
    ```
    This will start a match between the `botscissors` bot and the random bot `botrandom`. bots-judge will output the results of the match to the console.

## Creating a Custom Game Engine

To create a custom game engine that is compatible with bots-judge, you need to implement the `play_game` function according to the interaction protocol it uses. The `play_game` function accepts a `PlayerData` vector and must return a `GameResult` that describes the result of the game.

Here are the basic steps to create a new game engine:

1. **Define the game logic**: Define the rules of your game, the various possible moves or states, win, draw, and lose conditions.

2. **Create a source code file**: Create a new C++ source code file (e.g., `my_game_engine.cpp`) and include the necessary header files, such as `engine.h`.

3. **Implement the game logic**: Define the classes or data structures needed to represent the various game states, moves, or other game objects. Implement functions or methods to handle game logic, such as determining a winner, checking the validity of moves, etc.

4. **Implement the `play_game` function**: Create an implementation of the `play_game` function that will control the flow of the game. It should interact with players (bots) via playerstreams, receive their moves, apply game logic, and determine the outcome of the game.
    - The `play_game` function must return a `GameResult`, which can be one of the following:
        - `GameResult::createWin(players, winner, details)`: If one of the players won, pass the `PlayerData` vector, the winner, and additional information (if needed) as arguments.
        - `GameResult::createDraw(players, details)`: If the game ended in a tie, pass the `PlayerData` vector and additional information (if needed).
        - `GameResult::createError(players, error_details)`: If an engine error occurred, pass the `PlayerData` vector and a description of the error.

5. **Add error handling**: Handle possible exceptions or errors that may occur during the game, such as incorrect player moves or I/O errors.

6. **Compile and use**: Compile your game engine and make sure the executable is in the system `PATH` variable or pass the full path to it when you run bots-judge.

Here is an example of the basic code structure for the new game engine `my_game_engine.cpp`:

```cpp
#include "engine.h"
#include <vector>
#include <string>
#include <memory>
// Other necessary header files

namespace Engine {

// Define classes or data structures to represent game objects

// Implement functions or methods to handle game logic

GameResult play_game(std::vector<PlayerData>& players) noexcept {
    try {
        // Check the number of players
        if (players.size() != EXPECTED_PLAYERS) {
            return GameResult::createError(players, "Invalid number of players");
        }

        // Main game loop
        for (int round = 0; round < MAX_ROUNDS; ++round) {
            // Receiving moves from players
            // ...

            // Applying game logic
            // ...

            // Determining the result of the round
            // ...
        }

        // Determining the final result of the game
        if (win_condition) {
            return GameResult::createWin(players, winner, "Additional information");
        } else if (condition_draw) {
            return GameResult::createDraw(players, "Additional information");
        } else {
            // Handling other cases
        }
    } catch (const std::exception& e) {
        return GameResult::createError(players, e.what());
    }
}

} // namespace Engine
```

This example demonstrates the basic structure of a game engine. You need to fill it with real game logic, define classes or data structures to represent game objects, implement functions to process players' moves, apply game rules, and determine the result.

Don't forget to also implement bots or game clients that will interact with your game engine through I/O streams (std::cin and std::cout), according to the protocol used by bots-judge.
