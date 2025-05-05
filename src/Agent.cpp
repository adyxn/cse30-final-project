#include "GameState.h"
#include <iostream>
#include <Agent.h>
#include "ArrayList.h"

using namespace std;

const int AI_Agent = 1; // blue player
const int Human = 0; // red player
const int emptyCell = -1;
const int max_depth = 4;
const int inf_pos = 100000;
const int inf_neg = -100000;

// get board size, allows for more dynamic function
int getRows(const GameState& state){
    return state.getRows();
}

int getCols(const GameState& state){
    return state.getCols();
}

// prefer middle columns, allows for more directions to win
ArrayList<int> orderedCol(const GameState& state) {
    ArrayList<int> order;
    int cols = getCols(state);
    int center = cols / 2;
    order.append(center);
    for (int offset = 1; offset <= center; ++offset) {
        if (center - offset >= 0){
            order.append(center - offset);
        }
        if (center + offset < cols){
            order.append(center + offset);
        }
    }
    return order;
}

int findValidRows(const GameState& state, int col) {
    int rows = getRows(state);
    int cols = getCols(state);

    for (int row = rows -1; row >= 0; row--){
        int flatIndex = row * cols + col;
        if (state.buttonState(flatIndex) == emptyCell) {
            return row;
        }
    }
    return -1; // safe return statement if col is full
}

int findValidCols(const GameState&state, int col){
    int cols = state.getCols();

    if (col < 0 || col >= cols) {
        return false;
    }

    int flatIndex = 0*cols + col;
    return state.buttonState(flatIndex) == emptyCell;
}

// simulate move in given col
GameState simulateMove(GameState state, int col) {
    int rows = state.getRows();
    int cols = state.getCols();

    for (int row = rows -1; row >= 0; row--) {
        int flatIndex = row * cols + col;
        if (state.buttonState(flatIndex) == emptyCell) {
            state.play(flatIndex);
            break;
        }
    }

    return state;
}

// evaluates function for minimax
int evaluate (const GameState& state){
    int rows = state.getRows();
    int cols = state.getCols();

    // win conditions
    if (state.hasWon(AI_Agent)){
        return 1000;
    }
    if (state.hasWon(Human)){
        return -1000;
    }

    int score = 0;


    // scores for horizontal
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols -4 ; col++) {
            int aiCount = 0;
            int humanCount = 0;
            int emptyCount = 0;

            for (int i=0; i<4; i++) {
                int cellValue  = state.buttonState(row*cols + col + i);
                if (cellValue == AI_Agent) {
                    aiCount++;
                }
                else if (cellValue == Human) {
                    humanCount++;
                }
                else {
                    emptyCount++;
                }
            }

            if (humanCount == 0) {
                if (aiCount == 3) {
                    score += 50;
                }
                else if (aiCount == 2) {
                    score += 20;
                }
                else if (aiCount == 1){
                    score += 10;
                }
            }
            if (aiCount == 0) {
                if (humanCount == 3) {
                    score -= 50;
                }
                else if (humanCount == 2) {
                    score -= 20;
                }
                else if (humanCount == 1){
                    score -= 10;
                }
            }
        }
    }

    // scores for vertical
    for (int col = 0; col < cols; col++) {
        for (int row = 0; row <= rows - 4; row++) {
            int aiCount = 0;
            int humanCount = 0;
            int emptyCount = 0;

            for (int i=0; i<4; i++){
                int cellValue = state.buttonState((row+i)*cols + col);
                if (cellValue == AI_Agent) {
                    aiCount++;
                }
                else if (cellValue == Human) {
                    humanCount++;
                }
                else {
                    emptyCount++;
                }
            }

            if (humanCount == 0) {
                if (aiCount == 3) {
                    score += 50;
                }
                else if (aiCount == 2) {
                    score += 20;
                }
                else if (aiCount == 1){
                    score += 10;
                }
            }
            if (aiCount == 0) {
                if (humanCount == 3) {
                    score -= 50;
                }
                else if (humanCount == 2) {
                    score -= 20;
                }
                else if (humanCount == 1){
                    score -= 10;
                }
            }
        }
    }

    // scores for diag 1
    for (int row = 0; row <= rows-4; row++) {
        for (int col = 0; col <= cols; col++) {
            int aiCount = 0; 
            int humanCount = 0;
            int emptyCount = 0;

            for (int i=0; i <4; i++) {
                int cellValue = state.buttonState((row+i)*cols + col + i);
                if (cellValue == AI_Agent) {
                    aiCount++;
                }
                else if (cellValue == Human) {
                    humanCount++;
                }
                else {
                    emptyCount++;
                }
            }

            if (humanCount == 0) {
                if (aiCount == 3) {
                    score += 50;
                }
                else if (aiCount == 2) {
                    score += 20;
                }
                else if (aiCount == 1){
                    score += 10;
                }
            }
            if (aiCount == 0) {
                if (humanCount == 3) {
                    score -= 50;
                }
                else if (humanCount == 2) {
                    score -= 20;
                }
                else if (humanCount == 1){
                    score -= 10;
                }
            }
        }
    }

    // scores for diag 2
    for (int row = 3; row <= rows; row++) {
        for (int col = 0; col <= cols-4 ; col++) {
            int aiCount = 0; 
            int humanCount = 0;
            int emptyCount = 0;

            for (int i=0; i <4; i++) {
                int cellValue = state.buttonState((row-i)*cols + col);
                if (cellValue == AI_Agent) {
                    aiCount++;
                }
                else if (cellValue == Human) {
                    humanCount++;
                }
                else {
                    emptyCount++;
                }
            }

            if (humanCount == 0) {
                if (aiCount == 3) {
                    score += 50;
                }
                else if (aiCount == 2) {
                    score += 20;
                }
                else if (aiCount == 1){
                    score += 10;
                }
            }
            if (aiCount == 0) {
                if (humanCount == 3) {
                    score -= 50;
                }
                else if (humanCount == 2) {
                    score -= 20;
                }
                else if (humanCount == 1){
                    score -= 10;
                }
            }
        }
    }

    // prefer for AI to take center columns by rewarding a higher point for center values
    int center = cols/2;
    for (int row=0; row < rows; row++){
        for (int col = 0; col<cols; col++){
            int flatIndex = row*cols + col;
            if (state.buttonState(flatIndex) == AI_Agent) {
                score += 15 - min (3, abs(col-center));
            }
        }
    }

    return score;
}

