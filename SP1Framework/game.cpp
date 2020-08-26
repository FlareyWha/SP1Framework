// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Map.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include "Customer.h"
#include "Box.h"
#include "Player.h"
#include "Shelf.h"
#include "Son.h"

double  g_dElapsedTime;
double g_dElapsedWorkTime;
double  g_dDeltaTime;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;

// Game specific variables here
int level, day;
bool travelling[6];
int avoiding[6]; 
bool spawned[6] = { false, false, false, false, false, false };
bool tutorialFlags[10] = { false, false, false, false, false, false, false, false, false, false };
bool g_bRestocking;

double timer[6];
double spawnTimer;
SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state
EGAMESTATES g_ePreviousGameState = S_SPLASHSCREEN; // initial state
EDEBUGSTATES g_eDebugState = D_OFF; // initial state

Customer* customerPtr[6] = {nullptr , nullptr , nullptr , nullptr , nullptr , nullptr};
Shelf* sPtr[6] = { nullptr , nullptr , nullptr , nullptr , nullptr , nullptr };
Son* cPtr[2] = { nullptr, nullptr };

Player p;

Box* boxPtr[7] = { nullptr, nullptr, nullptr, nullptr, nullptr , nullptr, nullptr };
Position* boxPosPtr[7] = { nullptr, nullptr, nullptr, nullptr, nullptr , nullptr, nullptr };
WORD BoxColour;
Map map;
int framesPassed;

int customerDirection[6];
WORD CustomerBoxColour[6];

// Console object
int g_ConsoleX = 80;
int g_ConsoleY = 25;
Console g_Console(g_ConsoleX, g_ConsoleY, "SP1 Framework");


//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
    
    // initialise random seed for random generation
    srand((unsigned int)time(NULL));

    // Set precision for floating point output
    g_dElapsedTime = 0.0;
    g_dElapsedWorkTime = 0.0;

    g_bRestocking = true;

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;
    g_ePreviousGameState = S_SPLASHSCREEN;

   /* g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 2;*/

    g_sChar.m_cLocation.X = 18; //changed character spawn location
    g_sChar.m_cLocation.Y = 1;

    //init level and day
    level = 1;
    day = 0;
    
    //init box and box pos
    boxPtr[0] = new Box;
    boxPosPtr[0] = new Position;
    boxPosPtr[0]->setX(18);
    boxPosPtr[0]->setY(2);

    /*for (int i = 0; i <= 7; i++) 
    {
        if (boxPtr[i] == nullptr) 
        {
            boxPtr[i] = new Box;
            boxPosPtr[i] = new Position;
        }
        boxPosPtr[0]->setX(18);
        boxPosPtr[0]->setY(2);
    }*/

    //init Son objects
    cPtr[0] = new Son;
    cPtr[1] = new Son;

    //init shelf

    if (sPtr[0] == nullptr && map.getGrid(3, 1) != 'A') { //need to delete Shelf eventually
        sPtr[0] = new Shelf;
        sPtr[0]->setShelf(0x50);
    }
    if (sPtr[1] == nullptr && map.getGrid(4, 1) != 'A') {
        sPtr[1] = new Shelf;
        sPtr[1]->setShelf(0x10);
    }
    if (sPtr[2] == nullptr && map.getGrid(5, 1) != 'A') {
        sPtr[2] = new Shelf;
        sPtr[2]->setShelf(0xB0);
    }
    if (sPtr[3] == nullptr && map.getGrid(6, 1) != 'A') {
        sPtr[3] = new Shelf;
        sPtr[3]->setShelf(0xE0);
    }
    if (sPtr[4] == nullptr && map.getGrid(7, 1) != 'A') {
        sPtr[4] = new Shelf;
        sPtr[4]->setShelf(0xA0);
    }
    if (sPtr[5] == nullptr && map.getGrid(8, 1) != 'A') {
        sPtr[5] = new Shelf;
        sPtr[5]->setShelf(0x90);
    }

    for (int i = 0; i < 6; i++) {
        timer[i] = -1;
        travelling[i] = false;
        avoiding[i] = 0; 
        /*
        spawned[i] = false;
        customerPtr[i] = nullptr;
        sPtr[i] = nullptr;
        */
    }

    g_sChar.m_bActive = true;
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");

    // remember to set your keyboard handler, so that your functions can be notified of input events
    g_Console.setKeyboardHandler(keyboardHandler);
    g_Console.setMouseHandler(mouseHandler);
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();
}

//--------------------------------------------------------------
// Purpose  : Get all the console input events
//            This function sets up the keyboard and mouse input from the console.
//            We will need to reset all the keyboard status, because no events will be sent if no keys are pressed.
//
//            Remember to set the handlers for keyboard and mouse events.
//            The function prototype of the handler is a function that takes in a const reference to the event struct
//            and returns nothing. 
//            void pfKeyboardHandler(const KEY_EVENT_RECORD&);
//            void pfMouseHandlerconst MOUSE_EVENT_RECORD&);
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput( void )
{
    // resets all the keyboard events
    memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
    // then call the console to detect input from user
    g_Console.readConsoleInput();    
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the keyboard input. Whenever there is a keyboard event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            The KEY_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any keyboard event in the Splashscreen state
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent - reference to a key event struct
// Output   : void
//--------------------------------------------------------------
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent)
{    
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: gameplayKBHandler(keyboardEvent);// handle anything for splash screen
        break;
    case S_MENU: gameplayKBHandler(keyboardEvent);
        break;
    case S_ENDOFWORKSCREEN: gameplayKBHandler(keyboardEvent);
        break;
    case S_GAMEOVER: gameplayKBHandler(keyboardEvent);
        break;
    case S_HOME: gameplayKBHandler(keyboardEvent); // handle home menu keyboard menu
        break;
    case S_TUT: gameplayKBHandler(keyboardEvent);
        break;
    case S_GAME: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the mouse input. Whenever there is a mouse event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            For the mouse event, if the mouse is not moved, no event will be sent, hence you should not reset the mouse status.
//            However, if the mouse goes out of the window, you would not be able to know either. 
//
//            The MOUSE_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any mouse event in the Splashscreen state
//            
// Input    : const MOUSE_EVENT_RECORD& mouseEvent - reference to a mouse event struct
// Output   : void
//--------------------------------------------------------------
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{    
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: gameplayMouseHandler(mouseEvent); // handle mouse input for splash screen
        break;
    case S_MENU: gameplayMouseHandler(mouseEvent); // handle mouse input for menu
        break;
    case S_ENDOFWORKSCREEN: gameplayMouseHandler(mouseEvent);
        break;
    case S_GAMEOVER: gameplayMouseHandler(mouseEvent);
        break;
    case S_HOME: gameplayMouseHandler(mouseEvent); // handle mouse input for home menu
        break;
    case S_TUT: gameplayMouseHandler(mouseEvent);
        break;
    case S_GAME: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    }
} //140

