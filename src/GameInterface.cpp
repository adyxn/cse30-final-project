#include <FL/Enumerations.H>

#include <GameInterface.h>

#include <Agent.h>

#include <bobcat_ui/bobcat_ui.h>

#include <bobcat_ui/button.h>

#include <bobcat_ui/textbox.h>

using namespace std;

using namespace bobcat;

GameInterface::GameInterface(int x, int y, int w, int h, GameState initialState){

    this->x = x;

    this->y = y;

    this->w = w;

    this->h = h;

    

    state = initialState;

    int rows = state.getRows();

    int cols = state.getCols();

    makeButtonGrid(rows, cols);

    string message = "Player vs Player";

    if (state.getEnabledAI()){

        message = "Player vs AI";

    }

    statusBar = new TextBox(x, h-25 + y, w, 25, message);

    statusBar->align(FL_ALIGN_CENTER);

}

void GameInterface::makeButtonGrid(int rows, int cols) {

    // debug line
    std::cout << "Drawing board size: " << state.getRows() << " x " << state.getCols() << std::endl;

    for (int i=0; i <buttons.size(); i++){
        for (int j = 0; j<buttons[i].size(); j++) {
            delete buttons[i][j];
        }
    }

    buttons.clear();

    int buttonSize = min((w-20) / cols, (h-50)/rows);

    for (int i = 0; i < rows; i++) {
        ArrayList<Button*> row;

        for (int j = 0; j < cols; j++) {
            int x_coord = x + j * (buttonSize + 5);
            int y_coord = y + i * (buttonSize + 5);
            Button* temp = new Button(x_coord, y_coord, buttonSize, buttonSize, "");
            temp->box(FL_ROUND_UP_BOX);
            temp->color(49);
            temp->color2(49);

            ON_CLICK(temp, GameInterface::handleClick);
            row.append(temp);
        }

        buttons.append(row);
    }

    for (int i=0; i<buttons.size(); i++) {
        for (int j=0; j<buttons.size(); j++) {
            buttons[i][j]->show();
        }
    }

    Fl::redraw(); 
}

void GameInterface::makeAImove() {
    if (!state.getEnabledAI()) {
        return;
    }

    Agent agent;
    int aiMove = agent.play(state);

    if (aiMove >= 0) {
        int rows = state.getRows();
        int cols = state.getCols();

        for (int dropRow = rows - 1; dropRow >= 0; dropRow--){
            int flatIndex = dropRow * cols + aiMove;
            if (state.buttonState(flatIndex) == -1) {
                state.play(flatIndex);
                updateButtons();

                if (state.hasWon(1)) {
                    statusBar->label("AI wins");
                    // disable all buttons function
                }
                else if (state.gameOver()) {
                    statusBar->label("Draw");
                    // disable all buttons function
                }
                break;
            }
        }
    }
}

void GameInterface::handleClick(Widget *sender){
    if (state.gameOver()) {
        return;
    }
    if (state.hasWon(0)) {
        statusBar->label("Red Wins");
        return;
    }
    if (state.hasWon(1)) {
        statusBar->label("Blue Wins");
        return;
    }

    for (int row = 0; row < buttons.size(); row++) {

        for (int col = 0; col < buttons[row].size(); col++) {

            if (sender == buttons[row][col]) {

                // Step 1: Find the column that was clicked

                int clickedCol = col;
                int rows = state.getRows();
                int cols = state.getCols();

                // check if it is ai's turn
                if (state.getEnabledAI() && state.getCurrentTurn() != 0){
                    return;
                }

                for (int dropRow = rows - 1; dropRow >= 0; dropRow--) {
                    int flatIndex = dropRow *cols + clickedCol;
                    if (state.buttonState(flatIndex) == -1) { // Assuming -1 means empty
                        int currentPlayer = state.getCurrentTurn();

                        // debug line
                        std::cout << "before move, current player is " << currentPlayer << std::endl;

                        if (state.play(flatIndex)){
                            updateButtons();
                        }

                        int prevPlayer = !currentPlayer;

                        // debug line
                        std::cout << "after move, previous player is " << prevPlayer << std::endl;
                        bool player0Win = state.hasWon(0);
                        bool player1Win = state.hasWon(1);
                        bool gameOver = state.gameOver();
                        std::cout << "Player 0 win: " << player0Win << std::endl;
                        std::cout << "Player 1 win: " << player1Win << std::endl;
                        std::cout << "GameOver: " << gameOver << std::endl;


                        if (state.hasWon(prevPlayer)) {
                            std::string winMessage = (prevPlayer == 0) ? "Red Wins" : "Blue Wins";
                            statusBar->label(winMessage.c_str());

                        } else if (state.gameOver() && !state.hasWon(prevPlayer)) {
                            statusBar->label("Draw");
                        } else if (state.getEnabledAI() && state.getCurrentTurn() == 1) {
                            makeAImove();
                        }
                        return;
                    }
                }
                return;

            }

        }

    }

}

void GameInterface::updateButtons(){

    int cols = state.getCols();

    for (int i = 0; i < buttons.size(); i++) {

    for (int j = 0; j < buttons[i].size(); j++) {

        int flatIndex = i * cols + j;

        int stateVal = state.buttonState(flatIndex); // still assuming state is 1D

        if (stateVal == 0){

            buttons[i][j]->color(fl_rgb_color(255, 0, 0));

            buttons[i][j]->color2(fl_rgb_color(255, 0, 0));

        } else if (stateVal == 1){

            buttons[i][j]->color(fl_rgb_color(0, 0, 255));

            buttons[i][j]->color2(fl_rgb_color(0, 0, 255));

        } else {

            buttons[i][j]->color(49);

            buttons[i][j]->color2(49);

        }

        buttons[i][j]->redraw();

    }

}

}

void GameInterface::hideButtons(){

   for (int i = 0; i < buttons.size(); i++) {

        for (int j = 0; j < buttons[i].size(); j++) {

            buttons[i][j]->hide();

        }

    }

}

void GameInterface::showButtons(){

    for (int i = 0; i < buttons.size(); i++) {

        for (int j = 0; j < buttons[i].size(); j++) {

            buttons[i][j]->show();

        }

    }

}

void GameInterface::show() {

    statusBar->show();

    showButtons();

}

void GameInterface::hide() {

    statusBar->hide();

    hideButtons();

}

void GameInterface::reset() {

    state.reset();

    updateButtons();

}

const GameState& GameInterface::getState() const{

    return state;

}

void GameInterface::setState(GameState state){
    this->state = state;

    // update any made changes to state

    int rows = state.getRows();
    int cols = state.getCols();

    bool dimChanged = (buttons.size() != rows);

    if (!dimChanged && buttons.size() > 0) {
        dimChanged = (buttons[0].size() != cols);
    }

    // debug line
    std::cout << "Board Size changed: " << (dimChanged ? "yes" : "no") << std::endl;
    std::cout << "Rows: " << state.getRows() << std::endl;
    std::cout << "Cols: " << state.getCols() << std::endl;

    if (dimChanged == true) {
        makeButtonGrid(rows, cols);

        if (statusBar->visible()){
            showButtons();
        }
    }

    updateButtons();

    string message = "Player vs Player";

    if (state.getEnabledAI()){

        message = "Player vs AI";

    }

    statusBar->label(message);

    // redraw board
    statusBar->redraw();
    
    // debug line
    std::cout << "Post-redraw Board Size changed: " << (dimChanged ? "yes" : "no") << std::endl;
    std::cout << "Rows: " << rows << std::endl;
    std::cout << "Cols: " << cols << std::endl;

}

