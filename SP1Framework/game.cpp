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

double  g_dElapsedTime;
double g_dElapsedWorkTime;
double  g_dDeltaTime;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;

// Game specific variables here
SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state
EGAMESTATES g_ePreviousGameState = S_SPLASHSCREEN; // initial state
EDEBUGSTATES g_eDebugState = D_OFF; // initial state

Customer* customerPtr[6] = {nullptr , nullptr , nullptr , nullptr , nullptr , nullptr};

Shelf* sPtr[2] = { nullptr, nullptr };

Player p;

Box* boxPtr;
Position* boxPosPtr;
WORD BoxColour;
Map map;
int framesPassed;

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

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;
    g_ePreviousGameState = S_SPLASHSCREEN;

   /* g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 2;*/

    g_sChar.m_cLocation.X = 18; //changed character spawn location
    g_sChar.m_cLocation.Y = 1;

    
    //init shelf
    for (int i = 0; i < 2; i++) {
        if (sPtr[i] == nullptr) {
            sPtr[i] = new Shelf;
        }
    }
    if (sPtr[0] != nullptr) {
        sPtr[0]->setShelf(0x50);
    }
    if (sPtr[1] != nullptr) {
        sPtr[1]->setShelf(0x10);
    }

    //init box and box pos
    if (boxPtr == nullptr) {
        boxPtr = new Box;
        boxPosPtr = new Position;
        boxPosPtr->setX(18);
        boxPosPtr->setY(2);
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
    case S_HOME: gameplayKBHandler(keyboardEvent); // handle home menu keyboard menu
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
    case S_HOME: gameplayMouseHandler(mouseEvent); // handle mouse input for home menu
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
        case S_HOME: updateHome();
            break;
        case S_TUT: updateTutorial();
            break;
        case S_GAME: g_dElapsedWorkTime += dt; updateGame();// gameplay logic when we are in the game
            break;
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
    //PLAYER / BOX COLLISION WITH ENVIRONMENT IS SOLVED HERE

    switch (g_sChar.moving.UP) 
    {
    case true:
        
        if (map.collision(g_sChar.m_cLocation.Y, g_sChar.m_cLocation.X, -1, 0, map) == '0')
        {
            g_sChar.m_cLocation.Y--;
        }
        boxPosPtr->setX(g_sChar.m_cLocation.X);
        boxPosPtr->setY(g_sChar.m_cLocation.Y - 1);
        if (map.collision(boxPosPtr->getY(), boxPosPtr->getX(), 0, 0, map) != '0')
        {
            g_sChar.m_cLocation.Y++;
            boxPosPtr->setY(g_sChar.m_cLocation.Y - 1);
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
        boxPosPtr->setX(g_sChar.m_cLocation.X);
        boxPosPtr->setY(g_sChar.m_cLocation.Y + 1);
        if (map.collision(boxPosPtr->getY(), boxPosPtr->getX(), 0, 0, map) != '0')
        {
            g_sChar.m_cLocation.Y--;
            boxPosPtr->setY(g_sChar.m_cLocation.Y + 1);
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
        boxPosPtr->setX(g_sChar.m_cLocation.X - 1);
        boxPosPtr->setY(g_sChar.m_cLocation.Y);
        if (map.collision(boxPosPtr->getY(), boxPosPtr->getX(), 0, 0, map) != '0')
        {
            g_sChar.m_cLocation.X++;
            boxPosPtr->setX(g_sChar.m_cLocation.X - 1);
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
        boxPosPtr->setX(g_sChar.m_cLocation.X + 1);
        boxPosPtr->setY(g_sChar.m_cLocation.Y);
        if (map.collision(boxPosPtr->getY(), boxPosPtr->getX(), 0, 0, map) != '0')
        {
            g_sChar.m_cLocation.X--;
            boxPosPtr->setX(g_sChar.m_cLocation.X + 1);
        }
        break;
    case false:
        break;
    }
}

void pickUpBoxes()  //todo
{
    if (p.isHoldingProduct() == false) {
        BoxColour = 0x70; //empty box grey
    }

    if (g_skKeyEvent[K_SPACE].keyReleased && g_sChar.m_cLocation.X == 3 && g_sChar.m_cLocation.Y == 3 && map.getGrid(3, 1) != 'A')
    {
        BoxColour = 0x50; //toilet paper purple
        p.holdsProduct();
    }
    if (g_skKeyEvent[K_SPACE].keyReleased && g_sChar.m_cLocation.X == 3 && g_sChar.m_cLocation.Y == 4 && map.getGrid(4, 1) != 'A')
    {
        BoxColour = 0x10; //instant noodle dark blue          
        p.holdsProduct();
    }
    if (g_skKeyEvent[K_SPACE].keyReleased && g_sChar.m_cLocation.X == 3 && g_sChar.m_cLocation.Y == 5 && map.getGrid(5, 1) != 'A')
    {
        BoxColour = 0xB0; //canned food teal

        p.holdsProduct();
    }
    if (g_skKeyEvent[K_SPACE].keyReleased && g_sChar.m_cLocation.X == 3 && g_sChar.m_cLocation.Y == 6 && map.getGrid(6, 1) != 'A')
    {
        BoxColour = 0xE0; //rice cream

        p.holdsProduct();
    }
    if (g_skKeyEvent[K_SPACE].keyReleased && g_sChar.m_cLocation.X == 3 && g_sChar.m_cLocation.Y == 7 && map.getGrid(7, 1) != 'A')
    {
        BoxColour = 0xA0; //vegetable green

        p.holdsProduct();
    }
    if (g_skKeyEvent[K_SPACE].keyReleased && g_sChar.m_cLocation.X == 3 && g_sChar.m_cLocation.Y == 8 && map.getGrid(8, 1) != 'A')
    {
        BoxColour = 0x90;//bandages blue

        p.holdsProduct();
    }
}

void restockShelf(){
    for (int i = 27; i < 37; i++) { //shelf 1 purple
        if (g_skKeyEvent[K_SPACE].keyReleased && BoxColour == sPtr[0]->returnShelfColour() && (g_sChar.m_cLocation.Y == 6 + 2 || g_sChar.m_cLocation.Y == 6 - 2) && g_sChar.m_cLocation.X == i) {
            sPtr[0]->increaseItem(5);
            p.releaseProduct();
        }

        if (g_skKeyEvent[K_SPACE].keyReleased && BoxColour == sPtr[1]->returnShelfColour() && (g_sChar.m_cLocation.Y == 12 + 2 || g_sChar.m_cLocation.Y == 12 - 2) && g_sChar.m_cLocation.X == i) {
            sPtr[1]->increaseItem(5);
            p.releaseProduct();
        }
    }
    
}

void checkEnd() //Check if day has ended and update variables
{
    //if (g_dElapsedWorkTime >= 5)
    if (g_skKeyEvent[K_F4].keyDown)
    {
        g_dElapsedWorkTime = 0.0;
        COORD c;
        c.X = 18;
        c.Y = 1;
        g_sChar.m_cLocation = c;
        boxPosPtr->setX(18);
        boxPosPtr->setY(2);
        g_eGameState = S_ENDOFWORKSCREEN;
    }
}

void processDebugState() //Toggle debug options
{
    if (g_skKeyEvent[6].keyDown)
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
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED && g_ePreviousGameState == S_SPLASHSCREEN)
    {
        COORD c = g_Console.getConsoleSize();
        if ((g_mouseEvent.mousePosition.X >= c.X / 2 - 7
            && g_mouseEvent.mousePosition.X <= c.X / 2 + 1)
            && g_mouseEvent.mousePosition.Y == 9) //Change to main game state once mouse clicks on the button
        {
            g_ePreviousGameState = g_eGameState;
            g_eGameState = S_GAME;
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
            && g_mouseEvent.mousePosition.Y == 13) //Change to home state once mouse clicks on the button
        {
            g_eGameState = S_HOME;
        }
    }
}

void processInputHome()
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
        
    }
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {
        COORD c = g_Console.getConsoleSize();
        if ((g_mouseEvent.mousePosition.X >= c.X - 20
            && g_mouseEvent.mousePosition.X <= c.X - 13)
            && g_mouseEvent.mousePosition.Y == c.Y / 5 + 3) //Change to main game state once mouse clicks on the button
        {
            g_eGameState = S_GAME;
        }
    }
}