//--------------------------------------------------------------
// Purpose  : This is the keyboard handler in the game state. Whenever there is a keyboard event in the game state, this function will be called.
//            
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent) //movement
{
    // here, we map the key to our enums
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode)
    {
    case 0x57: key = K_UP; break; // changed VK_UP to 0x57 "W"
    case 0x53: key = K_DOWN; break; //changed VK_DOWN to 0x53 "S"
    case 0x41: key = K_LEFT; break; // changed VK_LEFT to 0x41 "A"
    case 0x44: key = K_RIGHT; break; // changed VK_RIGHT to 0x44 "D"
    case VK_SPACE: key = K_SPACE; break;
    case VK_ESCAPE: key = K_ESCAPE; break; 
    case VK_F3: key = K_F3; break;
    case VK_F4: key = K_F4; break;
    }
    // a key pressed event would be one with bKeyDown == true
    // a key released event would be one with bKeyDown == false
    // if no key is pressed, no event would be fired.
    // so we are tracking if a key is either pressed, or released
    if (key != K_COUNT)
    {
        g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
    }    
}

//--------------------------------------------------------------
// Purpose  : This is the mouse handler in the game state. Whenever there is a mouse event in the game state, this function will be called.
//            
//            If mouse clicks are detected, the corresponding bit for that mouse button will be set.
//            mouse wheel, 
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
    {
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    }
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
        case S_SPLASHSCREEN: updateSplashScreen(); // game logic for the splash screen
            break;
        case S_MENU: updateMenu();
            break;
        case S_ENDOFWORKSCREEN: updateEndofWorkScreen();
            break;
        case S_GAMEOVER: updateGameOver();
            break;
        case S_HOME: updateHome();
            break;
        case S_TUT: {
            spawnTimer += dt;
            for (int i = 0; i < 6; i++)
            {
                if (timer[i] != -1)
                {
                    timer[i] += dt;
                }
            }
            g_dElapsedWorkTime += dt;  updateTutorial();
            break;
        }
        case S_GAME: {
            spawnTimer += dt;
            for (int i = 0; i < 6; i++)
            {
                if (timer[i] != -1)
                {
                    timer[i] += dt;
                }
            }
            g_dElapsedWorkTime += dt; updateGame();// gameplay logic when we are in the game
            break;
        }
    }
}


void updateSplashScreen()    // Splash screen logic
{
    processUserInput();
}

void updateMenu() // Menu logic
{
    processUserInput();
}

void updateEndofWorkScreen()
{
    processUserInput();
    
}

void updateGameOver()
{
    processUserInput();
}

void updateHome() // Home logic
{
    g_ePreviousGameState = g_eGameState;
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
}

void updateTutorial() //Tutorial level logic
{
    g_ePreviousGameState = g_eGameState;
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
    actuallyMoving();
    pickUpBoxes();
    restockShelf();
}

void updateGame()       // game logic
{
    g_ePreviousGameState = g_eGameState;
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter(); 
    actuallyMoving();
    pickUpBoxes();
    restockShelf();
}

void moveCharacter()//to check if the player is pressing a key
{     
    if (g_skKeyEvent[K_UP].keyDown)
    {
        g_sChar.moving.UP = true;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown)
    {
        g_sChar.moving.RIGHT = true;
    }
    if (g_skKeyEvent[K_DOWN].keyDown)
    {
        g_sChar.moving.DOWN = true;
    }
    if (g_skKeyEvent[K_LEFT].keyDown)
    {
        g_sChar.moving.LEFT = true;
    }

    if (g_skKeyEvent[K_UP].keyReleased)
    {
        g_sChar.moving.UP = false;
    }
    if (g_skKeyEvent[K_RIGHT].keyReleased)
    {
        g_sChar.moving.RIGHT = false;
    }
    if (g_skKeyEvent[K_DOWN].keyReleased)
    {
        g_sChar.moving.DOWN = false;
    }
    if (g_skKeyEvent[K_LEFT].keyReleased)
    {
        g_sChar.moving.LEFT = false;
    } 
}

void actuallyMoving()
{
    if (framesPassed % 3 == 0) {
        //PLAYER / BOX COLLISION WITH ENVIRONMENT IS SOLVED HERE
        if (map.collision(g_sChar.m_cLocation.Y, g_sChar.m_cLocation.X, 0, 0, map) != '0')
        {
            g_sChar.m_cLocation.Y++;
            boxPosPtr[0]->setY(g_sChar.m_cLocation.Y);
            g_sChar.m_cLocation.X++;
            boxPosPtr[0]->setX(g_sChar.m_cLocation.X);
        }

        switch (g_sChar.moving.UP)
        {
        case true:

            if (map.collision(g_sChar.m_cLocation.Y, g_sChar.m_cLocation.X, -1, 0, map) == '0')
            {
                g_sChar.m_cLocation.Y--;
            }
            boxPosPtr[0]->setX(g_sChar.m_cLocation.X);
            boxPosPtr[0]->setY(g_sChar.m_cLocation.Y - 1);
            if (map.collision(boxPosPtr[0]->getY(), boxPosPtr[0]->getX(), 0, 0, map) != '0')
            {
                g_sChar.m_cLocation.Y++;
                boxPosPtr[0]->setY(g_sChar.m_cLocation.Y - 1);
            }
            break;

        case false:
            break;
        }

        switch (g_sChar.moving.DOWN)
        {
        case true:

            if (map.collision(g_sChar.m_cLocation.Y, g_sChar.m_cLocation.X, +1, 0, map) == '0')
            {
                g_sChar.m_cLocation.Y++;
            }
            boxPosPtr[0]->setX(g_sChar.m_cLocation.X);
            boxPosPtr[0]->setY(g_sChar.m_cLocation.Y + 1);
            if (map.collision(boxPosPtr[0]->getY(), boxPosPtr[0]->getX(), 0, 0, map) != '0')
            {
                g_sChar.m_cLocation.Y--;
                boxPosPtr[0]->setY(g_sChar.m_cLocation.Y + 1);
            }
            break;
        case false:
            break;
        }
        switch (g_sChar.moving.LEFT)
        {
        case true:

            if (map.collision(g_sChar.m_cLocation.Y, g_sChar.m_cLocation.X, 0, -1, map) == '0')
            {
                g_sChar.m_cLocation.X--;
            }
            boxPosPtr[0]->setX(g_sChar.m_cLocation.X - 1);
            boxPosPtr[0]->setY(g_sChar.m_cLocation.Y);
            if (map.collision(boxPosPtr[0]->getY(), boxPosPtr[0]->getX(), 0, 0, map) != '0')
            {
                g_sChar.m_cLocation.X++;
                boxPosPtr[0]->setX(g_sChar.m_cLocation.X - 1);
            }
            break;
        case false:
            break;
        }
        switch (g_sChar.moving.RIGHT)
        {
        case true:

            if (map.collision(g_sChar.m_cLocation.Y, g_sChar.m_cLocation.X, 0, +1, map) == '0')
            {
                g_sChar.m_cLocation.X++;
            }
            boxPosPtr[0]->setX(g_sChar.m_cLocation.X + 1);
            boxPosPtr[0]->setY(g_sChar.m_cLocation.Y);
            if (map.collision(boxPosPtr[0]->getY(), boxPosPtr[0]->getX(), 0, 0, map) != '0')
            {
                g_sChar.m_cLocation.X--;
                boxPosPtr[0]->setX(g_sChar.m_cLocation.X + 1);
            }
            break;
        case false:
            break;
        }
    }
}

