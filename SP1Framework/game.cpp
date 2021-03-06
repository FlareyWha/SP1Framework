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
#include "Tutorial.h"

//Sound dependencies
#include <Windows.h>
#pragma comment(lib, "winmm.lib")

double  g_dElapsedTime;
double g_dElapsedWorkTime;
double  g_dDeltaTime;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;

// Game specific variables here
int level, day;
bool g_bRestocking;
bool saveSuccessful;
bool loadSuccessful;

double spawnTimer;
SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state
EGAMESTATES g_ePreviousGameState = S_SPLASHSCREEN; // initial state
EDEBUGSTATES g_eDebugState = D_OFF; // initial state

Customer* customerPtr[10] = {nullptr , nullptr , nullptr , nullptr , nullptr , nullptr, nullptr , nullptr , nullptr , nullptr }; // spawn customer
Shelf* sPtr[6] = { nullptr , nullptr , nullptr , nullptr , nullptr , nullptr };// spawn shelf
Son* cPtr[2] = { nullptr, nullptr };

Player p;
bool playerSpeedToggle;
int playerWalkSpeed;
int playerSpeed;

Box* boxPtr[11] = { nullptr, nullptr, nullptr, nullptr, nullptr , nullptr, nullptr, nullptr, nullptr , nullptr, nullptr }; //spawn player and customer box
Position* boxPosPtr[11] = { nullptr, nullptr, nullptr, nullptr, nullptr , nullptr, nullptr, nullptr, nullptr , nullptr, nullptr }; //spawn player and customer box positions
WORD BoxColour; //player box colour
Map map;
int framesPassed;
int frameMarker;

int customerDirection[10];
WORD CustomerBoxColour[10]; //customer box position
int customerMultiplier;
bool increased;
int total;
bool totalLock;

//tutorial stuff;
Tutorial tutorial;
double tutorialTimer;

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

void deleteEverything() //delete everything when player exists game
{
    deleteCustomer(); //delete customers
    deleteBoxes(); //delete customer boxes & positions

    //delete player box
    if (boxPtr[0] != nullptr)
    {
        delete boxPtr[0];
        boxPtr[0] = nullptr;
    }
    if (boxPosPtr[0] != nullptr)
    {
        delete boxPosPtr[0];
        boxPosPtr[0] = nullptr;

    }

    //delete shelves
    for (int i = 0; i < 6; i++)
    {
        if (sPtr[i] != nullptr)
        {
           delete sPtr[i];
           sPtr[i] = nullptr;
        }
    }
    
    //delete Sons
    for (int i = 0; i < 2;i++)
    {
        if (cPtr[i] != nullptr)
        {
            delete cPtr[i];
            cPtr[i] = nullptr;
        }
    }
    


}

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


    //init Son objects
    cPtr[0] = new Son;
    cPtr[1] = new Son;

    //init shelf

    if (sPtr[0] == nullptr && map.getGrid(3, 1) != 'A') { //purple
        sPtr[0] = new Shelf;
        sPtr[0]->setShelf(0x50);
    }
    if (sPtr[1] == nullptr && map.getGrid(4, 1) != 'A') { //dark blue
        sPtr[1] = new Shelf;
        sPtr[1]->setShelf(0x10);
    }
    if (sPtr[2] == nullptr && map.getGrid(5, 1) != 'A') { //teal
        sPtr[2] = new Shelf;
        sPtr[2]->setShelf(0xB0);
    }
    if (sPtr[3] == nullptr && map.getGrid(6, 1) != 'A') { //cream
        sPtr[3] = new Shelf;
        sPtr[3]->setShelf(0xE0);
    }
    if (sPtr[4] == nullptr && map.getGrid(7, 1) != 'A') { //green
        sPtr[4] = new Shelf;
        sPtr[4]->setShelf(0xA0);
    }
    if (sPtr[5] == nullptr && map.getGrid(8, 1) != 'A') { //blue
        sPtr[5] = new Shelf;
        sPtr[5]->setShelf(0x90);
    }

    customerMultiplier = 6;
    increased = false;
    total = 0;
    totalLock = false;

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
    deleteEverything();

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
    case S_STORE: gameplayKBHandler(keyboardEvent);
        break;
    case S_TUT: gameplayKBHandler(keyboardEvent);
        break;
    case S_GAME: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
        break;
    case S_CREDITS: gameplayKBHandler(keyboardEvent);
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
    case S_STORE: gameplayMouseHandler(mouseEvent);
        break;
    case S_TUT: gameplayMouseHandler(mouseEvent);
        break;
    case S_GAME: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    case S_CREDITS: gameplayMouseHandler(mouseEvent);
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
    case 0x57: key = K_UP; break; // up "W"
    case 0x53: key = K_DOWN; break; //down "S"
    case 0x41: key = K_LEFT; break; // left "A"
    case 0x44: key = K_RIGHT; break; // right "D"
    case VK_SPACE: key = K_SPACE; break; // restocking space
    case VK_ESCAPE: key = K_ESCAPE; break; // pause menu escape 
    case VK_SHIFT: key = K_SHIFT; break; //slow player shift

    //cheats
    case VK_F3: key = K_F3; break; //debugging
    case VK_F4: key = K_F4; break; //skip level
    case VK_F5: key = K_F5; break; //free money
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
        case S_MENU: updateMenu(); // game logic for menu screen
            break;
        case S_ENDOFWORKSCREEN: updateEndofWorkScreen(); // game logic for end of work screen
            break;
        case S_GAMEOVER: updateGameOver(); // game logic for Game over screen
            break;
        case S_HOME: updateHome(); // game logic for home screen
            break;
        case S_STORE: updateStore(); // game logic for store screen
            break;
        case S_TUT: { // game logic for tutorial phase of game
            spawnTimer += dt;
            for (int i = 0; i < 10; i++)
            {
                if (customerPtr[i] != nullptr && customerPtr[i]->getTimer() != -1)
                {
                    customerPtr[i]->addTimer(dt);
                }
            }
            if (tutorial.getTutorialFlag(5) == true)
                g_dElapsedWorkTime += dt; 

            tutorialTimer += dt;
            updateTutorial();
            break;
        }
        case S_GAME: { // game logic for the main game
            spawnTimer += dt;
            for (int i = 0; i < 10; i++)
            {
                if (customerPtr[i] != nullptr && customerPtr[i]->getTimer() != -1)
                {
                    customerPtr[i]->addTimer(dt);
                }
            }
            g_dElapsedWorkTime += dt; 

            if (tutorial.getComplete() == false)
                tutorialTimer += dt;
            updateGame();// gameplay logic when we are in the game
            break;
        }
        case S_CREDITS: {
            updateCredits();
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

void updateEndofWorkScreen() // End of work screen logic
{
    processUserInput();
    
}

void updateGameOver() // Game over screen logic
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
    g_ePreviousGameState = g_eGameState; // save previous game state

    if (tutorial.getProceed() == true)
    {
        processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
        moveCharacter();    // moves the character
        actuallyMoving(); // collision detection and player speed variables
        pickUpBoxes(); // Picking up of boxes
        restockShelf(); // All logic related to restocking shelves
    }
}

void updateGame()       // game logic
{
    g_ePreviousGameState = g_eGameState;
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter(); // move the character
    actuallyMoving(); // used for collision detection;
    pickUpBoxes(); // Picking up of boxes
    restockShelf(); // All logic related to restocking shelves
}

// Store screen logic
void updateStore()
{
    processUserInput();
}

void updateCredits()
{
    processUserInput();
}

//to check if the player is pressing a key
void moveCharacter()
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
    if (g_skKeyEvent[K_SHIFT].keyDown)
    {
        playerSpeedToggle = true;
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
    if (g_skKeyEvent[K_SHIFT].keyReleased)
    {
        playerSpeedToggle = false;
    }
}

