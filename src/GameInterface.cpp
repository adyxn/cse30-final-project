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


   for (int i = 0; i < 4; i++) {
    ArrayList<Button*> row;
    for (int j = 0; j < 7; j++) {
        int x_coord = x + j * 50;
        int y_coord = y + i * 50;
        Button* temp = new Button(x_coord, y_coord, 40, 40, "");
        temp->box(FL_ROUND_UP_BOX);
        ON_CLICK(temp, GameInterface::handleClick);
        row.append(temp);
    }
    buttons.append(row);

   
}


   // updateButtons();


    string message = "Player vs Player";
    if (state.getEnabledAI()){
        message = "Player vs AI";
    }

    statusBar = new TextBox(x, h-25 + y, w, 25, message);
    statusBar->align(FL_ALIGN_CENTER);

}

void GameInterface::handleClick(Widget *sender){
    int rowCount = buttons.size();               // Number of rows
    if (rowCount == 0) return;
    int colCount = buttons[0].size();            // Number of columns
    
    for (int row = 0; row < rowCount; row++) {
        for (int col = 0; col < colCount; col++) {
            if (sender == buttons[row][col]) {
                int clickedCol = col;

                // Scan from bottom row upwards to find the first empty cell in the column
                for (int dropRow = rowCount - 1; dropRow >= 0; dropRow--) {
                    int flatIndex = dropRow * colCount + clickedCol;
                    if (state.buttonState(flatIndex) == -1) { // Empty slot
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
  int rowCount = buttons.size();
    if (rowCount == 0) return;

    int colCount = buttons[0].size();

    for (int row = 0; row < rowCount; row++) {
        for (int col = 0; col < colCount; col++) {
            int flatIndex = row * colCount + col;
            int stateVal = state.buttonState(flatIndex);

            if (stateVal == 0) {
                buttons[row][col]->color(fl_rgb_color(255, 0, 0));
                buttons[row][col]->color2(fl_rgb_color(255, 0, 0));
            } else if (stateVal == 1) {
                buttons[row][col]->color(fl_rgb_color(0, 0, 255));
                buttons[row][col]->color2(fl_rgb_color(0, 0, 255));
            } else {
                buttons[row][col]->color(49); // default background
                buttons[row][col]->color2(49);
            }

            buttons[row][col]->redraw();
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