void pickUpBoxes()  //todo
{
    if (p.isHoldingProduct() == false) {
        BoxColour = 0x70; //empty box grey
    }

    if (g_skKeyEvent[K_SPACE].keyReleased && boxPosPtr[0]->getX() == 2 && boxPosPtr[0]->getY() == 3 && map.getGrid(3, 1) != 'A')
    {
        BoxColour = 0x50; //toilet paper purple
        p.holdsProduct();
    }
    if (g_skKeyEvent[K_SPACE].keyReleased && boxPosPtr[0]->getX() == 2 && boxPosPtr[0]->getY() == 4 && map.getGrid(4, 1) != 'A')
    {
        BoxColour = 0x10; //instant noodle dark blue          
        p.holdsProduct();
    }
    if (g_skKeyEvent[K_SPACE].keyReleased && boxPosPtr[0]->getX() == 2 && boxPosPtr[0]->getY() == 5 && map.getGrid(5, 1) != 'A')
    {
        BoxColour = 0xB0; //canned food teal

        p.holdsProduct();
    }
    if (g_skKeyEvent[K_SPACE].keyReleased && boxPosPtr[0]->getX() == 2 && boxPosPtr[0]->getY() == 6 && map.getGrid(6, 1) != 'A')
    {
        BoxColour = 0xE0; //rice cream

        p.holdsProduct();
    }
    if (g_skKeyEvent[K_SPACE].keyReleased && boxPosPtr[0]->getX() == 2 && boxPosPtr[0]->getY() == 7 && map.getGrid(7, 1) != 'A')
    {
        BoxColour = 0xA0; //vegetable green

        p.holdsProduct();
    }
    if (g_skKeyEvent[K_SPACE].keyReleased && boxPosPtr[0]->getX() == 2 && boxPosPtr[0]->getY() == 8 && map.getGrid(8, 1) != 'A')
    {
        BoxColour = 0x90;//bandages blue

        p.holdsProduct();
    }
}

void restockShelf(){
    for (int i = 29; i < 38; i++) { //3 SHELVES ON THE LEFT
        for (int j = 0; j < 3; j++) {

            if (sPtr[j] != nullptr && map.getGrid(j + 3, 1) != 'A') {
                if (g_skKeyEvent[K_SPACE].keyReleased && BoxColour == sPtr[j]->returnShelfColour() && boxPosPtr[0]->getY() == 6 * (1 + j) + 1 
                     && boxPosPtr[0]->getX() == i && sPtr[j]->getAmount() != 9) 
                {          
                    sPtr[j]->increaseItem(1);
                    p.releaseProduct();
                }

                
            }
        }
    }

    for (int j = 0; j < 3;j++) {
        if (g_skKeyEvent[K_SPACE].keyReleased && BoxColour == sPtr[j]->returnShelfColour() && boxPosPtr[0]->getY() == 6 * (j + 1) && boxPosPtr[0]->getX() == 38 && sPtr[j]->getAmount() != 9)
        {
            sPtr[j]->increaseItem(1);
            p.releaseProduct();
        }
    }
   

    for (int i = 50; i < 59; i++) { //3 shelves on the right

        for (int j = 3; j < 6;j++) {

            if (sPtr[j] != nullptr && map.getGrid(j + 3, 1) != 'A') {

                if (g_skKeyEvent[K_SPACE].keyReleased && BoxColour == sPtr[j]->returnShelfColour() && boxPosPtr[0]->getY() == 6 * (j - 2) + 1 
                     && boxPosPtr[0]->getX() == i && sPtr[j]->getAmount() != 9) 
                {
                    sPtr[j]->increaseItem(1);
                    p.releaseProduct();
                }

                
            }
        }
    }

    for (int j = 3; j < 6;j++) {

        if (g_skKeyEvent[K_SPACE].keyReleased && BoxColour == sPtr[j]->returnShelfColour() && boxPosPtr[0]->getY() == 6 * (j - 2)
            && boxPosPtr[0]->getX() == 59 && sPtr[j]->getAmount() != 9)
        {
            sPtr[j]->increaseItem(1);
            p.releaseProduct();
        }
    }
}

//void updateCustomer()
//{
//    if
//}

void updateSons()
{
    for (int i = 0; i < 2; i++) {
        if (cPtr[i]->getStatusFed() == true) {
            cPtr[i]->resetNODUnfed();
        }
        else {
            cPtr[i]->increaseNODUnfed();
        }
        cPtr[i]->resetFed();
        if (cPtr[i]->getTreatState() == true) {
            cPtr[i]->Recovers();
            cPtr[i]->isTreated();
        }
    }
}

void checkEnd() //Check if day has ended and update variables as well as game over conditions
{
    if (p.getUnsatisfiedCustomers() == 10) {
            g_eGameState = S_GAMEOVER;
            p.resetUnsatisfiedCustomers(); //reset unsatifiedCustomers to 0
        }
    else if (g_dElapsedWorkTime >= 5) {
        g_bRestocking = false;
    }
    if (g_skKeyEvent[K_F4].keyDown || g_dElapsedWorkTime >= 150)
    {
        g_sChar.moving.UP = false;
        g_sChar.moving.DOWN = false;
        g_sChar.moving.LEFT = false;
        g_sChar.moving.RIGHT = false;
        g_dElapsedWorkTime = 0.0;
        g_bRestocking = true;
        COORD c;
        c.X = 18;
        c.Y = 1;
        g_sChar.m_cLocation = c;
        boxPosPtr[0]->setX(18);
        boxPosPtr[0]->setY(2);
        for (int i = 0; i < level + 1; i++) {
            sPtr[i]->setAmount(0);
        }
        p.releaseProduct();
        for (int i = 0; i < 2; i++) {
            if (cPtr[i]->getStatus() == true) {
                cPtr[i]->increaseNODSick();
            }
            if (day != 0) {
                cPtr[i]->ChancesOfFallingSick(cPtr[i]->getNODUnfed());
            }
            if (cPtr[i]->getNODSick() == 4) {
                g_eGameState = S_GAMEOVER;
                cPtr[i]->isHosp();
            }
            else if (g_eGameState != S_GAMEOVER) {
                g_eGameState = S_ENDOFWORKSCREEN;
            }
        if (p.getRentStatus() == true && day % 7 == 0 && day != 0) {
            p.isRentPaid();
            g_eGameState = S_ENDOFWORKSCREEN;
        }
        else if (p.getRentStatus() == false && day % 7 == 0 && day != 0) {
            g_eGameState = S_GAMEOVER;
        }
        }
    }
    
}