void processUserInput()
{
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: processInputSplash(); break;
    case S_MENU: processInputMenu(); break;
    case S_ENDOFWORKSCREEN: processInputEndOfWorkScreen(); break;
    case S_HOME: processInputHome();
        if (g_skKeyEvent[K_ESCAPE].keyReleased)// opens main menu if player hits the escape key
            g_eGameState = S_MENU; 
        break;
    case S_TUT:
        if (g_skKeyEvent[K_ESCAPE].keyReleased)// opens main menu if player hits the escape key
            g_eGameState = S_MENU;
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
    case S_HOME: renderHome();
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
    renderTutorialLevel();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    renderCustomer();
    renderBoxes();
    renderShelfAmount();
    framesPassed++; // counts frames
    COORD c;
    std::ostringstream ss;
    ss.str("");     // displays the elapsed time
    ss << g_dElapsedWorkTime << "secs";
    c.X = 36; //change to shift location of timer
    c.Y = 0;  //we might use this or we might need to make a new timer to show when the game starts
    g_Console.writeToBuffer(c, ss.str(), 0x59); 
    ss.str(""); //probably can be implemented cleaner
    ss << framesPassed << "frames";
    c.X = 36; 
    c.Y = 24;  
    g_Console.writeToBuffer(c, ss.str(), 0x59);
}

void renderShelfAmount()
{
    for (int i = 0; i < 2; i++) {
        int amt = sPtr[i]->getAmount();
        amt = sPtr[i]->getAmount();

        switch (amt) {
        case 5:
            renderItem(i);
            break;
        case 10:
            renderItem(i);
            break;
        case 15:
            renderItem(i);
            break;
        case 20:
            renderItem(i);
            break;
        }
    }
}

