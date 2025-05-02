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


    for (int i = 0; i < 7; i++){
        int x_coord = x + i*50;
        int y_coord = 100;
        Button* temp = new Button(x_coord, y_coord, 40, 40, "");
        temp->box(FL_ROUND_UP_BOX);

        ON_CLICK(temp, GameInterface::handleClick);

        buttons.append(temp);
    }

    updateButtons();


    string message = "Player vs Player";
    if (state.getEnabledAI()){
        message = "Player vs AI";
    }

    statusBar = new TextBox(x, h-25 + y, w, 25, message);
    statusBar->align(FL_ALIGN_CENTER);

}

void GameInterface::handleClick(Widget *sender){
    for (int i = 0; i < buttons.size(); i++){
        if (sender == buttons[i]){
            // cout << "Button " << i << " was clicked" << endl;
            state.play(i);
            updateButtons();
            // check winning conditions
            return;
        }
    }
}

void GameInterface::updateButtons(){
    for (int i = 0; i < buttons.size(); i++){
        if (state.buttonState(i) == 0){
            // Make it red
            cout << "Button " << i << " red" << endl;
            buttons[i]->color(fl_rgb_color(255, 0, 0));
            buttons[i]->color2(fl_rgb_color(255, 0, 0));
        }
        else if (state.buttonState(i) == 1){
            // Make it blue
            cout << "Button " << i << " blue" << endl;
            buttons[i]->color(fl_rgb_color(0, 0, 255));
            buttons[i]->color2(fl_rgb_color(0, 0, 255));
        }
        else{
            // Make it gray
            cout << "Button " << i << " gray" << endl;
            buttons[i]->color(49);
            buttons[i]->color2(49);
        }
        buttons[i]->redraw();
    }
}


void GameInterface::hideButtons(){
    for (int i = 0; i < 7; i++){
        buttons[i]->hide();
    }
}

void GameInterface::showButtons(){
    for (int i = 0; i < 7; i++){
        buttons[i]->show();
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