void processDebugState() //Toggle debug options
{
    if (g_skKeyEvent[K_F3].keyDown)
    {
        if (g_eDebugState == D_OFF)
        {
            g_eDebugState = D_BOTH;
        }
        else
        {
            g_eDebugState = D_OFF;
        }
    }
}

void processInputSplash() // All input processing related to Splashscreen
{
    COORD c = g_Console.getConsoleSize();
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {
        if (g_mouseEvent.mousePosition.X >= c.X / 2 - 16
            && g_mouseEvent.mousePosition.X <= c.X / 2 + 17
            && g_mouseEvent.mousePosition.Y == c.Y / 25 + 12) //Change to main game state once mouse clicks on the button
        {
            g_ePreviousGameState = g_eGameState;
            g_eGameState = S_MENU;
        }
    }
}

void processInputMenu() //All input processing related to Main Menu
{
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED 
        && (g_ePreviousGameState == S_SPLASHSCREEN || g_ePreviousGameState == S_GAMEOVER))
    {
        COORD c = g_Console.getConsoleSize();
        if ((g_mouseEvent.mousePosition.X >= c.X / 2 - 7
            && g_mouseEvent.mousePosition.X <= c.X / 2 + 1)
            && g_mouseEvent.mousePosition.Y == 9) //Change to main game state once mouse clicks on the button
        {
            g_ePreviousGameState = g_eGameState;
            g_eGameState = S_TUT;
        }
    }
    else if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {
        COORD c = g_Console.getConsoleSize();
        if ((g_mouseEvent.mousePosition.X >= c.X / 2 - 7
            && g_mouseEvent.mousePosition.X <= c.X / 2 + 1)
            && g_mouseEvent.mousePosition.Y == 9) //Change to previous game state once mouse clicks on the button
        {
            g_eGameState = g_ePreviousGameState;
        }
    }

    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {
        COORD c = g_Console.getConsoleSize();
        if ((g_mouseEvent.mousePosition.X >= c.X / 6 + 20
            && g_mouseEvent.mousePosition.X <= c.X / 6 + 29)
            && g_mouseEvent.mousePosition.Y == 12) //Exit once mouse clicks on the button
        {
            g_bQuitGame = true;
        }
    }
}

void processInputEndOfWorkScreen()
{
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {
        COORD c = g_Console.getConsoleSize();
        if ((g_mouseEvent.mousePosition.X >= c.X / 6 + 15
            && g_mouseEvent.mousePosition.X <= c.X / 6 + 36)
            && g_mouseEvent.mousePosition.Y == 12) //Change to home state once mouse clicks on the button
        {
            p.receivePay(p.getTotalEarned()); //increase total savings
            p.resetDayEarnings(); //reset daily amount earned back to 0
            
            g_eGameState = S_HOME;
        }
    }
}

void processInputGameOver()
{
    if (g_skKeyEvent[K_ESCAPE].keyReleased) {// opens main menu if player hits the escape key
        for (int i = 0; i < 2; i++) {
            cPtr[i]->resetNODSick();
            cPtr[i]->resetNODUnfed();
            cPtr[i]->Recovers();
            cPtr[i]->resetHospState();
        }
        g_eGameState = S_MENU;
    }
    day = 0; level = 1;
    g_ePreviousGameState = S_GAMEOVER;
}

void deleteCustomer()
{
    for (int i = 0; i < 6; i++)
    {
        if (customerPtr[i] != nullptr)
        {
            delete customerPtr[i];
            customerPtr[i] = nullptr;
            travelling[i] = false;
            timer[i] = -1;
        }
    }
}

void processInputHome() //note
{
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {
        COORD c = g_Console.getConsoleSize();
        if ((g_mouseEvent.mousePosition.X >= c.X - 20
            && g_mouseEvent.mousePosition.X <= c.X - 17)
            && g_mouseEvent.mousePosition.Y == 9) //Change to main menu state once mouse clicks on the button
        {
            g_eGameState = S_MENU;
        }
        if ((g_mouseEvent.mousePosition.X >= c.X - 20
            && g_mouseEvent.mousePosition.X <= c.X - 13)
            && g_mouseEvent.mousePosition.Y == c.Y / 5 + 3) //Change to main game state once mouse clicks on the button
        {
            day++;
            p.resetUnsatisfiedCustomers(); //reset unsatifiedCustomers to 0
            deleteCustomer();
            g_eGameState = S_GAME;
            updateSons();
        }

        // Expenses toggling
        if ((g_mouseEvent.mousePosition.X == 39)
            && g_mouseEvent.mousePosition.Y == 7) //Toggle recognition of son 1 being fed
        {
            if (p.getSavings() >= 30 && cPtr[0]->getStatusFed() == false) {
                cPtr[0]->isFed();
                p.payFood();
            }
            else if (cPtr[0]->getStatusFed() == true) {
                cPtr[0]->isFed();
                p.receivePay(30);
            }
        }
        if ((g_mouseEvent.mousePosition.X == 39)
            && g_mouseEvent.mousePosition.Y == 12) //Toggle recognition of son 1 being fed
        {
            if (p.getSavings() >= 30 && cPtr[1]->getStatusFed() == false) {
                cPtr[1]->isFed();
                p.payFood();
            }
            else if (cPtr[1]->getStatusFed() == true) {
                cPtr[1]->isFed();
                p.receivePay(30);
            }
        }
        if ((g_mouseEvent.mousePosition.X == 44)
            && g_mouseEvent.mousePosition.Y == 6
            && cPtr[0]->getStatus() == true) //Toggle recognition of son 1 being treated
        {
            if (p.getSavings() >= 100 && cPtr[0]->getTreatState() == false) {
                cPtr[0]->isTreated();
                p.payMedicine();
            }
            else if (cPtr[0]->getTreatState() == true) {
                cPtr[0]->isTreated();
                p.receivePay(100);
            }
        }
        if ((g_mouseEvent.mousePosition.X == 44)
            && g_mouseEvent.mousePosition.Y == 11
            && cPtr[1]->getStatus() == true) //Toggle recognition of son 2 being treated
        {
            if (p.getSavings() >= 100 && cPtr[1]->getTreatState() == false) {
                cPtr[1]->isTreated();
                p.payMedicine();
            }
            else if (cPtr[1]->getTreatState() == true) {
                cPtr[1]->isTreated();
                p.receivePay(100);
            }
        }
        if ((g_mouseEvent.mousePosition.X == 45)
            && g_mouseEvent.mousePosition.Y == 14
            && day % 6 == 0 && day != 0) {
            if (p.getSavings() >= 200 && p.getRentStatus() == false) {
                p.isRentPaid();
                p.payRent();
            }
            else if (p.getRentStatus() == true) {
                p.isRentPaid();
                p.receivePay(200);
            }
        }
    }
}

