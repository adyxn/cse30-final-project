#include <GameState.h>

using namespace std;
GameState::GameState(){
    cout << "Initializing to the default state" << endl;

    for (int i = 0; i < 6 * 7; i++){
        board.append(-1);
    }

    currentTurn = 0;
}


bool GameState::hasWon(int player) const{
    cout << "Checking whether player " << player << " has won" << endl;
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
    return 0;
}


int GameState::getCurrentTurn() const {
    cout << "Whose turn is it?" << endl;
    return currentTurn;
}


bool GameState::gameOver() const {
    cout << "Checking if the game is over" << endl;
    return false;
}

int GameState::getLastMove() const {
    cout << "Getting the last move that was made" << endl;
    cout << "Should have been updated during play()" << endl;
    return 0;
}


void GameState::reset(){
    
}

void GameState::enableAI(){
    cout << "Turning on the AI" << endl;
}

void GameState::disableAI(){
    cout << "Turning off the AI" << endl;
}

bool GameState::getEnabledAI() const {
    cout << "Is the AI turned on" << endl;
    return false;
}



