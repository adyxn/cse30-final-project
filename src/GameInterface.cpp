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


   for (int i = 0; i < 5; i++) {
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
    for (int row = 0; row < buttons.size(); row++) {
        for (int col = 0; col < buttons[row].size(); col++) {
            if (sender == buttons[row][col]) {
                // Step 1: Find the column that was clicked
                int clickedCol = col;
                for (int dropRow = buttons.size() - 1; dropRow >= 0; dropRow--) {
                    int flatIndex = dropRow * buttons[dropRow].size() + clickedCol;
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
    for (int i = 0; i < buttons.size(); i++) {
    for (int j = 0; j < buttons[i].size(); j++) {
        int flatIndex = i * buttons[i].size() + j;
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