void processUserInput()
{
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: processInputSplash(); 
        break;
    case S_MENU: processInputMenu();
        break;
    case S_ENDOFWORKSCREEN: processInputEndOfWorkScreen(); 
        break;
    case S_GAMEOVER: processInputGameOver();
    case S_HOME: processInputHome();
        if (g_skKeyEvent[K_ESCAPE].keyReleased)// opens main menu if player hits the escape key
            g_eGameState = S_MENU; 
        break;
    case S_TUT:
        if (g_skKeyEvent[K_ESCAPE].keyReleased)// opens main menu if player hits the escape key
            g_eGameState = S_MENU;
        checkEnd();
        break;
    case S_GAME:
        if (g_skKeyEvent[K_ESCAPE].keyReleased)// opens main menu if player hits the escape key
            g_eGameState = S_MENU;
        checkEnd();
        break;
    }
    processDebugState();
}

//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()// make render functions for our level and put it in the switch case
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: renderSplashScreen();
        break;
    case S_MENU: renderMainMenu();
        break;
    case S_ENDOFWORKSCREEN: renderEndOfWorkScreen();
        break;
    case S_GAMEOVER: renderGameOver();
        break;
    case S_HOME: renderHome();
        break;
    case S_TUT: renderTutorialLevel();
        break;
    case S_GAME: renderGame();
        break;
    }

    switch (g_eDebugState)
    {
    case D_OFF: break;
    case D_FRAMES: renderFramerate(); break;
    case D_INPUT: renderInputEvents(); break;
    case D_BOTH: renderFramerate(); renderInputEvents(); break;
    }

    //renderFramerate();      // renders debug information, frame rate, elapsed time, etc
    //renderInputEvents();    // renders status of input events
    renderToScreen();       // dump the contents of the buffer to the screen, one frame worth of game
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x00); 
}

void renderToScreen() 
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void renderSplashScreen()  // renders the splash screen
{
    COORD c = g_Console.getConsoleSize();
    c.Y /= 25;
    c.X = c.X / 2 - 5; 
    g_Console.writeToBuffer(c, "Fair Prize", 0x03);
    c.Y += 12;
    c.X = g_Console.getConsoleSize().X / 2 - 16;
    g_Console.writeToBuffer(c, "Left click here to start the game!", 0x09);
}

void renderGame()
{
   
    if (day < 5) {
        level = day + 1;
    }
    else { 
        level = 5;
    }
    map.chooseMap(level, g_Console);       // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    renderCustomer();
    renderBoxes();
    renderShelfAmount();
    renderHUD();
}

void renderShelfAmount()
{
    if (g_eGameState == S_GAME) {
        for (int i = 0; i < level + 1; i++) {
            int amt = sPtr[i]->getAmount();
            amt = sPtr[i]->getAmount();
            renderItem(i);
        }
    }
    else if (g_eGameState == S_TUT) {
        for (int i = 0; i < level + 1; i++) {
            int amt = sPtr[i]->getAmount();
            amt = sPtr[i]->getAmount();
            renderItem(i);
        }
    }
}

void renderHUD()
{
    framesPassed++; // counts frames
    COORD c;
    std::ostringstream ss;
    //ss.str("");//display strikes
    //ss << "Strikes:" << p.getStrikes();
    //c.X = 4;
    //c.Y = 0;
    //g_Console.writeToBuffer(c, ss.str(), 0x80);
    ss.str("");// display the current day
    ss << "Day:" << day;
    c.X = 20;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x80);
    
    ss.str("");// display the daily income
    ss << "Money earned: $" << p.getTotalEarned();
    c.X = 60;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x80);

    ss.str("");     // displays the elapsed time
    if (g_bRestocking == false) {
        int minute;
        int secs = 150 - g_dElapsedWorkTime;
        minute = secs / 60;
        if (minute > 0) {
            ss << "Time left : " << minute << " mins " << secs - minute * 60 << " secs";
        }
        else
        {
            ss << "Time left : " << secs - minute * 60 << " secs";
        }
    }
    else if (g_bRestocking == true) {
        int secs = 30 - g_dElapsedWorkTime;
        ss << "Restocking time : " << secs << " secs";
    }
    c.X = 30; //change to shift location of timer
    c.Y = 0;  //we might use this or we might need to make a new timer to show when the game starts
    g_Console.writeToBuffer(c, ss.str(), 0x80);
    ss.str(""); //probably can be implemented cleaner
    ss << framesPassed << "frames";
    c.X = 36;
    c.Y = 24;
    g_Console.writeToBuffer(c, ss.str(), 0x80);

    ss.str(""); //probably can be implemented cleaner
    ss << "Unsatisfied Customers: " << p.getUnsatisfiedCustomers();
    c.X = 1;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x80);

}

void renderItem(int shelf)
{
    const WORD colors[] = {
        0x50, 0x10, 0xB0, 0xE0, 0xA0, 0x90, 0x60
    };

    COORD c;
    std::ostringstream ss;
    c.X = 37;
    c.Y = 0;
    int amt = sPtr[shelf]->getAmount();
    for (int x = 0; x <= shelf; x++) {
        c.Y += 6;
    }
    if (shelf >= 3)
    {
        c.X += 21;
    }
    for (int x = 3; x <= shelf; x++) {
        switch (x)
        {
        case 3: c.Y = 6;
            break;
        case 4: c.Y = 12;
            break;
        case 5: c.Y = 18;
            break;
        }
    }
    for (int i = 0; i != amt; i++) {
        g_Console.writeToBuffer(c, ' ', colors[shelf]);
        c.X--;
    }
    if (shelf >= 3)
    {
        c.X = 48;
    }
    else {
        c.X = 27;
    }
}

void renderMap()
{
    // Set up sample colours, and output shadings
    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

    COORD c;
    for (int i = 0; i < 12; ++i)
    {
        c.X = 5;
        c.Y = i + 1;
        colour(colors[i]); //Change text colour
        g_Console.writeToBuffer(c, " ", colors[i]); //Change text background
    }
}

void renderMainMenu()
{
    map.chooseMap(0, g_Console);
    renderMenuAnimation();
    COORD c = g_Console.getConsoleSize();
    c.Y /= 25;
    c.X = c.X / 2 - 5;
    g_Console.writeToBuffer(c, "Main Menu", 0xF0);
    c.Y += 8;
    c.X = g_Console.getConsoleSize().X / 6 + 20;
    if (g_ePreviousGameState == S_SPLASHSCREEN || g_ePreviousGameState == S_GAMEOVER)
        g_Console.writeToBuffer(c, "Start New", 0xF0);
    else if (g_ePreviousGameState == S_HOME)
        g_Console.writeToBuffer(c, "Back Home", 0xF0);
    else
        g_Console.writeToBuffer(c, "Resume Work", 0xF0);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 6 + 20;
    g_Console.writeToBuffer(c, "Save", 0xF0);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 6 + 20;
    g_Console.writeToBuffer(c, "Load", 0xF0);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 6 + 20;
    g_Console.writeToBuffer(c, "Exit Game", 0xF0);
}

