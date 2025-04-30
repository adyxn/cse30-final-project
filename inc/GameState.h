#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <iostream>

struct Vec {
    int x;
    int y;

    Vec() {
        x = 0;
        y = 0;
    }

    Vec(int x, int y) {
        this->x = x;
        this->y = y;
    }

    void set(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Vec& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}



class GameState {
    int** grid;
    bool currentTurn;
    int size;
    int turnCount;

    bool enabledAI;
    bool done;
    Vec lastMove;

public:
    GameState(int size = 3);
    GameState(const GameState& other);
    ~GameState();

    bool operator==(const GameState& other);
    GameState& operator=(const GameState& other);

    bool play(int col);           // ADDED: Single-column play for Connect Four
    bool play(int x, int y);      // Legacy / manual play support

    bool hasWon(int player);
    bool gameOver() const;

    int gridSize() const;
    std::string squareState(int i, int j) const;

    void enableAI();
    void disableAI();
    bool getEnabledAI() const;

    int getCurrentTurn() const;
    Vec getLastMove() const;

    void reset();

    friend std::ostream& operator<<(std::ostream& os, const GameState& state);
};

inline std::ostream& operator<<(std::ostream& os, const GameState& state) {
    os << "   ";
    for (int j = 0; j < state.size; j++) {
        os << " " << j << "  ";
    }
    os << std::endl;
    os << "   ";
    for (int j = 0; j < state.size; j++) {
        os << "--- ";
    }
    os << std::endl;
    for (int i = 0; i < 6; i++) {
        os << i << " ";
        for (int j = 0; j < 7; j++) {
            char c = ' ';
            if (state.grid[i][j] == 0) c = 'X';
            else if (state.grid[i][j] == 1) c = 'O';
            os << "| " << c << " ";
            if (j == 6) os << "|";
        }
        os << std::endl << "   ";
        for (int j = 0; j < state.size; j++) {
            os << "--- ";
        }
        os << std::endl;
    }

    return os;
}

#endif
