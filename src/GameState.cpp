#include <GameState.h>

using namespace std;

GameState::GameState(int size) {
    this->size = 7;  // 7 columns
    currentTurn = 0;
    turnCount = 0;
    done = false;
    enabledAI = false;
    lastMove.set(-1, -1);

    grid = new int*[6];  // 6 rows
    for (int i = 0; i < 6; i++) {
        grid[i] = new int[7];
        for (int j = 0; j < 7; j++) {
            grid[i][j] = -1;
        }
    }
}

GameState::GameState(const GameState& other) {
    size = other.size;
    currentTurn = other.currentTurn;
    turnCount = other.turnCount;
    done = other.done;
    enabledAI = other.enabledAI;
    lastMove = other.lastMove;

    grid = new int*[6];
    for (int i = 0; i < 6; i++) {
        grid[i] = new int[7];
        for (int j = 0; j < 7; j++) {
            grid[i][j] = other.grid[i][j];
        }
    }
}

bool GameState::operator==(const GameState& other) {
    if (size != other.size || currentTurn != other.currentTurn ||
        turnCount != other.turnCount || done != other.done ||
        enabledAI != other.enabledAI ||
        lastMove.x != other.lastMove.x || lastMove.y != other.lastMove.y)
        return false;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            if (grid[i][j] != other.grid[i][j]) return false;
        }
    }
    return true;
}

GameState& GameState::operator=(const GameState& other) {
    if (this == &other) return *this;

    currentTurn = other.currentTurn;
    turnCount = other.turnCount;
    done = other.done;
    enabledAI = other.enabledAI;
    lastMove = other.lastMove;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            grid[i][j] = other.grid[i][j];
        }
    }
    return *this;
}

bool GameState::play(int col) {
    if (col < 0 || col >= 7) return false;

    for (int row = 5; row >= 0; row--) {
        if (grid[row][col] == -1) {
            grid[row][col] = currentTurn;
            lastMove.set(row, col);
            turnCount++;
            currentTurn = !currentTurn;

            if (turnCount == 42 || hasWon(0) || hasWon(1)) {
                done = true;
            }
            return true;
        }
    }
    return false;
}

bool GameState::play(int x, int y) {
    if (x < 0 || x >= 6 || y < 0 || y >= 7) return false;
    if (grid[x][y] != -1) return false;

    grid[x][y] = currentTurn;
    lastMove.set(x, y);
    turnCount++;
    currentTurn = !currentTurn;

    if (turnCount == 42 || hasWon(0) || hasWon(1)) {
        done = true;
    }
    return true;
}

bool GameState::hasWon(int player) {
    // Horizontal
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col <= 3; col++) {
            if (grid[row][col] == player && grid[row][col+1] == player &&
                grid[row][col+2] == player && grid[row][col+3] == player)
                return true;
        }
    }

    // Vertical
    for (int col = 0; col < 7; col++) {
        for (int row = 0; row <= 2; row++) {
            if (grid[row][col] == player && grid[row+1][col] == player &&
                grid[row+2][col] == player && grid[row+3][col] == player)
                return true;
        }
    }

    // Diagonal (\)
    for (int row = 0; row <= 2; row++) {
        for (int col = 0; col <= 3; col++) {
            if (grid[row][col] == player && grid[row+1][col+1] == player &&
                grid[row+2][col+2] == player && grid[row+3][col+3] == player)
                return true;
        }
    }

    // Diagonal (/)
    for (int row = 3; row < 6; row++) {
        for (int col = 0; col <= 3; col++) {
            if (grid[row][col] == player && grid[row-1][col+1] == player &&
                grid[row-2][col+2] == player && grid[row-3][col+3] == player)
                return true;
        }
    }

    return false;
}

bool GameState::gameOver() const {
    return done;
}

int GameState::gridSize() const {
    return 7; // Number of columns
}

string GameState::squareState(int i, int j) const {
    if (grid[i][j] == 0) return "X";
    if (grid[i][j] == 1) return "O";
    return "";
}

void GameState::enableAI() {
    enabledAI = true;
}

void GameState::disableAI() {
    enabledAI = false;
}

bool GameState::getEnabledAI() const {
    return enabledAI;
}

int GameState::getCurrentTurn() const {
    return currentTurn;
}

Vec GameState::getLastMove() const {
    return lastMove;
}

void GameState::reset() {
    currentTurn = 0;
    turnCount = 0;
    done = false;
    lastMove.set(-1, -1);
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            grid[i][j] = -1;
        }
    }
}

GameState::~GameState() {
    for (int i = 0; i < 6; i++) {
        delete[] grid[i];
    }
    delete[] grid;
}