void renderItem(int shelf)
{
    const WORD colors[] = {
        0x55, 0x11, 0x60
    };

    COORD c;
    std::ostringstream ss;
    c.X = 27;
    c.Y = 0;
    int amt = sPtr[shelf]->getAmount();
    for (int x = 0; x <= shelf; x++) {
        c.Y += 6;
    }
    c.X = 26;
    for (int i = 0; i < amt / 5; i++) {
        c.X += 2;
        g_Console.writeToBuffer(c, " ", colors[shelf]);
    }
    c.X = 27;
    ss.str("");
    ss << amt;
    g_Console.writeToBuffer(c, ss.str(), colors[2]);
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
    COORD c = g_Console.getConsoleSize();
    c.Y /= 25;
    c.X = c.X / 2 - 5;
    g_Console.writeToBuffer(c, "Main Menu", 0xF0);
    c.Y += 8;
    c.X = g_Console.getConsoleSize().X / 6 + 20;
    if (g_ePreviousGameState == S_SPLASHSCREEN)
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
    map.chooseMap(0, g_Console);
    COORD c = g_Console.getConsoleSize();
    // Game Mechanic stuff
    c.Y /= 25;
    c.X = c.X / 2 - 5;
    g_Console.writeToBuffer(c, "Home", 0xF0);
    c.Y += 3;
    c.X = g_Console.getConsoleSize().X / 8;
    g_Console.writeToBuffer(c, "Son 1", 0xF0);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 8;
    g_Console.writeToBuffer(c, "State : ", 0xF0);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 8;
    g_Console.writeToBuffer(c, "X days without medicine", 0xF0); //Make this hidden according to Son 1 state
    c.Y += 2;
    c.X = g_Console.getConsoleSize().X / 8;
    g_Console.writeToBuffer(c, "Food (Price) [ ] ", 0xF0);
    c.Y += 3;
    c.X = g_Console.getConsoleSize().X / 8;
    g_Console.writeToBuffer(c, "Rent (Price) [ ] ", 0xF0);
    c.Y += 4;
    c.X = g_Console.getConsoleSize().X / 8;
    g_Console.writeToBuffer(c, "Son 2", 0xF0);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 8;
    g_Console.writeToBuffer(c, "State : ", 0xF0);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 8;
    g_Console.writeToBuffer(c, "X days without medicine", 0xF0); //Make this hidden according to Son 2 state
    c.Y += 2;
    c.X = g_Console.getConsoleSize().X / 8;
    g_Console.writeToBuffer(c, "Food (Price) [ ] ", 0xF0);

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

void renderEndOfWorkScreen()
{
    map.chooseMap(0, g_Console);
    COORD c = g_Console.getConsoleSize();
    c.Y /= 25;
    c.X = c.X / 2 - 10;
    g_Console.writeToBuffer(c, "End of day report", 0xF0);
    c.Y += 8;
    c.X = g_Console.getConsoleSize().X / 6 + 15;
    g_Console.writeToBuffer(c, "Customers served: [ ]", 0xF0);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 6 + 15;
    g_Console.writeToBuffer(c, "Complaints given: [ ]", 0xF0);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 6 + 15;
    g_Console.writeToBuffer(c, "Strikes: [ ]", 0xF0);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 6 + 15;
    g_Console.writeToBuffer(c, "Todays pay: [ ]", 0xF0);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 6 + 15;
    g_Console.writeToBuffer(c, "Click here to go home", 0xF0);
}

void renderTutorialLevel()
{
    map.chooseMap(1, g_Console);
}

void renderBoxes() 
{   
    g_Console.writeToBuffer(boxPosPtr->getX(), boxPosPtr->getY(), ' ', BoxColour);
}

void renderCustomer()
{   
    COORD c = g_Console.getConsoleSize();
    c.X = 79;
    c.Y = 13;
    int time = g_dElapsedWorkTime;
    
        for (int i = 0; i < 6; i++) {
            if (customerPtr[i] == nullptr) {
                customerPtr[i] = new Customer; // spawn customer PS:needs to delete the customer 
                c.X = customerPtr[i]->getPos().getX();
                c.Y = customerPtr[i]->getPos().getY();
                /*g_Console.writeToBuffer(c, ' ', 0x77);*/
            }
        }
        for (int i = 0; i < 6; i++)
        {
            if (customerPtr[i] != nullptr && time % 20 != 0) {
                int num = rand() % 6 + 1; // randomizing customer item

                switch (i) {
                case 0:
                    if (time % 10 != 0) {
                        c.X = 79;
                        c.Y = 13;
                        g_Console.writeToBuffer(c, ' ', 0x20);
                        map.setGrid(c.Y, c.X, 'C');
                    }
                case 1:
                    if (time % 35 != 1) {
                        c.X = 37;
                        c.Y = 7;

                        sPtr[0]->decreaseItem();
                        g_Console.writeToBuffer(c, ' ', 0x20);
                        map.setGrid(c.Y, c.X, 'C');
                    }
                case 2:
                   if (time % 35 != 2) {
                        c.X = 37;
                        c.Y = 13;
                        sPtr[1]->decreaseItem();
                        g_Console.writeToBuffer(c, ' ', 0x20);
                        map.setGrid(c.Y, c.X, 'C');
                   };
            }
        }
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



