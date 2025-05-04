#include <GameState.h>

using namespace std;

GameState::GameState(int rows, int cols){

    cout << "Initializing to the game state with given rows and cols" << endl;

    this->rows = rows;

    this->cols = cols;

    board.clear();

    for (int i=0; i < rows*cols; i++) {

        board.append(-1);

    }

    currentTurn = 0;

    aiEnabled = false;

    lastMove = -1;

}

GameState::GameState():GameState(5,5) {

    cout << "making 5x5 board" << endl;

}

bool GameState::hasWon(int player) const{

    cout << "Checking whether player " << player << " has won" << endl;

    // row win check

    for (int row = 0; row < rows; row++) {

        for (int col = 0; col <= cols; col++) {

            bool win  = true;

            for (int i = 0; i < 4; i++) {

                if (board[row * cols + col + i] != player) {

                    win = false;

                    break;

                }

            }

            if (win == true){

                return true;

            }

        }

    }

    // col win check

    for (int col = 0; col < cols;  col++) {

        for (int row = 0;  row<= rows; row++) {

            bool win = true;

            for (int i = 0; i<4; i++) {

                if (board[(row+i) * cols + col] != player) {

                    win = false;

                    break;

                }

            }

            

            if (win == true){

                return true;

            }

        }

    }

    // 1st diagonals win check 

    for (int row = 0; row<=rows-4; row++){

        for (int col = 0; col <= cols; col++) {

            bool win = true;

            for (int i=0; i < 4; i++) {

                if (board[(row+i)* cols + col + i] != player){

                    win = false;

                    break;

                }

            }

            if (win == true){

                return true;

            }

        }

    }

    // 2nd diagonals win check

    for (int row = 3; row < rows; row++) {

        for (int col = 0; col <= cols - 4; col++) {

            bool win = true;

            for (int i=0; i<4; i++) {

                if (board[(row-i)*cols + col + i] != player) {

                    win = false;

                    break;

                }

            }

            if (win == true){

                return true;

            }

        }

    }

    // end case to guarantee a return

    return false;

}

int GameState::buttonState(int x) const{

    if (x < 0 || x >= board.size()) return -1;

    return board[x];

}

bool GameState::play(int x) {

    std::cout << "Current player makes move " << x << std::endl;

    if (x < 0 || x >= board.size() || board[x] != -1) {

        std::cout << "Invalid move: out of bounds or occupied." << std::endl;

        return false;

    }

    board[x] = currentTurn;

    currentTurn = !currentTurn;

    return true;

}

int GameState::getSize() const {

    cout << "Getting the size of the board" << endl;

    cout << "This may be represented as a single int, or as a Vec" << endl;

    return (rows < cols) ? rows : cols;

}

int GameState::getRows() const {

    return rows;

}

int GameState::getCols() const {

    return cols;

}

// resize board function

void GameState::resize (int tempRow, int tempCol) {

    rows = tempRow;

    cols = tempCol;

    board.clear();

    for (int i= 0; i < rows * cols; i++) {

        board.append(-1);

    }

    currentTurn = 0;

    lastMove = -1;

}

int GameState::getCurrentTurn() const {

    cout << "Whose turn is it?" << endl;

    return currentTurn;

}

bool GameState::gameOver() const {

    cout << "Checking if the game is over" << endl;

    // game over due to win

    if (hasWon(0) || hasWon(1)){

        return true;

    }

    // game over due to full board with no win

    for (int i=0; i< board.size(); i++){

        if (board[i] == -1) {

            return false;

        }

    }

    return true;

}

int GameState::getLastMove() const {

    cout << "Getting the last move that was made" << endl;

    cout << "Should have been updated during play()" << endl;

    return lastMove;

}

void GameState::reset(){
    currentTurn = 0;
    for (int i = 0; i < board.size(); i++) {
        board[i] = -1;
    }
    for (int i = board.size(); i < 10 * 7; i++) {
        board.append(-1);
    }
 
    std::cout << "Board has been reset." << std::endl;  
}

void GameState::enableAI(){

    cout << "Turning on the AI" << endl;

    aiEnabled = true;

}

void GameState::disableAI(){

    cout << "Turning off the AI" << endl;

    aiEnabled = false;

}

bool GameState::getEnabledAI() const {

    cout << "Is the AI turned on" << endl;

    return aiEnabled;

}