// Check if customer and player's boxes have collided with each other
void checkCustomerPlayerCollision()
{
    for (int i = 1; i < 7; i++)
    {
        if (boxPosPtr[i] != nullptr)
        {
            if (g_sChar.m_cLocation.X == boxPosPtr[i]->getX() && g_sChar.m_cLocation.Y == boxPosPtr[i]->getY())
            {
                if (g_sChar.m_cLocation.X == boxPosPtr[0]->getX() && g_sChar.m_cLocation.Y == boxPosPtr[0]->getY() - 1) {
                    g_sChar.m_cLocation.Y--;
                    boxPosPtr[0]->setY(g_sChar.m_cLocation.Y + 1);
                }
                else if (g_sChar.m_cLocation.X == boxPosPtr[0]->getX() && g_sChar.m_cLocation.Y == boxPosPtr[0]->getY() + 1) {
                    g_sChar.m_cLocation.Y++;
                    boxPosPtr[0]->setY(g_sChar.m_cLocation.Y - 1);
                }
                else
                {
                    boxPosPtr[0]->setY(g_sChar.m_cLocation.Y + 1);
                    g_sChar.m_cLocation.Y++;
                }

            }

            else if (boxPosPtr[0]->getX() == boxPosPtr[i]->getX() && boxPosPtr[0]->getY() == boxPosPtr[i]->getY())
            {
                if (g_sChar.m_cLocation.X == boxPosPtr[0]->getX() && g_sChar.m_cLocation.Y == boxPosPtr[0]->getY() - 1)
                {
                    g_sChar.m_cLocation.Y--;
                    boxPosPtr[0]->setY(g_sChar.m_cLocation.Y + 1);
                }
                else if (g_sChar.m_cLocation.X == boxPosPtr[0]->getX() && g_sChar.m_cLocation.Y == boxPosPtr[0]->getY() + 1) {
                    g_sChar.m_cLocation.Y++;
                    boxPosPtr[0]->setY(g_sChar.m_cLocation.Y - 1);
                }
                else if (g_sChar.m_cLocation.X == boxPosPtr[0]->getX() + 1 && g_sChar.m_cLocation.Y == boxPosPtr[0]->getY())
                {

                    if (customerPtr[i - 1]->getX() == boxPosPtr[i]->getX() - 1)
                    {
                        boxPosPtr[0]->setY(g_sChar.m_cLocation.Y + 1);
                        g_sChar.m_cLocation.Y++;
                    }
                    else
                    {
                        g_sChar.m_cLocation.X++;
                        boxPosPtr[0]->setX(g_sChar.m_cLocation.X - 1);
                    }


                }
                else if (g_sChar.m_cLocation.X == boxPosPtr[0]->getX() - 1 && g_sChar.m_cLocation.Y == boxPosPtr[0]->getY())
                {

                    if (boxPosPtr[i]->getX() == 36 || boxPosPtr[i]->getX() == 57)
                    {
                        g_sChar.m_cLocation.X--;
                        boxPosPtr[0]->setX(g_sChar.m_cLocation.X + 1);
                    }
                    else
                    {
                        boxPosPtr[0]->setY(g_sChar.m_cLocation.Y + 1);
                        g_sChar.m_cLocation.Y++;
                    }
                }
            }

        }

    }
}

// Change player speed according to Cooler shoes perk level and player-box collision detection
void actuallyMoving()
{
    if (playerSpeedToggle == true) 
    { 
        playerSpeed = 8; 
    }
    else 
    { 
        playerSpeed = 5 - (p.getPowerups()->getShoeslvl());
    }
    if ( framesPassed % playerSpeed == 0) {
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
                p.setPos(p.getPos().getX(), p.getPos().getY() - 1);
            }
            boxPosPtr[0]->setX(g_sChar.m_cLocation.X);
            boxPosPtr[0]->setY(g_sChar.m_cLocation.Y - 1);
            if (map.collision(boxPosPtr[0]->getY(), boxPosPtr[0]->getX(), 0, 0, map) != '0')
            {
                g_sChar.m_cLocation.Y++;
                p.setPos(p.getPos().getX(), p.getPos().getY() + 1);
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
                p.setPos(p.getPos().getX(), p.getPos().getY() + 1);
            }
            boxPosPtr[0]->setX(g_sChar.m_cLocation.X);
            boxPosPtr[0]->setY(g_sChar.m_cLocation.Y + 1);
            if (map.collision(boxPosPtr[0]->getY(), boxPosPtr[0]->getX(), 0, 0, map) != '0')
            {
                g_sChar.m_cLocation.Y--;
                p.setPos(p.getPos().getX(), p.getPos().getY() - 1);
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
                p.setPos(p.getPos().getX() - 1, p.getPos().getY());
            }
            boxPosPtr[0]->setX(g_sChar.m_cLocation.X - 1);
            boxPosPtr[0]->setY(g_sChar.m_cLocation.Y);
            if (map.collision(boxPosPtr[0]->getY(), boxPosPtr[0]->getX(), 0, 0, map) != '0')
            {
                g_sChar.m_cLocation.X++;
                p.setPos(p.getPos().getX() + 1, p.getPos().getY());
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
                p.setPos(p.getPos().getX() + 1, p.getPos().getY());
            }
            boxPosPtr[0]->setX(g_sChar.m_cLocation.X + 1);
            boxPosPtr[0]->setY(g_sChar.m_cLocation.Y);
            if (map.collision(boxPosPtr[0]->getY(), boxPosPtr[0]->getX(), 0, 0, map) != '0')
            {
                g_sChar.m_cLocation.X--;
                p.setPos(p.getPos().getX() - 1, p.getPos().getY());
                boxPosPtr[0]->setX(g_sChar.m_cLocation.X + 1);
            }
            break;
        case false:
            break;
        }

        

        
    }
}

//register item picked up and update box player is carrying accordingly
void pickUpBoxes()
{
    if (p.isHoldingProduct() == false) {
        BoxColour = 0x70; //empty box grey
        boxPtr[0]->setTag(0);
    }

    if (g_skKeyEvent[K_SPACE].keyReleased && boxPosPtr[0]->getX() == 2 && boxPosPtr[0]->getY() == 3 && map.getGrid(3, 1) != 'A')
    {
        BoxColour = 0x50; //toilet paper purple
        p.holdsProduct();
        boxPtr[0]->setTag(2);
    }
    if (g_skKeyEvent[K_SPACE].keyReleased && boxPosPtr[0]->getX() == 2 && boxPosPtr[0]->getY() == 4 && map.getGrid(4, 1) != 'A')
    {
        BoxColour = 0x10; //instant noodle dark blue          
        p.holdsProduct();
        boxPtr[0]->setTag(4);
    }
    if (g_skKeyEvent[K_SPACE].keyReleased && boxPosPtr[0]->getX() == 2 && boxPosPtr[0]->getY() == 5 && map.getGrid(5, 1) != 'A')
    {
        BoxColour = 0xB0; //canned food teal
        p.holdsProduct();
        boxPtr[0]->setTag(3);
    }
    if (g_skKeyEvent[K_SPACE].keyReleased && boxPosPtr[0]->getX() == 2 && boxPosPtr[0]->getY() == 6 && map.getGrid(6, 1) != 'A')
    {
        BoxColour = 0xE0; //rice cream
        p.holdsProduct();
        boxPtr[0]->setTag(1);
    }
    if (g_skKeyEvent[K_SPACE].keyReleased && boxPosPtr[0]->getX() == 2 && boxPosPtr[0]->getY() == 7 && map.getGrid(7, 1) != 'A')
    {
        BoxColour = 0xA0; //vegetable green
        p.holdsProduct();
        boxPtr[0]->setTag(5);
    }
    if (g_skKeyEvent[K_SPACE].keyReleased && boxPosPtr[0]->getX() == 2 && boxPosPtr[0]->getY() == 8 && map.getGrid(8, 1) != 'A')
    {
        BoxColour = 0x90;//bandages blue
        p.holdsProduct();
        boxPtr[0]->setTag(6);
    }
}