// minimax function
int minimax(GameState state, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (depth == 0 || state.gameOver()) {
        return evaluate(state);
    }

    int cols = getCols(state);

    if (maximizingPlayer) {
        int maxEval = inf_neg;
        for (int col = 0; col < cols; col++) {
            if (findValidCols(state, col)) {
                GameState child = simulateMove(state, col);
                int eval = minimax(child, depth-1, alpha, beta, false);
                maxEval = max(maxEval, eval);
                alpha = max(alpha, eval);
                if (beta <= alpha) {
                    break;
                }

            }
        }
        return maxEval;
    }
    else
    {
        int minEval = inf_pos;
        for (int col = 0; col < cols; col++){
            if (findValidCols(state, col)) {
                GameState child = simulateMove(state, col);
                int eval = minimax(child, depth-1, alpha, beta, true);
                minEval = min(minEval, eval);
                beta = min(beta, eval);
                if (beta <= alpha){
                    break;
                }
            }
        }
        return minEval;
    }
}

// AI_Agent's move function
int Agent::play(GameState state){
    int cols = getCols(state);
    int goalScore = inf_neg;
    int bestCol = -1;

    // winning move check
    for (int col = 0; col < cols; col++) {
        if (findValidCols(state, col)) {
            GameState newState = simulateMove(state, col);
            if (newState.hasWon(AI_Agent)) {
                return col;
            }
        }
    }

    // losing move check
    for (int col = 0; col < cols; col++) {
        if (findValidCols(state, col)) {
            int row = -1;
            for (int r = getRows(state) -1; r>= 0; r--) {
                int flatIndex = r*col + col;
                if (state.buttonState(flatIndex) == emptyCell) {
                    row = r;
                    break;
                }
            }
            
            if (row != -1) {
                GameState tempState = state;
                int currentTurn = tempState.getCurrentTurn();

                if (currentTurn != Human) {
                    int testRow = -1;
                    int testCol = -1;
                    for (int x = 0; x < getRows(tempState); x++) {
                        for (int y = 0; y < getCols(tempState); y++) {
                            if (tempState.buttonState(x*cols + y) != emptyCell) {
                                testRow = x;
                                testCol = y;
                                break;
                            }
                        }
                        if (testRow != -1) {
                            break;
                        }
                        if (testRow == -1) {
                            GameState testState = state;
                            int flatIndex = row*cols + col;
                            testState.play(flatIndex);
                            if (testState.hasWon(Human)) {
                                return col;
                            }
                            continue;
                        }
                    }
                    int flatIndex = row*cols + col;
                    tempState.play(flatIndex);
                    if (tempState.hasWon(Human)) {
                        return col;
                    }
                }
            }
        }
    }

    for (int col = 0; col < cols; col++) {
        if (findValidCols(state, col)) {
            GameState nextState = simulateMove(state, col);
            int score = minimax(nextState, max_depth-1, inf_neg, inf_pos, false);

            if (score > goalScore) {
                goalScore = score;
                bestCol = col;
            }
        }
    }
    return bestCol;
}