#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include "ArrayList.h"
#include <bobcat_ui/bobcat_ui.h>
#include <bobcat_ui/button.h>
#include <bobcat_ui/textbox.h>

#include <GameState.h>


class GameInterface{
    private:
        int x;
        int y;
        int w;
        int h;

        GameState state;

        ArrayList<ArrayList<bobcat::Button*>> buttons;

        bobcat::TextBox *statusBar;

        void makeButtonGrid(int rows, int cols);
        void updateButtons();
        void hideButtons();
        void showButtons();

    public:

        GameInterface(int x, int y, int w, int h, GameState initialState);

        void handleClick(bobcat::Widget *sender);

        void reset();
        void hide();
        void show();

        const GameState& getState() const;
        void setState(GameState state);

};

#endif