// Register restock inputs from player and updates
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



// Update son object variables and keep track of well-being state
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
            cPtr[i]->resetTreatState();
        }
    }
}

// Process mouse inputs in Store screen
void processStoreinput()
{
    framesPassed++;
    Player* pPtr = &p;
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED
        && framesPassed % 5 == 0) {
        if (g_mouseEvent.mousePosition.X >= 35
            && g_mouseEvent.mousePosition.X <= 38
            && g_mouseEvent.mousePosition.Y == 22) {
            g_eGameState = S_HOME;
        }
        else if (g_mouseEvent.mousePosition.X >= 27
            && g_mouseEvent.mousePosition.X <= 38
            && g_mouseEvent.mousePosition.Y == 14
            && day >= 0) {
            if (p.getSavings() >= 50) {
                p.purchasecheaperFood(pPtr);
            }
        }
        else if (g_mouseEvent.mousePosition.X >= 27
            && g_mouseEvent.mousePosition.X <= 38
            && g_mouseEvent.mousePosition.Y == 16
            && day >= 1) {
            if (p.getSavings() >= 50) {
                p.purchasecheaperRent(pPtr);
            }
        }
        else if (g_mouseEvent.mousePosition.X >= 27
            && g_mouseEvent.mousePosition.X <= 38
            && g_mouseEvent.mousePosition.Y == 10
            && day >= 5) {
            if (p.getSavings() >= 100) {
                p.purchaseplayerShoes(pPtr);
            }
        }
        else if (g_mouseEvent.mousePosition.X >= 27
            && g_mouseEvent.mousePosition.X <= 42
            && g_mouseEvent.mousePosition.Y == 8
            && day >= 4) {
            if (p.getSavings() >= 100) {
                p.purchaseslowerCustomers(pPtr);
            }
        }
        else if (g_mouseEvent.mousePosition.X >= 27
            && g_mouseEvent.mousePosition.X <= 40
            && g_mouseEvent.mousePosition.Y == 18
            && day >= 2) {
            if (p.getSavings() >= 50) {
                p.purchaserichCustomers(pPtr);
            }
        }
        else if (g_mouseEvent.mousePosition.X >= 27
            && g_mouseEvent.mousePosition.X <= 43
            && g_mouseEvent.mousePosition.Y == 20
            && day >= 3) {
            if (p.getSavings() >= 50) {
                p.purchasethriftyCustomers(pPtr);
            }
        }
    }
}

// Delete customers and reset their variables
void deleteCustomer()
{
    for (int i = 0; i < 10; i++)
    {
        if (customerPtr[i] != nullptr)
        {
            delete customerPtr[i];
            customerPtr[i] = nullptr;
        }
    }
}



// Delete customer boxes
void deleteBoxes()
{
    for (int i = 1; i < 11; i++)
    {
        if (boxPtr[i] != nullptr)
        {
            delete boxPtr[i];
            boxPtr[i] = nullptr;

        }

        if (boxPosPtr[i] != nullptr)
        {
            delete boxPosPtr[i];
            boxPosPtr[i] = nullptr;
        }
    }
}

// Check if day has ended and if lose conditions have been met; Reset variables
void checkEnd() //Check if day has ended and update variables as well as game over conditions
{
    if (tutorial.getTutorialFlag(14) == false) {
        tutorial.tutorial(g_Console, g_sChar, g_mouseEvent, g_skKeyEvent, g_dElapsedWorkTime, p, BoxColour, tutorialTimer, g_eGameState);
    }
    if (p.getUnsatisfiedCustomers() == 10) { // change back ltr to 10
            g_eGameState = S_GAMEOVER;
            deleteCustomer();
            deleteBoxes();
    }
    else if (g_dElapsedWorkTime >= 30) { //change back ltr to 30
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
        increased = false;
        p.setPos(18, 1);
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
        }
        if (p.getRentStatus() == true && day % 7 == 0 && day != 0) {
            g_eGameState = S_ENDOFWORKSCREEN;
            p.isRentPaid();
        }
        else if (p.getRentStatus() == false && day % 7 == 0 && day != 0) {
            g_eGameState = S_GAMEOVER;
        }
    }
    
}

// Process debug inputs
void processDebugState() //Toggle debug options
{
    if (g_skKeyEvent[K_F3].keyReleased)
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
    if (g_skKeyEvent[K_F5].keyReleased) {
        p.receivePay(5000);
    }
}

// Process inputs on splash screen
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
            g_eGameState = S_CREDITS;
        }
    }
}

void processInputCredits()
{
    framesPassed++;
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED
        && framesPassed % 10 == 0) {
        g_eGameState = S_MENU;
    }
}

// Process inputs on menu screen
void processInputMenu() //All input processing related to Main Menu
{
    framesPassed++;
    static bool alreadyPlayed = false;
    if (!alreadyPlayed && (g_ePreviousGameState != S_HOME && g_ePreviousGameState != S_SPLASHSCREEN)) {
        PlaySound(L"BGM.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        alreadyPlayed = true;
    }
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
            g_ePreviousGameState = S_MENU;
            alreadyPlayed = false;
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
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {
        if ((g_mouseEvent.mousePosition.X >= 33
            && g_mouseEvent.mousePosition.X <= 36)
            && g_mouseEvent.mousePosition.Y == 10
            && g_ePreviousGameState == S_HOME) //saves game
        {
            std::ofstream outdata; // outdata is like cin
            Powerup* PowerupsPtr = p.getPowerups();
            outdata.open("Save.dat"); // opens the file
            outdata << "Day = " << day << std::endl;
            outdata << "Savings = " << p.getSavings() << std::endl;
            outdata << "Son One State = " << cPtr[0]->getStatus() << std::endl;
            outdata << "Son Two State = " << cPtr[1]->getStatus() << std::endl;
            outdata << "Cheaper Food = " << PowerupsPtr->getFoodlvl() << std::endl;
            outdata << "Cheaper Rent = " << PowerupsPtr->getRentlvl() << std::endl;
            outdata << "Player Shoes = " << PowerupsPtr->getShoeslvl() << std::endl;
            outdata << "Slower Customers = " << PowerupsPtr->getSCustomerslvl() << std::endl;
            outdata << "Rich Customers = " << PowerupsPtr->getRCustomerslvl() << std::endl;
            outdata << "Thrifty Customers = " << PowerupsPtr->getTCustomerslvl() << std::endl;
            outdata.close();

            saveSuccessful = true;
        }
    }
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {
        if ((g_mouseEvent.mousePosition.X >= 33
            && g_mouseEvent.mousePosition.X <= 36)
            && g_mouseEvent.mousePosition.Y == 11
            && (g_ePreviousGameState == S_HOME || g_ePreviousGameState == S_TUT || g_ePreviousGameState == S_GAME)) //loads game
        {
            std::fstream indata; // indata is like cout
            indata.open("Save.dat"); // opens the file
            std::string line;
            for (int x = 0; x < 10; x++) {
                getline(indata, line);
                loadValues(x, line);
            }
            indata.close();
            loadSuccessful = true;
            if (loadSuccessful) {
                loadSuccessful = false;
                g_eGameState = S_HOME;
                g_ePreviousGameState = S_MENU;
                for (int x = 0; x < 15; x++) {
                    tutorial.setTutorialFlag(x, true);
                }
                g_bRestocking = true;
                g_dElapsedWorkTime = 0.0;
            }
        }
    }
}

void loadValues(int x, std::string line)
{
    Powerup* PowerupsPtr = p.getPowerups();
    switch (x) {
    case 0:
        //Get saved days
        day = getNumberStr(line);
        break;
    case 1:
        //get saved savings (money)
        p.setSavings(getNumberStr(line));
        break;
    case 2: {
        //get saved son1 state 
        bool son1state = getNumberStr(line);
        if (!son1state) { cPtr[0]->Recovers(); }
        else { cPtr[0]->isSick(); }
        break;
    }
    case 3: {
        //get saved son2 state
        bool son2state = getNumberStr(line);
        if (!son2state) { cPtr[1]->Recovers(); }
        else { cPtr[1]->isSick(); }
        break;
    }
    case 4:
        //get cheaperfood lvl
        PowerupsPtr->setFoodlvl(getNumberStr(line));
        break;
    case 5:
        //get cheaperrent lvl
        PowerupsPtr->setRentlvl(getNumberStr(line));
        break;
    case 6:
        //get playershoes lvl
        PowerupsPtr->setShoeslvl(getNumberStr(line));
        break;
    case 7:
        //get slowercustomers lvl
        PowerupsPtr->setSCustomerslvl(getNumberStr(line));
        break;
    case 8:
        //get rich customers lvl
        PowerupsPtr->setRCustomerslvl(getNumberStr(line));
        break;
    case 9:
        //get thrifty customers lvl
        PowerupsPtr->setTCustomerslvl(getNumberStr(line));
        break;
    }
}

int getNumberStr(std::string line)
{
    int y = 0;
    for (; y < line.length(); y++) {
        if (isdigit(line[y]))
            break;
    }
    line = line.substr(y, line.length() - y);
    int number = atoi(line.c_str());
    return number;
}

// Process inputs on End of work screen
void processInputEndOfWorkScreen()
{
    static bool alreadyPlayed = false;
    if (!alreadyPlayed) {
        PlaySound(L"BGM.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        alreadyPlayed = true;
    }
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
            alreadyPlayed = false;
        }
    }
}

