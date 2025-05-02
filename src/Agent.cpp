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
int getBoardSize (const GameState& state) {
    return state.getSize();
}

// prefer middle columns, allows for more directions to win
ArrayList<int> orderedCol(const GameState& state) {
    ArrayList<int> order;
    int gridSize = getBoardSize(state);
    int center = gridSize / 2;
    order.append(center);
    for (int offset = 1; offset <= center; ++offset) {
        if (center - offset >= 0){
            order.append(center - offset);
        }
        if (center + offset < gridSize){
            order.append(center + offset);
        }
    }
    return order;
}

// check if spot is valid or not
bool validSpot(const GameState& state, int col) {
    int gridSize = getBoardSize(state);
    return col >= 0 && col < gridSize && state.buttonState(col) == emptyCell;
}

// simulate move, playing in first row possible
GameState simulateMove(GameState state, int col) {
    state.play(col);
    return state;
}

// checking for a streak in any given direction
bool checkLine (const GameState& state, int player, int intCol, int dx, int len) {
    int gridSize = getBoardSize(state);
        if (intCol < 0 || intCol >= gridSize || intCol + (len-1)*dx < 0 || intCol + (len-1)*dx >= gridSize) {
            return false;
        }

        for (int i = 0; i < len; ++i) {
            int col = intCol + i*dx;
            if (state.buttonState(col) != player){
                return false;
            }
        }
        return true;
    }

// counts win streak
int countStreak (const GameState& state, int player, int streakNum) {
    int count = 0;
    int gridSize = getBoardSize(state);

    for (int col = 0; col <= gridSize - streakNum; ++col) {
        if (checkLine(state, player, col, 1, streakNum)){
            count++;
        }
    }
    return count;
}

// evaluates function for minimax
int evaluate (const GameState& state){
    if (state.hasWon(AI_Agent)){
        return 1000;
    }
    if (state.hasWon(Human)){
        return -1000;
    }

    // count streaks of 2 and 3 for both colors
    int score = 0;
    score += countStreak(state, AI_Agent, 3) * 100;
    score += countStreak(state, AI_Agent, 2) * 10;
    score -= countStreak(state, Human, 3) * 100;
    score -= countStreak(state, Human, 2) * 10;

    // prefer for AI to take center columns by rewarding a higher point for center values
    int gridSize = getBoardSize(state);
    int center = gridSize/2;
    for (int col = 0; col < gridSize; col++){
        if (state.buttonState(col) == AI_Agent) {
            score += center - abs(col - center);
        }
        else if (state.buttonState(col == Human)) {
            score -= center - abs(col - center);
        }
    }

    return score;
}

// minimax function
int minimax(GameState state, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (depth == 0 || state.gameOver()) {
        return evaluate(state);
    }

    ArrayList<int> columns = orderedCol(state);

    if (maximizingPlayer) {
        int maxEval = inf_neg;
        for (int i = 0; i < columns.size(); i++) {
            int col = columns[i];
            if (validSpot(state, col)) {
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
        for (int i = 0; i <columns.size(); i++){
            int col = columns[i];
            if (validSpot(state, col)) {
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
    ArrayList<int> columns = orderedCol(state);

    int goalScore = inf_neg;
    int bestCol = -1;

    for (int i = 0; i < columns.size(); i++) {
        int col = columns[i];
        if (validSpot(state, col)) {
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