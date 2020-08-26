#include "Tutorial.h"
#include <sstream>
#include <Windows.h>

Tutorial::Tutorial()
{
	for (int i = 0; i < 10; i++)
	{
		tutorialFlags[i] = false;
		
		if (i < 5)
			moved[i] = false;
	}
    allTrue = true;
}

Tutorial::~Tutorial()
{
}

void Tutorial::tutorial(Console& console, SGameChar& g_sChar, SMouseEvent& g_mouseEvent, SKeyEvent g_skKeyEvent[K_COUNT], double g_dElaspedWorkTime)
{
    if (tutorialFlags[0] == false)
        flagOne(console);
    else if (tutorialFlags[1] == false)
        flagTwo(console, g_sChar, g_skKeyEvent);
    else if (tutorialFlags[2] == false)
        flagThree(console, g_sChar, g_skKeyEvent);

    if ((g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) && (tutorialFlags[0] == false) && (g_dElaspedWorkTime > 1))
        tutorialFlags[0] = true;
    else if (allTrue == true && tutorialFlags[0] == true && tutorialFlags[1] == false)
        tutorialFlags[1] = true;
    else if (g_sChar.m_cLocation.X == 3 && g_sChar.m_cLocation.Y == 3 && moved[4] == true && tutorialFlags[1] == true && tutorialFlags[2] == false)
        tutorialFlags[2] = false;

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

    for (int i = 0; i < 4; i++)
    {
        c.Y += 1;
        console.writeToBuffer(c, movementKeys[i], 0xF0);

        if (moved[i] == true)
        {
            c.X += 3;
            console.writeToBuffer(c, ' ', 0xAA);
            c.X -= 3;
            allTrue = true;
        }
        else
            allTrue = false;
    }

    if (g_skKeyEvent[K_UP].keyDown)
    {
        moved[0] = true;
    }
    if (g_skKeyEvent[K_LEFT].keyDown)
    {
        moved[1] = true;
    }
    if (g_skKeyEvent[K_DOWN].keyDown)
    {
        moved[2] = true;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown)
    {
        moved[3] = true;
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

    if (moved[4] == true)
    {
        c.X += 7;
        console.writeToBuffer(c, ' ', 0xAA);
        c.X -= 7;
    }

    if (g_skKeyEvent[K_SHIFT].keyDown)
    {
        moved[4] = true;
    }
}

bool Tutorial::getTutorialFlag(int number)
{
    return tutorialFlags[number];
}
