#include "Queue.h"
#include "Vertex.h"
#include "Graph.h"
#include <iostream>
#include <Agent.h>
#include "ArrayList.h"

using namespace std;

const int AI_Agent = 1;
const int Human = 0;
const int max_depth = 4;
const int inf_pos = 100000;
const int inf_neg = -100000;

struct GameTreeNode{
    Vertex<GameState>* vertex;
    int depth;

    GameTreeNode(Vertex<GameState>* vertex = nullptr, int depth = 0){
        this->vertex = vertex;
        this->depth = depth;
    }
};

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

    bool validSpot(GameState& state, int col) {
        for (int row = state.gridSize() - 1; row >= 0; row--){
            if (state.squareState(col, row) == "") {
                return true;
            }
        }
        return false;
    }

    GameState simulateMove(GameState state, int col) {
        for (int row = state.gridSize()-1; row >= 0; row--){
            if (state.squareState(col, row) == "") {
                state.play(col,row);
                break;
            }
        }
        return state;
    }

    bool checkLine (GameState& s, int p, int x, int y, int dx, int dy, int len, int sz) {
            for (int i = 0; i < len; ++i){
                int nx = x+i*dx;
                int ny = y+i*dy;
                if (nx < 0 || ny < 0 || nx >= sz){
                    return false;
                }
                if (s.squareState(nx, ny) != s.playerStr(p)){
                    return false;
                }
            }
            return true;
        }

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

    int evaluate (GameState& state, int player){
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
    }

    int minimax(Vertex<GameState>* node, int depth, int x, int y, bool maximizingPlayer, int player) {
        GameState& state = node-> data;

        if (depth == 0 || state.gameOver() || node->neighbors.size() == 0) {
            return evaluate(state, player);
        }

        if (maximizingPlayer) {
            int maxEval = inf_neg;
            for (int i = 0; i < node->neighbors.size(); i++) {
                int eval = minimax (node->neighbors[i]-> location, depth - 1, x, y, false, player);
                if (eval > maxEval) {
                    maxEval = eval;
                }
                if (eval > x) {
                    x = eval;
                }
                if (y <= x) {
                    break;
                }
            }
            return maxEval;
        }
        else {
            int minEval = inf_pos;
            for (int i = 0; i < node->neighbors.size(); i++){
                int eval = minimax(node->neighbors[i]-> location, depth-1, x, y, true, player){
                    if (eval < minEval){
                        minEval = eval;
                    }
                    if (eval < y) {
                        y = eval;
                    }
                    if (y <= x) {
                        break;
                    }
                }
            }
            return minEval;
        }
    }

Vec Agent::play(GameState state){
    Vertex<GameState>* root = new Vertex<GameState>(state);
    Graph<GameState> gameSpace;
    gameSpace.addVertex(root);

    Queue<GameTreeNode> frontier;
    frontier.enqueue(GameTreeNode(root, 0));

    int limit = 3;
    while (!frontier.isEmpty()){
        GameTreeNode gtn = frontier.dequeue();
        Vertex<GameState>* node = gtn.vertex;
        int depth = gtn.depth;
        ArrayList<Vec> moves = openSquares(node->data);

        if (state.gridSize() == 3){
            limit = 6;
        }
        else{
            if (moves.size() < 15){
                limit = 5;
            }
        }
        if (depth < limit){
            for (int i = 0; i < moves.size(); i++){
                GameState currentState = node->data;
                currentState.play(moves[i].x, moves[i].y);
                Vertex<GameState>* child = new Vertex<GameState>(currentState);
                gameSpace.addVertex(child);
                gameSpace.addDirectedEdge(node, child, 1);
                frontier.enqueue(GameTreeNode(child, depth + 1));
            }
        }
    }

    int reward = getReward(root->neighbors[0]->location, 1);
    int pos = 0;
    for (int i = 1; i < root->neighbors.size(); i++){
        int curr = getReward(root->neighbors[i]->location, 1);
        if (curr > reward){
            reward = curr;
            pos = i;
        }
        else if (curr == reward && root->neighbors[i]->location->data.hasWon(1)){
            reward = curr;
            pos = i;
        }
    }

    return root->neighbors[pos]->location->data.getLastMove();
}