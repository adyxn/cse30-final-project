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

        int x_coord = x + j * (buttonSize + 10);

        int y_coord = y + i * (buttonSize + 10);

        Button* temp = new Button(x_coord, y_coord, 40, 40, "");

        temp->box(FL_ROUND_UP_BOX);

        ON_CLICK(temp, GameInterface::handleClick);

        row.append(temp);

    }

    buttons.append(row);

    }

}

void GameInterface::handleClick(Widget *sender){

    for (int row = 0; row < buttons.size(); row++) {

        for (int col = 0; col < buttons[row].size(); col++) {

            if (sender == buttons[row][col]) {

                // Step 1: Find the column that was clicked

                int clickedCol = col;

                int rows = state.getRows();

                int cols = state.getCols();

                for (int dropRow = rows - 1; dropRow >= 0; dropRow--) {

                    int flatIndex = dropRow * cols + clickedCol;

                    if (state.buttonState(flatIndex) == -1) { // Assuming -1 means empty

                        state.play(flatIndex);

                        updateButtons();

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

    if (dimChanged == true) {

        for (int i=0; i<buttons.size(); i++) {

            for (int j = 0; j<buttons[i].size(); j++) {
                if (buttons[i][j] != nullptr) {
                    buttons[i][j]->hide();
                    delete buttons[i][j];
                    buttons[i][j] = nullptr;
                }

            }

        }
        buttons.clear();

        makeButtonGrid(rows, cols);

    }

    updateButtons();

    string message = "Player vs Player";

    if (state.getEnabledAI()){

        message = "Player vs AI";

    }

    statusBar->label(message);

}