void renderHome() 
{
    map.chooseMap(6, g_Console);
    renderMenuAnimation();
    COORD c = g_Console.getConsoleSize();
    // Game Mechanic stuff
    c.Y /= 25;
    c.X = c.X / 2 - 5;
    renderHomeExpenses(c);
    // Menu stuff
    c = g_Console.getConsoleSize();
    c.Y /= 5;
    c.X = c.X - 20;
    g_Console.writeToBuffer(c, "Options", 0xF0);
    c.Y += 3;
    g_Console.writeToBuffer(c, "Next Day", 0xF0);
    c.Y += 1;
    g_Console.writeToBuffer(c, "Menu", 0xF0);
}

void renderMenuAnimation() //tbd
{
    framesPassed++;
    if (framesPassed % 50 == 0)
    {
        for (int pGy = 0; pGy < 25; pGy++)
        {
            for (int pGx = 0; pGx < 25; pGx++)
            {
                switch (map.getGrid(pGy, pGx))
                {
                case '6': map.setGrid(pGy, pGx, '7'); break;
                case '5': map.setGrid(pGy, pGx, '6'); break;
                case '4': map.setGrid(pGy, pGx, '5'); break;
                case '3': map.setGrid(pGy, pGx, '4'); break;
                case '2': map.setGrid(pGy, pGx, '3'); break;
                case '1': map.setGrid(pGy, pGx, '2'); break;
                case '7': map.setGrid(pGy, pGx, '1'); break;
                }
            }
        }
    }
}

void renderHomeExpenses(COORD c)
{
    std::ostringstream ss;
    ss.str("");
    //g_Console.writeToBuffer(c, "Home", 0xF0);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2.5 + 1;
    ss << "Savings : $" << p.getSavings();
    g_Console.writeToBuffer(c, ss.str(), 0xF0);
    ss.str("");
    c.Y += 2;
    c.X = g_Console.getConsoleSize().X / 3 + 1;
    g_Console.writeToBuffer(c, "!=========Son 1=========!", 0xF0);
    c.Y += 1;
    g_Console.writeToBuffer(c, "State : ", 0xF0);
    c.X += 8;
    if (cPtr[0]->getStatus() == true) {
        g_Console.writeToBuffer(c, "(Q//A//Q )", 0xF0);
    }
    else {
        g_Console.writeToBuffer(c, "(O w O 6)", 0xF0);
    }
    c.X -= 8;
    c.Y += 1;
    if (cPtr[0]->getStatus() == true) {
        g_Console.writeToBuffer(c, "Medicine ($100) [ ]", 0xF0);
        if (cPtr[0]->getTreatState() == true) {
            c.X += 17;
            g_Console.writeToBuffer(c, " ", 0x00);
            c.X -= 17;
        }
    } //Make this hidden according to Son 1 state
    c.Y += 1;
    g_Console.writeToBuffer(c, "Food ($30) [ ] ", 0xF0);
    if (cPtr[0]->getStatusFed() == true) {
        c.X += 12;
        g_Console.writeToBuffer(c, " ", 0x00);
        c.X -= 12;
    }
    c.Y += 2;
    g_Console.writeToBuffer(c, "!=========Son 2=========!", 0xF0);
    c.Y += 1;
    g_Console.writeToBuffer(c, "State : ", 0xF0);
    c.X += 8;
    if (cPtr[1]->getStatus() == true) {
        g_Console.writeToBuffer(c, "(T//-//T )", 0xF0);
    }
    else {
        g_Console.writeToBuffer(c, "(^ - ^ 6)", 0xF0);
    }
    c.X -= 8;
    c.Y += 1;
    if (cPtr[1]->getStatus() == true) {
        g_Console.writeToBuffer(c, "Medicine ($100) [ ]", 0xF0);
        if (cPtr[1]->getTreatState() == true) {
            c.X += 17;
            g_Console.writeToBuffer(c, " ", 0x00);
            c.X -= 17;
        }
    }
    c.Y += 1;
    g_Console.writeToBuffer(c, "Food ($30) [ ] ", 0xF0);
    if (cPtr[1]->getStatusFed() == true) {
        c.X += 12;
        g_Console.writeToBuffer(c, " ", 0x00);
        c.X -= 12;
    }
    c.Y += 2;
    if (day % 6 == 0 && day != 0) {
        g_Console.writeToBuffer(c, "!====Rent ($200) [ ]====!", 0xF0);
        if (p.getRentStatus() == true) {
            c.X += 18;
            g_Console.writeToBuffer(c, " ", 0x00);
            c.X -= 18;
        }
    }
}

void renderEndOfWorkScreen()
{
    map.chooseMap(7, g_Console);
    COORD c = g_Console.getConsoleSize();
    std::ostringstream ss;
    
    c.Y /= 25;
    c.X = c.X / 2 - 10;
    g_Console.writeToBuffer(c, "End of day report", 0xF0);
    c.Y += 8;
    c.X = g_Console.getConsoleSize().X / 6 + 15;
    ss.str("");
    ss << "Customers served: ";
    g_Console.writeToBuffer(c, ss.str(), 0xF0);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 6 + 15;
    ss.str("");
    ss << "Complaints given: " << p.getUnsatisfiedCustomers();
    g_Console.writeToBuffer(c, ss.str(), 0xF0);
    /*c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 6 + 15;*/
    /*ss.str("");
    ss << "Total number of Strikes: "<< p.getStrikes();
    g_Console.writeToBuffer(c, ss.str(), 0xF0);*/
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 6 + 15;
    ss.str("");
    ss << "Today's pay: $" << p.getTotalEarned();
    g_Console.writeToBuffer(c, ss.str(), 0xF0);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 6 + 15;
    g_Console.writeToBuffer(c, "Click here to go home", 0xF0);
}

void renderGameOver()
{
    COORD c = g_Console.getConsoleSize();
    map.chooseMap(8, g_Console);
    c.Y /= 25;
    c.X = g_Console.getConsoleSize().X / 3 + 8;
    g_Console.writeToBuffer(c, "Game Over!", 0xF0);
    c.Y += 6;
    c.X = g_Console.getConsoleSize().X / 3 + 2;
    for (int i = 0; i < 2; i++) {
        if (cPtr[i]->getHospState() == true)
        {
            g_Console.writeToBuffer(c, "     One of your", 0xF0);
            c.Y++;
            g_Console.writeToBuffer(c, "sons was hospitalised!", 0xF0); i++; //stop double print if both sons sick
        }
    }
    if (g_eGameState == S_GAMEOVER && p.getUnsatisfiedCustomers() == 10)
    {
        g_Console.writeToBuffer(c, "You got too many complaints (10)!", 0xF0);
    }
    if (g_eGameState == S_GAMEOVER && p.getRentStatus() == false) {
        g_Console.writeToBuffer(c, "Your landlord was not", 0xF0);
        c.Y += 1;
        g_Console.writeToBuffer(c, "as kind as you thought.", 0xF0);
        c.Y += 1;
        g_Console.writeToBuffer(c, "You were evicted", 0xF0);
        c.Y += 1;
        g_Console.writeToBuffer(c, "for not paying rent!", 0xF0);
    }
    c.Y += 6;
    c.X = g_Console.getConsoleSize().X / 3 + 2;
    g_Console.writeToBuffer(c, " Press [ESC] to head", 0xF0);
    c.Y++;
    g_Console.writeToBuffer(c, "back to the main menu!", 0xF0);
}

