#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "ArrayList.h"
#include <iostream>

class GameState{
    // Variables to represent state...
    // These can include the size of the game
    // Representation of the board
    // Whose turn it is
    // Whether AI is enabled
    // -1 unoccupied
    // 0 player 0
    // 1 player 1
    ArrayList<int> board;
    bool currentTurn;
    int rows;
    int cols;
    int lastMove;
    bool aiEnabled;

public:
    // Initialize default game state
    GameState();                    // May take in parameters if necessary

    GameState(int rows, int cols);

    int getSize() const;            // What is the size of the game
    int getRows() const;
    int getCols() const;
    void resize(int tempRow, int tempCol);

    int getCurrentTurn() const;     // Whose turn is it?

    int buttonState(int x) const;

    bool play(int x);               // Update state resulting from player making move

    bool hasWon(int player) const;  // Has a given player won?

    bool gameOver() const;          // Is it Game Over?

    void enableAI();                // Turn on the AI

    void disableAI();               // Turn off the AI

    bool getEnabledAI() const;      // Is AI currently on?

    int getLastMove() const;        // What was the last move?

    void reset();                   // Reset the state to its default (may take parameters)

    friend std::ostream& operator<<(std::ostream& os, const GameState& state);
};

inline std::ostream& operator<<(std::ostream& os, const GameState& state){
    os << "Printing the game state";

    return os;
}

#endif