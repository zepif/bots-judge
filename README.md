# bots-judge

This is a C++ program designed to run matches between different game bots. It provides a safe environment for running third-party programs and evaluates their performance.

## Overview

The program runs programs as child processes and ensures their interaction through interprocess communication channels (pipes). It controls the progress of the game, determines the winner or tie, and outputs the results.

## Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/zepif/bots-judge
    ```

2. Navigate to the project directory:
    ```sh
    cd bots-judge/src
    ```

3. Build the project using `make`:
    ```sh
    make
    ```

## Usage

For detailed usage instructions, see the [example](https://github.com/zepif/bots-judge/tree/main/example/rsp).