void renderTutorialLevel()
{
    COORD c = g_Console.getConsoleSize();
    std::ostringstream ss;
    map.chooseMap(1, g_Console);
    renderCharacter();  // renders the character into the buffer
    renderBoxes();
    renderShelfAmount();
    renderHUD();
    
    /*
    if (tutorialFlags[0] == false)
    {
        ss.str("");
        ss << "Welcome to Jackville Supermarket! As it is your first day on the job,";
        c.Y = 4;
        c.X = 60;
        g_Console.writeToBuffer(c, ss.str(), 0xF0);
        ss.str("");
        ss << "I will tell you what you have to do. Click the screen to continue.";
        c.Y += 1;
        g_Console.writeToBuffer(c, ss.str(), 0xF0);
    }
    else if (tutorialFlags[1] == false)
    {
        ss.str("");
        ss << "Use your WASD keys to move up,";
        c.Y = 4;
        c.X = 41;
        g_Console.writeToBuffer(c, ss.str(), 0xF0);
        ss.str("");
        ss << "left, down and right respectively";
        c.Y += 1;
        g_Console.writeToBuffer(c, ss.str(), 0xF0);
    }
    if ((g_mouseEvent.buttonState == DOUBLE_CLICK) && (tutorialFlags[0] == false) && (g_eGameState == S_TUT))
        tutorialFlags[0] = true;
        */

    renderCustomer();
}

void renderBoxes()
{
    g_Console.writeToBuffer(boxPosPtr[0]->getX(), boxPosPtr[0]->getY(), ' ', BoxColour);
    for (int i = 0; i < 6; i++)
    {
        if (boxPtr[i + 1] != nullptr)
        {
            g_Console.writeToBuffer(boxPosPtr[i + 1]->getX(), boxPosPtr[i + 1]->getY(), ' ', CustomerBoxColour[i]);
        }
    }
    //for (int i = 0; i < 6; i++) { //maybe useful
    //    switch ('1') {
    //        case '0': {g_Console.writeToBuffer(boxPosPtr[i]->getX(), boxPosPtr[i]->getY(), char(221), BoxColour);
    //        g_Console.writeToBuffer(boxPosPtr[i]->getX(), boxPosPtr[i]->getY(), char(222), BoxColour); }

    //        case '1': {g_Console.writeToBuffer(boxPosPtr[i]->getX(), boxPosPtr[i]->getY(), char(220), BoxColour);
    //        g_Console.writeToBuffer(boxPosPtr[i]->getX(), boxPosPtr[i]->getY(), char(223), BoxColour); }

    //        case '2': {g_Console.writeToBuffer(boxPosPtr[i]->getX(), boxPosPtr[i]->getY(), char(222), BoxColour);
    //        g_Console.writeToBuffer(boxPosPtr[i]->getX(), boxPosPtr[0]->getY(), char(221), BoxColour); }

    //        case '3': {g_Console.writeToBuffer(boxPosPtr[i]->getX(), boxPosPtr[i]->getY(), char(223), BoxColour);
    //        g_Console.writeToBuffer(boxPosPtr[i]->getX(), boxPosPtr[i]->getY(), char(220), BoxColour); }
    //    }
    //}
}

//void moveCustomer()
//{
//    //todo
//}

