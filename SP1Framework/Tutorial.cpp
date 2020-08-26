#include "Tutorial.h"
#include <sstream>
#include <Windows.h>

Tutorial::Tutorial(): tutorialFlags {false}, pressed {false}
{
	for (int i = 0; i < 10; i++)
	{
		tutorialFlags[i] = false;
		
		if (i < 7)
            pressed[i] = false;
	}
    allTrue = true;
}

Tutorial::~Tutorial()
{
}

void Tutorial::tutorial(Console& console, SGameChar& g_sChar, SMouseEvent& g_mouseEvent, SKeyEvent g_skKeyEvent[K_COUNT], double g_dElaspedWorkTime, Player p, WORD boxColour)
{
    if (tutorialFlags[0] == false)
        flagOne(console);
    else if (tutorialFlags[1] == false)
        flagTwo(console, g_sChar, g_skKeyEvent);
    else if (tutorialFlags[2] == false)
        flagThree(console, g_sChar, g_skKeyEvent);
    else if (tutorialFlags[3] == false)
        flagFour(console, g_sChar, g_skKeyEvent, boxColour);
    else if (tutorialFlags[4] == false)
        flagFive(console, g_sChar, g_skKeyEvent, boxColour);

    if ((g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) && (tutorialFlags[0] == false) && (g_dElaspedWorkTime > 1))
        tutorialFlags[0] = true;
    else if (allTrue == true && tutorialFlags[0] == true && tutorialFlags[1] == false)
        tutorialFlags[1] = true;
    else if (p.getPos().getX() == 3 && p.getPos().getX() == 3 && pressed[4] == true && tutorialFlags[1] == true && tutorialFlags[2] == false)
        tutorialFlags[2] = true;
    else if (boxColour == 0x50 && pressed[5] == true && tutorialFlags[2] == true && tutorialFlags[3] == false)
        tutorialFlags[3] = true;
    else if (boxColour == 0x70 && pressed[5] == true && tutorialFlags[3] == true && tutorialFlags[4] == false)
        tutorialFlags[4] = true;
    else if (g_dElaspedWorkTime >= 30 && tutorialFlags[4] == true && tutorialFlags[5] == false)
        tutorialFlags[5] = true;
}

void Tutorial::flagOne(Console& console)
{
    COORD c;
    c.Y = 4;
    c.X = 40;
    console.writeToBuffer(c, "Welcome to Jackville Supermarket!", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "As it is your first day on the job,", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "I, the manager, will tell you what", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "you have to do. Click the screen", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "to continue.", 0xF0);
}

void Tutorial::flagTwo(Console& console, SGameChar& g_sChar, SKeyEvent g_skKeyEvent[K_COUNT])
{
    COORD c;
    std::string movementKeys[4] = { "W { }", "A { }", "S { }", "D { }" };
    c.Y = 4;
    c.X = 40;
    console.writeToBuffer(c, "Use your WASD keys to move up,", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "left, down and right respectively", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "Press each key once to proceed.", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "left, down and right respectively", 0xF0);

    if (g_skKeyEvent[K_UP].keyDown)
    {
        pressed[0] = true;
        allTrue = true;
    }
    if (g_skKeyEvent[K_LEFT].keyDown)
    {
        pressed[1] = true;
        allTrue = true;
    }
    if (g_skKeyEvent[K_DOWN].keyDown)
    {
        pressed[2] = true;
        allTrue = true;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown)
    {
        pressed[3] = true;
        allTrue = true;
    }

    for (int i = 0; i < 4; i++)
    {
        c.Y += 1;
        console.writeToBuffer(c, movementKeys[i], 0xF0);

        if (pressed[i] == true)
        {
            c.X += 3;
            console.writeToBuffer(c, ' ', 0xAA);
            c.X -= 3;
        }
        else
            allTrue = false;
    }
}

void Tutorial::flagThree(Console& console, SGameChar& g_sChar, SKeyEvent g_skKeyEvent[K_COUNT])
{
    COORD c;
    c.Y = 4;
    c.X = 40;
    console.writeToBuffer(c, "Great! Now move towards the purple", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "box on the left. Your empty box (grey)", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "must touch it. Also, if u have trouble", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "moving, press shift to walk. Try it!", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "Shift { }", 0xF0);

    if (pressed[4] == true)
    {
        c.X += 7;
        console.writeToBuffer(c, ' ', 0xAA);
        c.X -= 7;
    }

    if (g_skKeyEvent[K_SHIFT].keyDown)
    {
        pressed[4] = true;
    }
}

void Tutorial::flagFour(Console& console, SGameChar& g_sChar, SKeyEvent g_skKeyEvent[K_COUNT], WORD boxColour)
{
    COORD c;
    c.Y = 4;
    c.X = 40;
    console.writeToBuffer(c, "Press space when ur grey box", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "is touching the purple box", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "Space { }", 0xF0);

    if (pressed[5] == true && boxColour == 0x50)
    {
        c.X += 7;
        console.writeToBuffer(c, ' ', 0xAA);
        c.X -= 7;
    }

    if (g_skKeyEvent[K_SPACE].keyDown)
    {
        pressed[5] = true;
    }
}

void Tutorial::flagFive(Console& console, SGameChar& g_sChar, SKeyEvent g_skKeyEvent[K_COUNT], WORD boxColour)
{
    COORD c;
    c.Y = 4;
    c.X = 40;
    console.writeToBuffer(c, "Now that you are holding", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "some toilet paper, its time", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "to put it on the shelf. Go", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "to the shelf marked with the", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "same colour as the item you are", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "holding to restock it. Press space", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "on the grey area of the shelf", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "Restocked shelf { }", 0xF0);

    if (pressed[5] == true && boxColour == 0x70)
    {
        c.X += 18;
        console.writeToBuffer(c, ' ', 0xAA);
        c.X -= 18;
    }

    if (g_skKeyEvent[K_SPACE].keyDown)
    {
        pressed[6] = true;
    }
}

void Tutorial::flagSix(Console& console)
{
    COORD c;
    c.Y = 4;
    c.X = 40;
    console.writeToBuffer(c, "Good job! You're one of the best", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "workers we have seen to date! Now,", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "the customers will start coming soon.", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "Use the 30 seconds left before the", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "supermarket opens to restock the shelves", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "to the best of your ability. A recap of", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "the controls are here for you to look at.", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "Move: W (up), A (left), S (down), D (right)", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "Shift to walk, Space when grey box is touching", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "coloured box to take it. Space on grey area on", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "shelf to restock the shelf.", 0xF0);

    //add something so that it freezes the game until u click so u have 30 seconds to restock.
    //also think of a way for phase 1 without havign to use g_dElaspedWorkTime.

}

bool Tutorial::getTutorialFlag(int number)
{
    return tutorialFlags[number];
}