// Process inputs on Game over screen; reset variables for new session
void processInputGameOver()
{
    if (g_skKeyEvent[K_ESCAPE].keyReleased) {// opens main menu if player hits the escape key
        for (int i = 0; i < 2; i++) {
            cPtr[i]->resetNODSick();
            cPtr[i]->resetNODUnfed();
            cPtr[i]->Recovers();
            cPtr[i]->resetHospState();
        }
        p.resetUnsatisfiedCustomers(); //reset unsatifiedCustomers to 0
        p.getPowerups()->resetall();
        for (int x = 0; x < 14; x++) {
            tutorial.setTutorialFlag(x, false);
        }
        g_dElapsedWorkTime = 0;
        g_eGameState = S_MENU;
    }
    day = 0; level = 1;
    p.setPos(18, 1);
    g_ePreviousGameState = S_GAMEOVER;
}

// Process inputs on Home screen
void processInputHome() //note lol
{
    framesPassed++;
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED
        && framesPassed % 5 == 0)
    {
        COORD c = g_Console.getConsoleSize();
        Player* pPtr = &p;
        if ((g_mouseEvent.mousePosition.X >= 27
            && g_mouseEvent.mousePosition.X <= 30)
            && g_mouseEvent.mousePosition.Y == 22) //Change to main menu state once mouse clicks on the button
        {
            g_eGameState = S_MENU;
        }
        else if ((g_mouseEvent.mousePosition.X >= 27
            && g_mouseEvent.mousePosition.X <= 34)
            && g_mouseEvent.mousePosition.Y == 20 && tutorial.getTutorialFlag(12) == true) //Change to main game state once mouse clicks on the button
        {
            day++;
            p.resetUnsatisfiedCustomers(); //reset unsatifiedCustomers to 0
            deleteCustomer();
            deleteBoxes();
            PlaySound(L"IntenseMusic(30).wav", NULL, SND_ASYNC);
            g_eGameState = S_GAME;
            updateSons();
        }
        else if ((g_mouseEvent.mousePosition.X >= 27
            && g_mouseEvent.mousePosition.X <= 31)
            && g_mouseEvent.mousePosition.Y == 21) //Change to store state once mouse clicks on the button
        {
            g_eGameState = S_STORE;
        }

        // Expenses toggling
        if ((g_mouseEvent.mousePosition.X == 39)
            && g_mouseEvent.mousePosition.Y == 7) //Toggle recognition of son 1 being fed
        {
            if (p.getSavings() >= p.getFood()) {
                p.payFood(cPtr[0]);
            }
        }
        else if ((g_mouseEvent.mousePosition.X == 39)
            && g_mouseEvent.mousePosition.Y == 12) //Toggle recognition of son 2 being fed
        {
            if (p.getSavings() >= p.getFood()) {
                p.payFood(cPtr[1]);
            }
        }
        else if ((g_mouseEvent.mousePosition.X == 44)
            && g_mouseEvent.mousePosition.Y == 6
            && cPtr[0]->getStatus() == true) //Toggle recognition of son 1 being treated
        {
            if (p.getSavings() >= p.getMedicine()) {
                p.payMedicine(cPtr[0]);
            }
        }
        else if ((g_mouseEvent.mousePosition.X == 44)
            && g_mouseEvent.mousePosition.Y == 11
            && cPtr[1]->getStatus() == true) //Toggle recognition of son 2 being treated
        {
            if (p.getSavings() >= p.getMedicine()) {
                p.payMedicine(cPtr[1]);
            }
        }
        else if ((g_mouseEvent.mousePosition.X == 45)
            && g_mouseEvent.mousePosition.Y == 14
            && day % 6 == 0 && day != 0) {
            if (p.getSavings() >= p.getRent()) {
                p.payRent();
            }
        }
    }
}

// Switch case for input processing for different game states; also includes debug processing
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
        break;
    case S_HOME: processInputHome();
        if (g_skKeyEvent[K_ESCAPE].keyReleased)// opens main menu if player hits the escape key
            g_eGameState = S_MENU; 
        break;
    case S_STORE: processStoreinput();
        break;
    case S_TUT:
        if (g_skKeyEvent[K_ESCAPE].keyReleased)// opens main menu if player hits the escape key
            g_eGameState = S_MENU;
        checkEnd();
        break;
    case S_GAME:
        if (g_skKeyEvent[K_ESCAPE].keyReleased) {// opens main menu if player hits the escape key
            g_eGameState = S_MENU; PlaySound(L"BGM.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        }
        checkEnd();
        break;
    case S_CREDITS: {
        processInputCredits();
    }
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
    static bool hasPlayed = false;
    if (!hasPlayed) {
        PlaySound(L"BGM.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
        hasPlayed = true;
    }

    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: renderSplashScreen(); // render splashscreen ui
        break;
    case S_MENU: renderMainMenu(); // render menu ui
        break;
    case S_ENDOFWORKSCREEN: renderEndOfWorkScreen(); // render endofwork screen ui
        break;
    case S_GAMEOVER: renderGameOver(); // render game over screen ui
        break;
    case S_HOME: renderHome(); // render home screen ui
        break;
    case S_STORE: renderStore(); // render store screen ui
        break;
    case S_TUT: renderTutorialLevel(); // render tutorial level ui
        break;
    case S_GAME: renderGame(); // render game screen ui
        break;
    case S_CREDITS: renderCredits();
        break;
    }

    switch (g_eDebugState)
    {
    case D_OFF: break;
    case D_BOTH: renderFramerate(); renderInputEvents(); break; // render framerate and input events; also includes other debug info
    }

    
    renderToScreen();       // dump the contents of the buffer to the screen, one frame worth of game
}

// Clear the screen
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

// Render game levels and entities
void renderGame()
{
    if (day < 5) {
        level = day + 1;
    }
    else { 
        level = 5;
    }
    map.chooseMap(level, g_Console);       // renders the map to the buffer first
    checkCustomerPlayerCollision();

    if (tutorial.getTutorialFlag(14) == false) {
        tutorial.tutorial(g_Console, g_sChar, g_mouseEvent, g_skKeyEvent, g_dElapsedWorkTime, p, BoxColour, tutorialTimer, g_eGameState);
    }

    renderCharacter();
    renderCustomer();  // renders the character into the buffer
    renderBoxes();
    renderShelfAmount();
    renderHUD();
}

// Render amount of items on the shelf
void renderShelfAmount()
{
    if (g_eGameState == S_GAME) {
        for (int i = 0; i < level + 1; i++) {
            int amt = sPtr[i]->getAmount();
            amt = sPtr[i]->getAmount();
            renderItem(i);
            renderCustomerTimer(i);
        }
    }
    else if (g_eGameState == S_TUT) {
        for (int i = 0; i < level + 1; i++) {
            int amt = sPtr[i]->getAmount();
            amt = sPtr[i]->getAmount();
            renderItem(i);
            renderCustomerTimer(i);
        }
    }
}

// Render HUD info
void renderHUD()
{
    framesPassed++; // counts frames
    COORD c;
    std::ostringstream ss;
    
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
    ss << "Unsatisfied Customers: " << p.getUnsatisfiedCustomers();
    c.X = 1;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x80);
    renderIteminBox();
}

