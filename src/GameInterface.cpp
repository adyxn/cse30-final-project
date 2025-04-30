#include <FL/Fl_Window.H>
#include <GameInterface.h>
#include <Agent.h>
#include <bobcat_ui/bobcat_ui.h>
#include <bobcat_ui/textbox.h>

using namespace std;
using namespace bobcat;

GameInterface::GameInterface(int x, int y, int w, int h, GameState initialState){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    
    state = initialState;

    string message = "Player vs Player";
    if (state.getEnabledAI()){
        message = "Player vs AI";
    }

    statusBar = new TextBox(0, 375, 400, 25, message);

    initButtons();
    showButtons();
}

void GameInterface::handleClick(Widget *sender){
   // Loop through column buttons to see which was clicked
    for (int col = 0; col < columnButtons.size(); col++) {
        if (sender == columnButtons[col]) {
            bool moveSuccess = state.play(col);
            if (!moveSuccess) return;  // Column full
               updateButtons();

            if (checkWinningConditions()) return;

            if (state.getEnabledAI()) {
                Vec aiMove = Agent::play(state);  // Agent returns (row, col) or just col
                state.play(aiMove.y);  // assuming Agent returns Vec(x=row, y=col)
                updateButtons();

                checkWinningConditions();
            }

            return;
        }
    }
}
    
bool GameInterface::checkWinningConditions(){
    bool result = state.gameOver();
    if (state.gameOver()){
        if (state.hasWon(0)){
            showMessage("Player X has won.\nClick Close to start a new game.", "Game Over");
        }
        else if (state.hasWon(1)){
            showMessage("Player O has won.\nClick Close to start a new game.", "Game Over");
        }
        else{
            showMessage("It is a tie.\nClick Close to start a new game.", "Game Over");
        }
        reset();
    }
    return result;
}

void GameInterface::initButtons() {
    int cols = 7;
    int rows = 6;
    int cellW = w / cols;
    int cellH = (h - 100) / rows;

    for (int col = 0; col < cols; col++) {
        int xCoord = x + col * cellW;
        int yCoord = y;

        auto* dropBtn = new bobcat::Button(xCoord, yCoord, cellW, 40, "↓");
        dropBtn->labelsize(24);
        dropBtn->box(FL_ROUND_UP_BOX);
        ON_CLICK(dropBtn, GameInterface::handleClick);
        columnButtons.append(dropBtn);
    }

    for (int row = 0; row < rows; row++) {
        ArrayList<Button*> rowButtons;
        for (int col = 0; col < cols; col++) {
            int xCoord = x + col * cellW;
            int yCoord = y + 50 + row * cellH;

            auto* gridBtn = new bobcat::Button(xCoord, yCoord, cellW, cellH, "");
            gridBtn->labelsize(20);
            gridBtn->deactivate();
            gridBtn->box(FL_DOWN_BOX);
            rowButtons.append(gridBtn);
        }
        boardGrid.append(rowButtons);
    }
}


void GameInterface::showButtons() {
    for (int i = 0; i < columnButtons.size(); i++) {
        columnButtons[i]->show();
    }

    for (int i = 0; i < boardGrid.size(); i++) {
        for (int j = 0; j < boardGrid[i].size(); j++) {
            boardGrid[i][j]->show();
        }
    }
}


void GameInterface::hideButtons() {
    for (int i = 0; i < columnButtons.size(); i++) {
        columnButtons[i]->hide();
    }

    for (int i = 0; i < boardGrid.size(); i++) {
        for (int j = 0; j < boardGrid[i].size(); j++) {
            boardGrid[i][j]->hide();
        }
    }
}


void GameInterface::updateButtons() {
    for (int row = 0; row < boardGrid.size(); row++) {
        for (int col = 0; col < boardGrid[row].size(); col++) {
            std::string mark = state.squareState(row, col);
            boardGrid[row][col]->label(mark);
        }
    }
}


void GameInterface::show() {
    showButtons();
    statusBar->show();
}

void GameInterface::hide() {
    hideButtons();
    statusBar->hide();
}

void GameInterface::reset() {
    state.reset();
    updateButtons();
}

GameState GameInterface::getState() const{
    return state;
}

void GameInterface::setState(GameState state){
    this->state = state;
    string message = "Player vs Player";
    if (state.getEnabledAI()){
        message = "Player vs AI";
    }
    statusBar->label(message);
}