void renderCustomer() // fix later yes ues
{   
    if (g_bRestocking == false) {
        COORD c = g_Console.getConsoleSize();
        bool created = false;

        for (int i = 0; i < 6; i++)
        {
            if (customerPtr[i] != nullptr)
            {
                switch (customerPtr[i]->getItemToBuy())
                {
                case 1:
                    if (timer[i] >= 10.9 || timer[i] <= 11.1)
                    {
                        customerPtr[i]->moveToShelfContainingItem(customerPtr[i]->getItemToBuy());
                        travelling[i] = true;
                        break;
                    }

                case 2:
                    if (timer[i] >= 10.9 || timer[i] <= 11.1)
                    {
                        customerPtr[i]->moveToShelfContainingItem(customerPtr[i]->getItemToBuy());
                        travelling[i] = true;
                        break;
                    }
                }

                if (travelling[i] == true)
                {
                    customerDirection[i] = customerPtr[i]->moveCustomer(map, framesPassed, 4);
                    customerPtr[i]->customerCollision(map, travelling[i], avoiding[i]);
                }
                    
                else if (avoiding[i] == 4 || avoiding[i] == 8)
                {
                    avoiding[i] = 0;
                    travelling[i] = true;
                }

                customerPtr[i]->bumpIntoCustomer(avoiding[i], map);
                    
                customerPtr[i]->printOutCustomer(spawned[i], g_Console, customerPtr[i]->getPos(), map, customerPtr[i]->getQuantity());
                
                if (boxPtr[i + 1] == nullptr) {
                    boxPtr[i + 1] = new Box;
                    boxPosPtr[i + 1] = new Position;
                }

                CustomerBoxColour[i] = 0x77;
                
                if (boxPosPtr[i + 1] != nullptr) {
                    switch (customerDirection[i])
                    {
                    case 0: boxPosPtr[i + 1]->setX((customerPtr[i]->getX()) + 1);
                        boxPosPtr[i + 1]->setY((customerPtr[i]->getY()) + 0); break;
                    
                    case 1: boxPosPtr[i + 1]->setX((customerPtr[i]->getX()) + 0);
                        boxPosPtr[i + 1]->setY((customerPtr[i]->getY()) - 1); break;

                    case 2:boxPosPtr[i + 1]->setX((customerPtr[i]->getX()) - 1);
                        boxPosPtr[i + 1]->setY((customerPtr[i]->getY()) + 0); break;

                    case 3:boxPosPtr[i + 1]->setX((customerPtr[i]->getX()) + 0);
                        boxPosPtr[i + 1]->setY((customerPtr[i]->getY()) + 1); break;
                    }
                }
                

                if ((timer[i] >= 30.95) && (timer[i] <= 31.05))
                {
                    bool bComplain = false;
                    

                    for (int j = 0; j < 3; j++) {

                        if (sPtr[j] != nullptr) {

                            if (customerPtr[i]->getX() == 37 && customerPtr[i]->getY() == 7 + 6 * j) {

                                if (sPtr[j]->getAmount() >= customerPtr[i]->getQuantity())
                                {
                                    sPtr[j]->decreaseItem(customerPtr[i]->getQuantity());

                                    p.AddDayEarnings(customerPtr[i]->getQuantity()); //for adding amount earned daily// can change it if need be

                                    switch (j) 
                                    {
                                        case 0:
                                            CustomerBoxColour[i] = 0x50;
                                            break;
                                        case 1:
                                            CustomerBoxColour[i] = 0x10;
                                            break;
                                        case 3:
                                            CustomerBoxColour[i] = 0xB0;
                                            break;
                                    }
                                }

                                else if (sPtr[j]->getAmount() < customerPtr[i]->getQuantity()) {
                                    p.increaseUnsatisfiedCustomers();

                                }

                            }

                        }
                    }

                    for (int j = 3; j < 6; j++) {

                        if (sPtr[j] != nullptr) {

                            if (customerPtr[i]->getX() == 58 && customerPtr[i]->getY() == 7 + 6 * j) {

                                if (sPtr[j]->getAmount() >= customerPtr[i]->getQuantity())
                                {
                                    sPtr[j]->decreaseItem(customerPtr[i]->getQuantity());


                                    p.AddDayEarnings(customerPtr[i]->getQuantity()); //for adding amount earned daily// can change it if need be

                                    switch (j)
                                    {
                                    case 3:
                                        CustomerBoxColour[i] = 0xE0;
                                        break;
                                    case 4:
                                        CustomerBoxColour[i] = 0xA0;
                                        break;
                                    case 5:
                                        CustomerBoxColour[i] = 0x90;
                                        break;
                                    }


                                }

                                else if (sPtr[j]->getAmount() < customerPtr[i]->getQuantity()) {
                                    p.increaseUnsatisfiedCustomers();

                                }

                            }

                        }

                    }

                    customerPtr[i]->setEndPoint(79, 15);
                    //customerPtr[i]->setPos(customerPtr[i]->getPos().getX(), customerPtr[i]->getPos().getY() + 1);
                }

                if (customerPtr[i]->getPos().getX() == 79 && customerPtr[i]->getPos().getY() == 15)
                {
                    spawned[i] = false;
                    delete customerPtr[i];
                    customerPtr[i] = nullptr;

                    delete boxPtr[i + 1];
                    boxPtr[i + 1] = nullptr;
                    delete boxPosPtr[i + 1];
                    boxPosPtr[i + 1] = nullptr;

                    CustomerBoxColour[i] = 0x77;

                    timer[i] = -1;
                    travelling[i] = false;
                }
            }
            else
            {
                if (spawnTimer >= 5)
                {
                    if (created != true)
                    {
                        customerPtr[i] = new Customer;
                        customerPtr[i]->setItemToBuy(2);
                        timer[i] = 0;
                        customerPtr[i]->setPos(customerPtr[i]->getPos().getX(), customerPtr[i]->getPos().getY() + i);
                        spawned[i] = true;
                        created = true;
                    }
                    spawnTimer = 0;
                }
            }
        }
        if (spawnTimer >= 5)
            spawnTimer = 0;
    }
}

void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x99;
    if (g_sChar.m_bActive)
    {
        charColor = 0xCC;
    }
    g_Console.writeToBuffer(g_sChar.m_cLocation, ' ', charColor);
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 36; //change to shift location of timer
    c.Y = 24;  //we might use this or we might need to make a new timer to show when the game starts
    g_Console.writeToBuffer(c, ss.str(), 0x59);

    // temporary: displays test customer position
    /*
    ss.str("");
    ss << "Customer Coords";
    c.X = 0; //change to shift location of timer
    c.Y = 14;  //we might use this or we might need to make a new timer to show when the game starts
    g_Console.writeToBuffer(c, ss.str(), 0x0F);
    c.Y += 1;
    
    ss.str("");
    ss << "X: "<< testCustomer.getPos().getX();
    g_Console.writeToBuffer(c, ss.str(), 0x0F);
    c.Y += 1;
    ss.str("");
    ss << "Y: " << testCustomer.getPos().getY();
    g_Console.writeToBuffer(c, ss.str(), 0x0F);
    */
    ss.str("");
    ss << spawnTimer;
    c.X = 0;
    c.Y = 23;
    g_Console.writeToBuffer(c, ss.str(), 0x0F);


    for (int i = 0; i < 6; i++)
    {
        ss.str("");
        double display = timer[i];
        ss << display;
        c.X = 0;
        c.Y = 14 + i;
        g_Console.writeToBuffer(c, ss.str(), 0x0F);

        if (customerPtr[i] != nullptr)
        {
            ss.str("");
            ss << customerPtr[i] << " pos:" << customerPtr[i]->getPos().getX() << ", " << customerPtr[i]->getPos().getY();
            c.X = 30;
            c.Y = 14 + i;
            g_Console.writeToBuffer(c, ss.str(), 0x0F);
        }
    }
}

// this is an example of how you would use the input events
void renderInputEvents()
{
    // keyboard events
    COORD startPos = g_Console.getConsoleSize();
    startPos.Y /= 25;
    startPos.X = startPos.X - 20;
    std::ostringstream ss;
    std::string key;
    for (int i = 0; i < K_COUNT; ++i)
    {
        ss.str("");
        switch (i)
        {
        case K_UP: key = "UP";
            break;
        case K_DOWN: key = "DOWN";
            break;
        case K_LEFT: key = "LEFT";
            break;
        case K_RIGHT: key = "RIGHT";
            break;
        case K_SPACE: key = "SPACE";
            break;
        default: continue;
        }
        if (g_skKeyEvent[i].keyDown)
            ss << key << " pressed";
        else if (g_skKeyEvent[i].keyReleased)
            ss << key << " released";
        else
            ss << key << " not pressed";

        COORD c = { startPos.X, startPos.Y + i };
        g_Console.writeToBuffer(c, ss.str(), 0x17);
    }

    // mouse events    
    ss.str("");
    ss << "Mouse position (" << g_mouseEvent.mousePosition.X << ", " << g_mouseEvent.mousePosition.Y << ")";
    g_Console.writeToBuffer(g_mouseEvent.mousePosition, ss.str(), 0x59);
    ss.str("");
    switch (g_mouseEvent.buttonState)
    {
    case FROM_LEFT_1ST_BUTTON_PRESSED:
    { ss.str("LMB PRESSED"); g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 1, ss.str(), 0x59); break; }
    case RIGHTMOST_BUTTON_PRESSED:
    { ss.str("RMB PRESSED"); g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 2, ss.str(), 0x59); break; }
    }
    
    switch (g_mouseEvent.eventFlags)
    {
    case DOUBLE_CLICK:
    { ss.str("DBL CLICKED"); g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 3, ss.str(), 0x59); break; }
    case MOUSE_WHEELED:
    { ss.str("MOUSE WHEEL"); g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 4, ss.str(), 0x59); break; }
    }
    
}