void renderCustomerTimer(int shelf) //works ?
{
    const WORD colors[] = {
        0x56, 0x16, 0xB6, 0xE6, 0xA6, 0x96, 0x66
    };

    COORD c;
    std::ostringstream ss;
    c.X;
    c.Y;
    int shelfX;
    int shelfY;
    int cTimerArrival;
    for (int i = 0; i < customerMultiplier; i++)
    {
        if (customerPtr[i] != nullptr && 
            customerPtr[i]->getEndPointX() == customerPtr[i]->getX() && customerPtr[i]->getEndPointY() == customerPtr[i]->getY())
        {
            shelfY = ((customerPtr[i]->getEndPointY() - 1 ) / 6) -1;
            c.Y = ((shelfY + 1) * 6) - 1; 

            shelfX = ((customerPtr[i]->getEndPointX() - 16) / 21) -1;
            c.X = ((shelfX) * 21) + 27;

            if (shelfX == 0) { shelf = shelfY; }
            else {shelf = shelfY + 3;}

            cTimerArrival = ((20 - (customerPtr[i]->getTimer()) + i)/2)+2; //change this formula if u change anything about customer timer
            
            for (int i = 0; i < cTimerArrival; i++) {
                g_Console.writeToBuffer(c, char(220), colors[shelf]);
                c.X++;
            }
        }
    }
}


// Render items for renderShelfAmount
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
    for (int i = 0; i < amt; i++) {
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

//Render the map in its latest state
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

// Render Main menu UI
void renderMainMenu()
{
    map.chooseMap(0, g_Console);
    //renderMenuAnimation();
    COORD c = g_Console.getConsoleSize();
    c.Y /= 25;
    c.X = c.X / 2 - 5;
    g_Console.writeToBuffer(c, "Main Menu", 0xF0);
    c.Y += 8;
    c.X = g_Console.getConsoleSize().X / 6 + 20;
    if (g_ePreviousGameState == S_SPLASHSCREEN || g_ePreviousGameState == S_GAMEOVER)
        if (g_mouseEvent.mousePosition.X >= 33 && g_mouseEvent.mousePosition.X <= 41
            && g_mouseEvent.mousePosition.Y == 9) {
            g_Console.writeToBuffer(c, "Start New", 0xE0);
        }
        else {
            g_Console.writeToBuffer(c, "Start New", 0xF0);
        }
    else if (g_ePreviousGameState == S_HOME)
        if (g_mouseEvent.mousePosition.X >= 33 && g_mouseEvent.mousePosition.X <= 41
            && g_mouseEvent.mousePosition.Y == 9) {
            g_Console.writeToBuffer(c, "Back Home", 0xE0);
        }
        else {
            g_Console.writeToBuffer(c, "Back Home", 0xF0);
        }
    else
        if (g_mouseEvent.mousePosition.X >= 33 && g_mouseEvent.mousePosition.X <= 43
            && g_mouseEvent.mousePosition.Y == 9) {
            g_Console.writeToBuffer(c, "Resume Work", 0xE0);
        }
        else {
            g_Console.writeToBuffer(c, "Resume Work", 0xF0);
        }
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 6 + 20;
    if (g_mouseEvent.mousePosition.X >= 33 && g_mouseEvent.mousePosition.X <= 36
        && g_mouseEvent.mousePosition.Y == 10 && g_ePreviousGameState == S_HOME) {
        g_Console.writeToBuffer(c, "Save", 0xE0);
    }
    else if (g_ePreviousGameState == S_HOME) {
        g_Console.writeToBuffer(c, "Save", 0xF0);
    }
    else {
        g_Console.writeToBuffer(c, "Save", 0xF8);
    }
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 6 + 20;
    if (g_mouseEvent.mousePosition.X >= 33 && g_mouseEvent.mousePosition.X <= 36
        && g_mouseEvent.mousePosition.Y == 11 
        && (g_ePreviousGameState == S_HOME || g_ePreviousGameState == S_TUT || g_ePreviousGameState == S_GAME)) {
        g_Console.writeToBuffer(c, "Load", 0xE0);
    }
    else if (g_ePreviousGameState == S_HOME || g_ePreviousGameState == S_TUT || g_ePreviousGameState == S_GAME) {
        g_Console.writeToBuffer(c, "Load", 0xF0);
    }
    else {
        g_Console.writeToBuffer(c, "Load", 0xF8);
    }
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 6 + 20;
    if (g_mouseEvent.mousePosition.X >= 33 && g_mouseEvent.mousePosition.X <= 41
        && g_mouseEvent.mousePosition.Y == 12) {
        g_Console.writeToBuffer(c, "Exit Game", 0xE0);
    }
    else {
        g_Console.writeToBuffer(c, "Exit Game", 0xF0);
    }

    if (saveSuccessful) {
        c.X = 31;
        c.Y = 17;
        static int localFramesPassed = framesPassed;
        g_Console.writeToBuffer(c, "Save Successful!", 0xFA);
        if (framesPassed == localFramesPassed + 120) {
            saveSuccessful = false;
        }
    }
    else if (loadSuccessful) {
        c.X = 31;
        c.Y = 17;
        static int localFramesPassed = framesPassed;
        g_Console.writeToBuffer(c, "Load Successful!", 0xF2);
        if (framesPassed == localFramesPassed + 120) {
            loadSuccessful = false;
        }
    }
}

// Render home menu
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
    c.Y = 18;
    c.X = 27;
    g_Console.writeToBuffer(c, "Options:", 0xF0);
    c.Y += 2;
    if (g_mouseEvent.mousePosition.X >= 27 && g_mouseEvent.mousePosition.X <= 34
        && g_mouseEvent.mousePosition.Y == 20) {
        g_Console.writeToBuffer(c, "Next Day", 0xE0);
    }
    else {
        g_Console.writeToBuffer(c, "Next Day", 0xF0);
    }
    c.Y += 1;
    if (g_mouseEvent.mousePosition.X >= 27 && g_mouseEvent.mousePosition.X <= 31
        && g_mouseEvent.mousePosition.Y == 21) {
        g_Console.writeToBuffer(c, "Store", 0xE0);
    }
    else {
        g_Console.writeToBuffer(c, "Store", 0xF0);
    }
    c.Y += 1;
    if (g_mouseEvent.mousePosition.X >= 27 && g_mouseEvent.mousePosition.X <= 30
        && g_mouseEvent.mousePosition.Y == 22) {
        g_Console.writeToBuffer(c, "Menu", 0xE0);
    }
    else {
        g_Console.writeToBuffer(c, "Menu", 0xF0);
    }
    if (tutorial.getComplete() == false)
    {
        tutorial.tutorial(g_Console, g_sChar, g_mouseEvent, g_skKeyEvent, g_dElapsedWorkTime, p, BoxColour, tutorialTimer, g_eGameState);
    }
}

