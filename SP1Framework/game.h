#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"

extern CStopWatch g_swTimer;
extern bool g_bQuitGame;

// struct to store keyboard events
// a small subset of KEY_EVENT_RECORD
struct SKeyEvent
{
    bool keyDown;
    bool keyReleased;
};

// struct to store mouse events
// a small subset of MOUSE_EVENT_RECORD
struct SMouseEvent
{
    COORD mousePosition;
    DWORD buttonState;
    DWORD eventFlags;
};

// Enumeration to store the control keys that your game will have
enum EKEYS
{
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_ESCAPE,
    K_SPACE, //to pick up boxes
    K_F3,
    K_F4,
    K_COUNT,
};

// Enumeration for the different screen states
enum EGAMESTATES
{
    S_SPLASHSCREEN,
    S_MENU,
    S_ENDOFWORKSCREEN,
    S_HOME,
    S_TUT,
    S_GAME,
    S_COUNT
};

// Enumeration for the different debug states
enum EDEBUGSTATES
{
    D_OFF,
    D_FRAMES,
    D_INPUT,
    D_BOTH
};

// Enumeration for different item types
enum ItemType
{
    I_RICE,
    I_TOILETPAPER,
    I_CANNED,
    I_INSTANTNOODLE,
    I_VEGGIE,
    I_BANDAGE,
    I_COUNT
};

struct DIRECTION
{
    bool UP;
    bool DOWN;
    bool LEFT;
    bool RIGHT;
};
// struct for the game character
struct SGameChar
{
    COORD m_cLocation;
    DIRECTION moving;
    bool  m_bActive;
};

void init        ( void );      // initialize your variables, allocate memory, etc
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory

void updateSplashScreen();    // waits for time to pass in splash screen
void updateMenu();            // Menu logic
void updateEndofWorkScreen(); // End of Work logic
void updateHome();            // Home logic
void updateTutorial();        // Tutorial logic
void updateGame();          // Game logic

void moveCharacter(); // moves the character, collision detection, physics, etc
void actuallyMoving();
void pickUpBoxes();

void restockShelf();
void updateCustomer();

void updateSons(); // update son status and run logic

void processUserInput();    // checks if you should change states or do something else with the game, e.g. pause, exit
void clearScreen();         // clears the current screen and draw from scratch 
void renderSplashScreen();  // renders the splash screen
void renderGame();          // renders the game stuff
void renderShelfAmount(); //renders the shelf stuff
void renderItem(int shelf); //renders amount of items on shelf
void renderMap();           // renders the map to the buffer first
void renderHUD();        // renders the HUD
void renderMainMenu(); //renders the main menu
void renderHome(); //renders home
void renderHomeExpenses(COORD c); // renders info about sons and stuff
void renderEndOfWorkScreen();
void renderTutorialLevel(); // renders tutorial level

void renderBoxes();
void renderCustomer();
void renderCharacter();     // renders the character into the buffer
void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game
void renderInputEvents();   // renders the status of input events

// keyboard and mouse input event managers
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent);  // define this function for the console to call when there are keyboard events
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);      // define this function for the console to call when there are mouse events

void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent);   // handles keyboard events for gameplay 
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent); // handles mouse events for gameplay 


#endif // _GAME_H