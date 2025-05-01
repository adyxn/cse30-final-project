#include "GameState.h"
#include "Queue.h"
#include "Vertex.h"
#include "Graph.h"
#include <iostream>
#include <Agent.h>
#include <pthread.h>
#include "ArrayList.h"

using namespace std;

const int AI_Agent = 1;
const int Human = 0;
const int max_depth = 4;
const int inf_pos = 100000;
const int inf_neg = -100000;
std::string playerSymbol(int player) {
    return (player == AI_Agent) ? "Red" : "Blue";
}

// prefer middle columns
ArrayList<int> orderedCol(int gridSize) {
    ArrayList<int> order;
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
bool validSpot(GameState& state, int col) {
    for (int row = state.gridSize() - 1; row >= 0; row--){
        if (state.squareState(col, row) == "") {
            return true;
        }
    }
    return false;
}

// simulate move, playing in first row possible
GameState simulateMove(GameState state, int col) {
    for (int row = state.gridSize()-1; row >= 0; row--){
        if (state.squareState(col, row) == "") {
            state.play(col,row);
            break;
        }
    }
    return state;
}

// checking for a line, a streak of len at x,y in dx,dy directions
bool checkLine (GameState& s, int p, int x, int y, int dx, int dy, int len, int sz) {
        for (int i = 0; i < len; ++i){
            int nx = x+i*dx;
            int ny = y+i*dy;
            if (nx < 0 || ny < 0 || nx >= sz){
                return false;
            }
            if (s.squareState(nx, ny) != playerSymbol(p)){
                return false;
            }
        }
        return true;
    }

// counts win streak
int countStreak (GameState& state, int player, int streakNum) {
    int count = 0;
    int size = state.gridSize();

    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            if (checkLine(state, player, x, y, 1, 0, streakNum, size)){
                count++;
            }
            if (checkLine(state, player, x, y, 0, 1, streakNum, size)){
                count++;
            }
            if (checkLine(state, player, x, y, 1, 1, streakNum, size)){
                count++;
            }
            if (checkLine(state, player, x, y, 1, -1, streakNum, size)){
                count++;
            }
        }
    }
    return count;
}

// evaluates function for minimax
int evaluate (GameState& state){
    if (state.hasWon(AI_Agent)){
        return 1000;
    }
    if (state.hasWon(Human)){
        return -1000;
    }

    int score = 0;
    score += countStreak(state, AI_Agent, 3) * 100;
    score += countStreak(state, AI_Agent, 2) * 10;
    score -= countStreak(state, Human, 3) * 100;
    score -= countStreak(state, Human, 2) * 10;

    return score;
}

// minimax function
int minimax(GameState state, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (depth == 0 || state.gameOver()) {
        return evaluate(state);
    }

    int gridSize = state.gridSize();
    ArrayList<int> columns = orderedCol(gridSize);

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
Vec Agent::play(GameState state){
    int gridSize = state.gridSize();
    ArrayList<int> columns = orderedCol(gridSize);

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

    for (int row = gridSize-1; row >= 0; row--){
        if (state.squareState(bestCol, row) == ""){
            return Vec(bestCol,row);
        }
    }
}