// Render animation for menu screens
void renderMenuAnimation() 
{
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

// Render expenses options for home menu
void renderHomeExpenses(COORD c)
{
    std::ostringstream ss;
    ss.str("");
   
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
        g_Console.writeToBuffer(c, "(Q//A//Q )", 0xFC);
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
    ss << "Food ($" << p.getFood() << ") [ ]";
    g_Console.writeToBuffer(c, ss.str(), 0xF0);
    if (cPtr[0]->getStatusFed() == true) {
        c.X += 12;
        g_Console.writeToBuffer(c, " ", 0x00);
        c.X -= 12;
    }
    ss.str("");
    c.Y += 2;
    g_Console.writeToBuffer(c, "!=========Son 2=========!", 0xF0);
    c.Y += 1;
    g_Console.writeToBuffer(c, "State : ", 0xF0);
    c.X += 8;
    if (cPtr[1]->getStatus() == true) {
        g_Console.writeToBuffer(c, "(T//-//T )", 0xFC);
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
    ss << "Food ($" << p.getFood() << ") [ ]"; 
    g_Console.writeToBuffer(c, ss.str(), 0xF0);
    ss.str("");
    if (cPtr[1]->getStatusFed() == true) {
        c.X += 12;
        g_Console.writeToBuffer(c, " ", 0x00);
        c.X -= 12;
    }
    c.Y += 2;
    if (day % 6 == 0 && day != 0) {
        ss << "!====Rent ($" << p.getRent() << ") [ ]====!";
        g_Console.writeToBuffer(c, ss.str(), 0xF0);
        ss.str("");
        if (p.getRentStatus() == true) {
            c.X += 18;
            g_Console.writeToBuffer(c, " ", 0x00);
            c.X -= 18;
        }
    }
}

// Render end of work screen information
void renderEndOfWorkScreen()
{
    map.chooseMap(7, g_Console);
    COORD c = g_Console.getConsoleSize();
    std::ostringstream ss;
    
    c.Y /= 25;
    c.X = c.X / 2 - 10;
    g_Console.writeToBuffer(c, "End of day report", 0xF0);
   
    c.Y += 9;
    c.X = g_Console.getConsoleSize().X / 6 + 15;
    ss.str("");
    ss << "Complaints given: " << p.getUnsatisfiedCustomers();
    g_Console.writeToBuffer(c, ss.str(), 0xF0);
    
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 6 + 15;
    ss.str("");
    ss << "Today's pay: $" << p.getTotalEarned();
    g_Console.writeToBuffer(c, ss.str(), 0xF0);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 6 + 15;
    if (g_mouseEvent.mousePosition.X >= 28 && g_mouseEvent.mousePosition.X <= 48
        && g_mouseEvent.mousePosition.Y == 12) {
        g_Console.writeToBuffer(c, "Click here to go home", 0xE0);
    }
    else {
        g_Console.writeToBuffer(c, "Click here to go home", 0xF0);
    }
    if (tutorial.getComplete() == false)
    {
        tutorial.tutorial(g_Console, g_sChar, g_mouseEvent, g_skKeyEvent, g_dElapsedWorkTime, p, BoxColour, tutorialTimer, g_eGameState);
    }

}

// Render cause of game over and game over screen
void renderGameOver()
{
    COORD c = g_Console.getConsoleSize();
    map.chooseMap(8, g_Console);
    c.Y /= 25;
    c.X = g_Console.getConsoleSize().X / 3 + 8;
    g_Console.writeToBuffer(c, "Game Over!", 0xF0);
    c.Y += 6;
    c.X = g_Console.getConsoleSize().X / 3 + 2;
    if (cPtr[1]->getHospState() == true || cPtr[0]->getHospState() == true)
    {
        g_Console.writeToBuffer(c, "     One of your", 0xF0);
        c.Y++;
        g_Console.writeToBuffer(c, "sons was hospitalised!", 0xF0); //stop double print if both sons sick
    }
    else if (g_eGameState == S_GAMEOVER && p.getUnsatisfiedCustomers() >= 10)
    {
        g_Console.writeToBuffer(c, "You got too", 0xF0);
        c.Y++;
        g_Console.writeToBuffer(c, "many complaints (10)!", 0xF0);
    }
    else if (g_eGameState == S_GAMEOVER && p.getRentStatus() == false) {
        g_Console.writeToBuffer(c, "Your landlord was not", 0xF0);
        c.Y += 1;
        g_Console.writeToBuffer(c, "as kind as you thought.", 0xF0);
        c.Y += 1;
        g_Console.writeToBuffer(c, "You were evicted", 0xF0);
        c.Y += 1;
        g_Console.writeToBuffer(c, "for not paying rent!", 0xF0);
        c.Y -= 3;
    }
    c.Y = 20;
    c.X = g_Console.getConsoleSize().X / 3 + 2;
    g_Console.writeToBuffer(c, " Press [ESC] to head", 0xF0);
    c.Y++;
    g_Console.writeToBuffer(c, "back to the main menu!", 0xF0);
}

// Render tutorial level ui
void renderTutorialLevel()
{
    map.chooseMap(1, g_Console);
    checkCustomerPlayerCollision();
    renderCharacter();  // renders the character into the buffer
    renderShelfAmount();
    renderHUD();

    if (tutorial.getTutorialFlag(6) == true) 
        renderCustomer();

    renderBoxes();
    tutorial.tutorial(g_Console, g_sChar, g_mouseEvent, g_skKeyEvent, g_dElapsedWorkTime, p, BoxColour, tutorialTimer, g_eGameState);
}

// Render store perks available for purchase; one unlocked per day
void renderStore()
{
    map.chooseMap(9, g_Console);
    COORD c = g_Console.getConsoleSize();
    c.X /= 2;
    c.X -= 6;
    c.Y = 2;
    std::ostringstream ss;
    ss.str("");
    g_Console.writeToBuffer(c, "Store", 0xF0);
    c.X = 80 / 4 + 7;
    c.Y += 2;
    ss << "Savings : $" << p.getSavings();
    g_Console.writeToBuffer(c, ss.str(), 0xF0);
    ss.str("");
    if (day >= 4) {
        c.Y += 2;
        g_Console.writeToBuffer(c, "[$100]", 0xF0);
        ss.str("");
        c.Y += 2;
        if (g_mouseEvent.mousePosition.X >= 27 && g_mouseEvent.mousePosition.X <= 39
            && g_mouseEvent.mousePosition.Y == 8) {
            ss << "Crowd Control (Lvl " << p.getPowerups()->getSCustomerslvl() << ")";
            g_Console.writeToBuffer(c, ss.str(), 0xE0);
            ss.str("");
        }
        else {
            ss << "Crowd Control (Lvl " << p.getPowerups()->getSCustomerslvl() << ")";
            g_Console.writeToBuffer(c, ss.str(), 0xF0);
            ss.str("");
        }
        if (day >= 5) {
            c.Y += 2;
            if (g_mouseEvent.mousePosition.X >= 27 && g_mouseEvent.mousePosition.X <= 38
                && g_mouseEvent.mousePosition.Y == 10) {
                ss << "Cooler Shoes (Lvl " << p.getPowerups()->getShoeslvl() << ")";
                g_Console.writeToBuffer(c, ss.str(), 0xE0);
                ss.str("");
            }
            else {
                ss << "Cooler Shoes (Lvl " << p.getPowerups()->getShoeslvl() << ")";
                g_Console.writeToBuffer(c, ss.str(), 0xF0);
                ss.str("");
            }
        }
    }
    if (day < 4) {
        c.Y += 6;
    }
    else if (day < 5) {
        c.Y += 2;
    }
    c.Y += 2;
    g_Console.writeToBuffer(c, "[$50]", 0xF0);
    c.Y += 2;
    if (g_mouseEvent.mousePosition.X >= 27 && g_mouseEvent.mousePosition.X <= 38
        && g_mouseEvent.mousePosition.Y == 14) {
        ss << "Cheaper Food (Lvl " << p.getPowerups()->getFoodlvl() << ")";
        g_Console.writeToBuffer(c, ss.str(), 0xE0);
        ss.str("");
    }
    else {
        ss << "Cheaper Food (Lvl " << p.getPowerups()->getFoodlvl() << ")";
        g_Console.writeToBuffer(c, ss.str(), 0xF0);
        ss.str("");
    }
    c.Y += 2;
    if (day >= 1) {
        if (g_mouseEvent.mousePosition.X >= 27 && g_mouseEvent.mousePosition.X <= 38
            && g_mouseEvent.mousePosition.Y == 16) {
            ss << "Cheaper Rent (Lvl " << p.getPowerups()->getRentlvl() << ")";
            g_Console.writeToBuffer(c, ss.str(), 0xE0);
            ss.str("");
        }
        else {
            ss << "Cheaper Rent (Lvl " << p.getPowerups()->getRentlvl() << ")";
            g_Console.writeToBuffer(c, ss.str(), 0xF0);
            ss.str("");
        }
    }
    c.Y += 2;
    if (day >= 2) {
        if (g_mouseEvent.mousePosition.X >= 27 && g_mouseEvent.mousePosition.X <= 40
            && g_mouseEvent.mousePosition.Y == 18) {
            ss << "Rich Customers (Lvl " << p.getPowerups()->getRCustomerslvl() << ")";
            g_Console.writeToBuffer(c, ss.str(), 0xE0);
            ss.str("");
        }
        else {
            ss << "Rich Customers (Lvl " << p.getPowerups()->getRCustomerslvl() << ")";
            g_Console.writeToBuffer(c, ss.str(), 0xF0);
            ss.str("");
        }
    }
    c.Y += 2;
    if (day >= 3) {
        if (g_mouseEvent.mousePosition.X >= 27 && g_mouseEvent.mousePosition.X <= 43
            && g_mouseEvent.mousePosition.Y == 20) {
            ss << "Thrifty Customers (Lvl " << p.getPowerups()->getTCustomerslvl() << ")";
            g_Console.writeToBuffer(c, ss.str(), 0xE0);
            ss.str("");
        }
        else {
            ss << "Thrifty Customers (Lvl " << p.getPowerups()->getTCustomerslvl() << ")";
            g_Console.writeToBuffer(c, ss.str(), 0xF0);
            ss.str("");
        }
    }
    c = g_Console.getConsoleSize();
    c.X /= 2;
    c.X -= 5;
    c.Y = 22;
    if (g_mouseEvent.mousePosition.X >= 35 && g_mouseEvent.mousePosition.X <= 38
        && g_mouseEvent.mousePosition.Y == 22) {
        g_Console.writeToBuffer(c, "Home", 0xE0);
    }
    else {
        g_Console.writeToBuffer(c, "Home", 0xF0);
    }

    if (tutorial.getComplete() == false)
    {
        tutorial.tutorial(g_Console, g_sChar, g_mouseEvent, g_skKeyEvent, g_dElapsedWorkTime, p, BoxColour, tutorialTimer, g_eGameState);
    }
}

void renderIteminBox()
{
    const WORD colors[] = {
        0x0D, 0x01, 0x0B, 0x0E, 0x0A, 0x09
    };
    COORD c{ 1, 21 };
    int itemtag = boxPtr[0]->getTag();
    Map itemrender;
    switch (itemtag) {
    case 0: {
        break;
    }
    case 1: {
        std::fstream rice("Rice.txt");
        itemrender.printItemtxt(rice, g_Console, colors[3]);
        g_Console.writeToBuffer(c, "Rice", colors[3]);
        break;
    }
    case 2: {
        std::fstream toiletpaper("ToiletPaper.txt");
        itemrender.printItemtxt(toiletpaper, g_Console, colors[0]);
        g_Console.writeToBuffer(c, "Toilet Paper", colors[0]);
        break;
    }
    case 3: {
        std::fstream cannedfood("CannedFood.txt");
        itemrender.printItemtxt(cannedfood, g_Console, colors[2]);
        g_Console.writeToBuffer(c, "Canned Food", colors[2]);
        break;
    }
    case 4: {
        std::fstream instant("InstantNoodles.txt");
        itemrender.printItemtxt(instant, g_Console, colors[1]);
        g_Console.writeToBuffer(c, "Instant Noodles", colors[1]);
        break;
    }
    case 5: {
        std::fstream veg("Vegetable.txt");
        itemrender.printItemtxt(veg, g_Console, colors[4]);
        g_Console.writeToBuffer(c, "Vegetables", colors[4]);
        break;
    }
    case 6: {
        std::fstream band("Bandages.txt");
        itemrender.printItemtxt(band, g_Console, colors[5]);
        g_Console.writeToBuffer(c, "Bandages", colors[5]);
        break;
    }
    }
}

void renderCredits()
{
    COORD c{34, 1};
    g_Console.writeToBuffer(c, "Credits", 0x0F);
    c.Y += 5;
    c.X -= 20;
    g_Console.writeToBuffer(c, "All dialogue speech sounds are credited to discord bot", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "Text To Speech #1736", 0x0F);
    c.Y += 2;
    g_Console.writeToBuffer(c, "All background music is generally credited to", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "FStudios royalty-free music", 0x0F);
    c.Y += 1;
    g_Console.writeToBuffer(c, "www.fesliyanstudios.com", 0x0F);
    c.Y += 5;
    c.X += 10;
    g_Console.writeToBuffer(c, "Left click to head to main menu", 0x0F);
}

// Render boxes
void renderBoxes()
{
    g_Console.writeToBuffer(boxPosPtr[0]->getX(), boxPosPtr[0]->getY(), ' ', BoxColour);
    for (int i = 1; i < 11; i++)
    {
        if (boxPtr[i] != nullptr)
        {
            g_Console.writeToBuffer(boxPosPtr[i]->getX(), boxPosPtr[i]->getY(), ' ', CustomerBoxColour[i-1]);
        }
    }
   
}
void checkWaveGone(int number)
{
    for (int i = 0; i < number; i++)
    {
        if (customerPtr[i] == nullptr) {
            total++;
        }
    }
}

bool switchWaveGone(int customerMultiplier)
{
    total = 0;
    bool returnval;
    switch (day) {
    case 0:
        checkWaveGone(6);
        if (total == 6)
        {
            returnval = true;
            return returnval;
        }
        else if (total < 6)
        {
            returnval = false;
            return returnval;
        }
        break;
    case 1:
        checkWaveGone(7);
        if (total == 7)
        {
            returnval = true;
            return returnval;
        }
        else if (total < 7)
        {
            returnval = false;
            return returnval;
        }
        break;
    case 2:
        checkWaveGone(8);
        if (total == 8)
        {
            returnval = true;
            return returnval;
        }
        else if (total < 8)
        {
            returnval = false;
            return returnval;
        }
        break;
    case 3:
        checkWaveGone(9);
        if (total == 9)
        {
            returnval = true;
            return returnval;
        }
        else if (total < 9)
        {
            returnval = false;
            return returnval;
        }
        break;
    default:
        checkWaveGone(10);
        if (total == 10)
        {
            returnval = true;
            return returnval;
        }
        else if (total < 10)
        {
            returnval = false;
            return returnval;
        }
        break;
    }
}



// Render customer current position yeye
void renderCustomer()
{   
    static bool alreadyPlayed = false;
    static bool alreadyPlayed2 = false;
    if ((!alreadyPlayed2 && g_bRestocking) || (g_bRestocking && g_ePreviousGameState == S_MENU)) {
        PlaySound(L"IntenseMusic(30).wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        alreadyPlayed2 = true;
        alreadyPlayed = false;
    }
    if (g_bRestocking == false) {
        COORD c = g_Console.getConsoleSize();
        bool created = false;
        if (!alreadyPlayed) {
            PlaySound(L"BGM1.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            alreadyPlayed = true;
            alreadyPlayed2 = false;
        }
        bool waveGone;
        //Check if current wave of customers is gone
        waveGone = switchWaveGone(customerMultiplier);
        //Scalable difficulty formula

        if (increased == false)
        {
            customerMultiplier += day;
            increased = true;
        }

        if (customerMultiplier >= 10) {
            customerMultiplier = 10;
        }

        for (int i = 0; i < customerMultiplier; i++) 
        {
            if (customerPtr[i] != nullptr)
            {
                
                if (customerPtr[i]->getTravelling() == true)
                {
                    customerDirection[i] = customerPtr[i]->moveCustomer(map, framesPassed, 4 + (p.getPowerups()->getSCustomerslvl()));
                }

                if (customerPtr[i]->getQuantity() == 0 && customerPtr[i]->getMovingBack() != true) {
                    customerPtr[i]->setEndPoint(79, 17);
                    customerPtr[i]->setAvoiding(5);
                    customerPtr[i]->setTravelling(false);
                    customerPtr[i]->setMovingBack(true);
                }

                customerPtr[i]->customerCollision(map);   

                if (customerPtr[i]->getPos().getX() == 73)
                {
                    customerPtr[i]->setYLock(false);
                }

                
                WORD customerColour;
                customerColour = 0x20;//green satisfied
                if (customerPtr[i]->getState() == false)
                {
                    customerColour = 0x44; //red unsatisfied
                }
               
                customerPtr[i]->printOutCustomer(g_Console, map, customerPtr[i]->getQuantity(), customerColour);
               

                if (customerPtr[i]->getPos().getX() == customerPtr[i]->getEndPoint().getX() && customerPtr[i]->getPos().getY() == customerPtr[i]->getEndPoint().getY() && customerPtr[i]->getTimerSet() == false)
                {
                    customerPtr[i]->setTimer(5.10 + i);
                    customerPtr[i]->setTimerSet(true);
                    customerPtr[i]->setTravelling(false);
                }
                
                if (boxPtr[i + 1] == nullptr) {
                    boxPtr[i + 1] = new Box;
                    boxPosPtr[i + 1] = new Position;
                    CustomerBoxColour[i] = 0x77;
                }
                
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

                if (customerPtr[i]->getTimer() >= 4.95 + i && customerPtr[i]->getTimer() <= 5.05 + i)
                {
                    customerPtr[i]->moveToShelfContainingItem(customerPtr[i]->getItemToBuy());
                    customerPtr[i]->setTravelling(true);
                    customerPtr[i]->setTimer(-1);
                }

                if ((customerPtr[i]->getTimer() >= 19.95 + i) && (customerPtr[i]->getTimer() <= 20.05 + i))
                {
                    

                    

                    for (int j = 0; j < 3; j++) { //right hand shelves
                     
                        if (sPtr[j] != nullptr) {

                            if (customerPtr[i]->getX() == 37 && customerPtr[i]->getY() == 7 + 6 * j) {
   

                                if (sPtr[j]->getAmount() >= customerPtr[i]->getQuantity()) //satisfied
                                {
                                    sPtr[j]->decreaseItem(1);

                                    p.AddDayEarnings(1); //for adding amount earned daily

                                    customerPtr[i]->setQuantity(customerPtr[i]->getQuantity() - 1);

                                    switch (j)
                                    {
                                    case 0:
                                        CustomerBoxColour[i] = 0x50;
                                        break;
                                    case 1:
                                        CustomerBoxColour[i] = 0x10;
                                        break;
                                    case 2:
                                        CustomerBoxColour[i] = 0xB0;
                                        break;
                                    }
                                }

                                else if (CustomerBoxColour[i] = 0x77 && sPtr[j]->getAmount() < customerPtr[i]->getQuantity()) { //unsatisfied
                                    p.increaseUnsatisfiedCustomers();
                                    customerPtr[i]->setEndPoint(79, 17);
                                    customerPtr[i]->setAvoiding(5);
                                    customerPtr[i]->setMovingBack(true);
                                    customerPtr[i]->unSatisfied();
                                    CustomerBoxColour[i] = 0x77;
                                }

                            }

                        }
                    }

                    for (int j = 3; j < 6; j++) { //left hand shelves

                        if (sPtr[j] != nullptr) {

                            if (customerPtr[i]->getX() == 58 && customerPtr[i]->getY() == 7 + 6 * (j-3)) {
                                
                                
                                if (sPtr[j]->getAmount() >= customerPtr[i]->getQuantity()) //satisfied
                                {
                                    sPtr[j]->decreaseItem(1);

                                    p.AddDayEarnings(1); //for adding amount earned daily

                                    customerPtr[i]->setQuantity(customerPtr[i]->getQuantity() - 1);

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

                                else if (CustomerBoxColour[i] = 0x77 && sPtr[j]->getAmount() < customerPtr[i]->getQuantity()) { //unsatisfied
                                    p.increaseUnsatisfiedCustomers();
                                    customerPtr[i]->setEndPoint(79, 17);
                                    customerPtr[i]->setAvoiding(5);
                                    customerPtr[i]->setMovingBack(true);
                                    customerPtr[i]->unSatisfied();
                                    CustomerBoxColour[i] = 0x77;
                                }
                            }
                        }
                    }
                }

                else if (customerPtr[i]->getPos().getX() == 79 && customerPtr[i]->getPos().getY() == 17) //when customer exits store
                {
                    delete customerPtr[i];
                    customerPtr[i] = nullptr;
                    total--;

                    delete boxPtr[i + 1];
                    boxPtr[i + 1] = nullptr;
                    delete boxPosPtr[i + 1];
                    boxPosPtr[i + 1] = nullptr;

                    CustomerBoxColour[i] = 0x77;
                    map.setGrid(79, 17, '0');
                }
            }
            else
            {
                if (waveGone == true)
                {
                    customerPtr[i] = new Customer(p);
                    customerPtr[i]->setItemToBuy(day + 2);
                    customerPtr[i]->setTimer(0);
                    customerPtr[i]->setPos(customerPtr[i]->getPos().getX(), customerPtr[i]->getPos().getY() + i);
                    customerPtr[i]->setSpawned(true);
                }
            }
        }
    }
}

// Render player current position
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

// DEBUG: render framerate and other information needed
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

    ss.str("");
    ss << "Player Coords";
    c.X = 45; //change to shift location of timer
    c.Y = 20;  //we might use this or we might need to make a new timer to show when the game starts
    g_Console.writeToBuffer(c, ss.str(), 0x0F);
    c.Y += 1;
    ss.str("");
    ss << "Player X: " << p.getPos().getX();
    g_Console.writeToBuffer(c, ss.str(), 0x0F);
    c.Y += 1;
    ss.str("");
    ss << "Player Y: " << p.getPos().getY();
    g_Console.writeToBuffer(c, ss.str(), 0x0F);
    
    ss.str("");
    ss << spawnTimer;
    c.X = 0;
    c.Y = 23;
    g_Console.writeToBuffer(c, ss.str(), 0x0F);


    for (int i = 0; i < 10; i++)
    {
        if (customerPtr[i] != nullptr)
        {
            ss.str("");
            double display = customerPtr[i]->getTimer();
            ss << display;
            c.X = 0;
            c.Y = 14 + i;
            g_Console.writeToBuffer(c, ss.str(), 0x0F);

            ss.str("");
            ss << customerPtr[i] << " pos:" << customerPtr[i]->getPos().getX() << ", " << customerPtr[i]->getPos().getY() << ", Shelf " << customerPtr[i]->getItemToBuy() << ", endpos" << customerPtr[i]->getEndPoint().getX() << ", " << customerPtr[i]->getEndPoint().getY() << ", " << map.getGrid(customerPtr[i]->getPrevPos().getY(), customerPtr[i]->getPrevPos().getX()) << ", " << customerPtr[i]->getColliding();
            c.X = 10;
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
