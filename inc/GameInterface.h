#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include <bobcat_ui/bobcat_ui.h>
#include <bobcat_ui/button.h>
#include <bobcat_ui/textbox.h>

#include <ArrayList.h>
#include <GameState.h>
#include <string>

class GameInterface {
    int x;
    int y;
    int w;
    int h;

    GameState state;

    bobcat::TextBox *statusBar;

    // Top buttons for dropping discs
    ArrayList<bobcat::Button*> columnButtons;

    // 2D grid for displaying placed discs
    ArrayList<ArrayList<bobcat::Button*>> boardGrid;

    void initButtons();                      // Set up column + grid buttons
    void handleClick(bobcat::Widget *sender); // Handle drop button clicks
    void updateButtons();                    // Refresh grid visuals
    void showButtons();                      // Show all buttons
    void hideButtons();                      // Hide all buttons
    bool checkWinningConditions();           // Show win/tie messages

public:
    GameInterface(int x, int y, int w, int h, GameState initialState);

    void reset();                            // Reset game state and UI
    void hide();                             // Hide full interface
    void show();                             // Show full interface

    GameState getState() const;
    void setState(GameState state);
};

#endif
