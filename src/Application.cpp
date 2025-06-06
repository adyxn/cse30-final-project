// Application.cpp
#include "Application.h"
#include "GameInterface.h"
#include "GameState.h"
#include <bobcat_ui/bobcat_ui.h>
#include <bobcat_ui/button.h>
#include <cstdlib>

using namespace bobcat;
using namespace std;

Application::Application(){
    window = new Window(100, 100, 400, 400, "Connect 4");

    // Initialize default state (you had a no-arg ctor)
    state = GameState();
    state.enableAI();  // your default

    // Create the game UI and settings UI, both seeded with the same state
    gameInterface     = new GameInterface(30,  35, 340, 340, state);
    settingsInterface = new SettingsInterface(30,  40, 340, 340, state);

    // Create Cancel / Apply buttons (hidden until "Settings")
    cancelButton = new Button(30,  360, 155, 25, "Cancel");
    applyButton  = new Button(215, 360, 155, 25, "Apply");

    settingsInterface->hide();
    cancelButton->hide();
    applyButton->hide();

    // Build the File menu
    menu    = new Menu();
    newGame = new MenuItem("File/New Game");
    settings= new MenuItem("File/Settings");
    quit    = new MenuItem("File/Exit");

    menu->addItem(newGame);
    menu->addItem(settings);
    menu->addItemSep(quit);

    // Wire up all callbacks
    ON_CLICK(newGame,     Application::handleNewGameMenuClick);
    ON_CLICK(settings,    Application::handleSettingsMenuClick);
    ON_CLICK(quit,        Application::handleQuitMenuClick);
    ON_CLICK(cancelButton,Application::handleCancelBtnClick);
    ON_CLICK(applyButton, Application::handleApplyBtnClick);

    window->show();
}

void Application::handleNewGameMenuClick(Widget* sender){
    // Just reset the existing game interface
    gameInterface->reset();
}

void Application::handleSettingsMenuClick(Widget* sender){
    // Hide game, show settings + buttons
    settingsInterface->setState(gameInterface->getState());
    gameInterface->hide();
    settingsInterface->show();
    applyButton->show();
    cancelButton->show();
}

void Application::handleCancelBtnClick(Widget* sender){
    // Discard changes & go back
    settingsInterface->hide();
    applyButton->hide();
    cancelButton->hide();
    gameInterface->show();
}

void Application::handleApplyBtnClick(Widget* sender){
    // Tell settingsInterface to update its internal GameState
    settingsInterface->applyUpdates();

    // Pull the new state out and apply to gameInterface
    state = settingsInterface->getState();
    // gameInterface->setState(state);

    delete gameInterface;
    window->begin();
    gameInterface     = new GameInterface(30,  35, 340, 340, state);
    window->end();
    window->redraw();


    // Hide settings and return to the game
    settingsInterface->hide();
    applyButton->hide();
    cancelButton->hide();
    gameInterface->show();
}

void Application::handleQuitMenuClick(Widget *){
    exit(0);
}
