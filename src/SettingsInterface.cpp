// #include <FL/Enumerations.H>

#include <SettingsInterface.h>

#include <strings.h>

using namespace std;

using namespace bobcat;

SettingsInterface::SettingsInterface(int x, int y, int w, int h, GameState state){

    visible = true;

    this->x = x;

    this->y = y;

    this->w = w;

    this->h = h;

    this->state = state;

    pageHeading = new TextBox(x, y, w, 25, "Settings");

    pageHeading->labelsize(24);

    pageHeading->labelfont(FL_BOLD);

    gridSizeSelector = new Dropdown(x, y + 50, w, 25, "Board size");

    gridSizeSelector->add("5 x 5");

    gridSizeSelector->add("6 x 7");

    gridSizeSelector->add("7 x 8");

    enableAISelector = new Dropdown(x, y + 100, w, 25, "AI Player");

    enableAISelector->add("Enabled");

    enableAISelector->add("Disabled");

    if (state.getEnabledAI()) {

        enableAISelector->value(0);

    }

    else {

        enableAISelector->value(1);

    }

    int currentRows = state.getRows();

    int CurrentCols = state.getCols();

    if (currentRows == 5 && CurrentCols == 5) {

        gridSizeSelector->value(0);

    }

    else if (currentRows == 6 && CurrentCols == 7) {

        gridSizeSelector->value(1);

    }

    else if (currentRows == 7 && CurrentCols == 8) {

        gridSizeSelector->value(2);

    }

    else { // if nothing, fall back to default board

        gridSizeSelector->value (0);

    }

}

// board dim cases

BoardDimensions SettingsInterface::getBoardDimensions() const {

    BoardDimensions dims;

    switch (gridSizeSelector->value()) {

        case 0:

        dims.rows = 5;

        dims.cols = 5;

        break;

        case 1:

        dims.rows = 6;

        dims.cols = 7;

        break;

        case 2:

        dims.rows = 7;

        dims.cols = 8;

        break;

        default:

        dims.rows = 5;

        dims.cols = 5;

    }

    return dims;

}

GameState SettingsInterface::applyUpdates(){
    BoardDimensions dims = getBoardDimensions();
    bool shouldEnableAI = (enableAISelector->value() == 0);
    if (shouldEnableAI == true){
        state.enableAI();
    }
    else {
        state.disableAI();
    }

    bool dimsChanged = (dims.rows != state.getRows() || dims.cols != state.getCols());
    if (dimsChanged == 1){
        state.resize(dims.rows, dims.cols);
    }

    return state;
}

void SettingsInterface::hide(){

    visible = false;

    pageHeading->hide();

    gridSizeSelector->hide();

    enableAISelector->hide();

}

void SettingsInterface::show(){

    visible = true;

    pageHeading->show();

    gridSizeSelector->show();

    enableAISelector->show();

}

GameState SettingsInterface::getState() const{

    return state;

}

void SettingsInterface::setState(GameState state){

    this->state = state;

    if (state.getEnabledAI()) {

        enableAISelector->value(0);

    }

    else {

         enableAISelector->value(1);

    }

    int currentRows = state.getRows();

    int CurrentCols = state.getCols();

    if (currentRows == 5 && CurrentCols == 5) {

        gridSizeSelector->value(0);

    }

    else if (currentRows == 6 && CurrentCols == 7) {

        gridSizeSelector->value(1);

    }

    else if (currentRows == 7 && CurrentCols == 8) {

        gridSizeSelector->value(2);

    }

}