#include "Tutorial.h"
#include <sstream>
#include <Windows.h>

Tutorial::Tutorial(): tutorialFlags {false}, pressed {false}  //add in tutorial if anything go wrong just hide in your box **************
{
	for (int i = 0; i < 20; i++)
	{
		tutorialFlags[i] = false;
        alreadyPlayed[i] = false;
		
		if (i < 6)
            pressed[i] = false;
	}
    allTrue = true;
    proceed = true;
}

Tutorial::~Tutorial()
{
}

void Tutorial::tutorial(Console& console, SGameChar& g_sChar, SMouseEvent& g_mouseEvent, SKeyEvent g_skKeyEvent[K_COUNT], double g_dElaspedWorkTime, Player p, WORD boxColour, double& tutorialTimer)
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
    else if (tutorialFlags[5] == false) // from this point on the flag will match the flag aka 6 = 6 
        flagSix(console);
    else if (tutorialFlags[6] == false)
        flagSeven();
    else if (tutorialFlags[7] == false)
        flagEight(console);
    else if (tutorialFlags[8] == false)
        flagNine(console);
    else if (tutorialFlags[9] == false)
        flagTen(console);
    else if (tutorialFlags[10] == false)
        flagEleven(console);
    


    // Plays TTS for tutorial instructions
    if (alreadyPlayed[0] == false && tutorialFlags[0] == false) {
        PlaySound(L"TutorialTTSF1.wav", NULL, SND_FILENAME | SND_ASYNC);
        alreadyPlayed[0] = true;
    }
    else if (alreadyPlayed[1] == false && tutorialFlags[1] == false && tutorialFlags[0] == true) {
        PlaySound(L"TutorialTTSF2.wav", NULL, SND_FILENAME | SND_ASYNC);
        alreadyPlayed[1] = true;
    }
    else if (alreadyPlayed[2] == false && tutorialFlags[2] == false && tutorialFlags[1] == true) {
        PlaySound(L"TutorialTTSF3.wav", NULL, SND_FILENAME | SND_ASYNC);
        alreadyPlayed[2] = true;
    }
    else if (alreadyPlayed[3] == false && tutorialFlags[3] == false && tutorialFlags[2] == true) {
        PlaySound(L"TutorialTTSF4.wav", NULL, SND_FILENAME | SND_ASYNC);
        alreadyPlayed[3] = true;
    }
    else if (alreadyPlayed[4] == false && tutorialFlags[4] == false && tutorialFlags[3] == true) {
        PlaySound(L"TutorialTTSF5.wav", NULL, SND_FILENAME | SND_ASYNC);
        alreadyPlayed[4] = true;
    }
    else if (alreadyPlayed[5] == false && tutorialFlags[5] == false && tutorialFlags[4] == true) {
        PlaySound(L"TutorialTTSF6.wav", NULL, SND_FILENAME | SND_ASYNC);
        alreadyPlayed[5] = true;
    }
    /*else if (alreadyPlayed[6] == false && tutorialFlags[6] == false && tutorialFlags[5] == true) {
        PlaySound(L"BGM1.wav", NULL, SND_FILENAME | SND_ASYNC);
        alreadyPlayed[6] = true;
    }*/
    /*else if (alreadyPlayed[6] == false && tutorialFlags[6] == false && tutorialFlags[7] == true) {
        PlaySound(L"IntenseMusic(30sec).wav", NULL, SND_FILENAME | SND_ASYNC);
        alreadyPlayed[6] = true;
    }*/

    if ((g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) && (tutorialFlags[0] == false) && (tutorialTimer > 1))
        tutorialFlags[0] = true;
    else if (allTrue == true && tutorialFlags[0] == true && tutorialFlags[1] == false)
        tutorialFlags[1] = true;
    else if (p.getPos().getX() == 3 && p.getPos().getY() == 3 && tutorialFlags[1] == true && tutorialFlags[2] == false)
        tutorialFlags[2] = true;
    else if (boxColour == 0x50 && pressed[5] == true && tutorialFlags[2] == true && tutorialFlags[3] == false)
    {
        tutorialFlags[3] = true;
        pressed[5] = false;
    }
    else if (boxColour == 0x70 && pressed[5] == true && tutorialFlags[3] == true && tutorialFlags[4] == false)
    {
        tutorialFlags[4] = true;
        proceed = false;
        tutorialTimer = 0;
    }
    else if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED && tutorialTimer > 1 && tutorialFlags[4] == true && tutorialFlags[5] == false)
    {
        tutorialFlags[5] = true;
        proceed = true;
        PlaySound(L"IntenseMusic(30).wav", NULL, SND_FILENAME | SND_ASYNC);
        g_sChar.moving.UP = false;
        g_sChar.moving.DOWN = false;
        g_sChar.moving.LEFT = false;
        g_sChar.moving.RIGHT = false;
    }
    else if (g_dElaspedWorkTime >= 30 && tutorialFlags[5] == true && tutorialFlags[6] == false)
    {
        tutorialFlags[6] = true;
        tutorialTimer = 0;
    }
    else if (tutorialTimer >= 10 && tutorialFlags[6] == true && tutorialFlags[7] == false)
    {
        tutorialFlags[7] = true;
    }
    else if (tutorialTimer >= 30 && tutorialFlags[7] == true && tutorialFlags[8] == false)
    {
        tutorialFlags[8] = true;
    }
    else if (tutorialTimer >= 120 && tutorialFlags[8] == true && tutorialFlags[9] == false)
    {
        tutorialFlags[9] = true;
    }
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
    console.writeToBuffer(c, "must touch it. Also, safety is key ", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "press Shift to walk to avoid accidents.", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "Try it! Optional: Shift { }", 0xF0);

    if (pressed[4] == true)
    {
        c.X += 25;
        console.writeToBuffer(c, ' ', 0xAA);
        c.X -= 25;
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
    console.writeToBuffer(c, "Press Space when your grey box", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "is touching the purple box.", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "hit Space to pick the item! { }", 0xF0);

    if (pressed[5] == true && boxColour == 0x50)
    {
        c.X += 29;
        console.writeToBuffer(c, ' ', 0xAA);
        c.X -= 29;
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
    console.writeToBuffer(c, "to stock up the shelves. Go", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "to the shelf marked with the", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "same colour as the item you are", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "holding to restock. Press space", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "on the grey area of the shelf", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "Restocked shelf { }", 0xF0);

    if (pressed[5] == true && boxColour == 0x70)
    {
        c.X += 17;
        console.writeToBuffer(c, ' ', 0xAA);
        c.X -= 17;
    }

    if (g_skKeyEvent[K_SPACE].keyDown)
    {
        pressed[5] = true;
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
    console.writeToBuffer(c, "to the best of your ability.", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "Click to continue.", 0xF0);
}

void Tutorial::flagSeven()
{
    //yes
}

void Tutorial::flagEight(Console& console)
{
    COORD c;

    c.Y = 4;
    c.X = 40;
    console.writeToBuffer(c, "A customer has appeared. They will", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "wait at the entrance for 10 secs", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "until they find an item to buy.", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "The number on the customer", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "represents how much of an item they", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "want to buy. For now, keep on", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "restocking.", 0xF0);
}

void Tutorial::flagNine(Console& console)
{
    COORD c;

    c.Y = 14;
    c.X = 17;
    console.writeToBuffer(c, "The customer is moving towards the shelf. They will wait there", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "for 20 secs. After that, if they got what they want, they will", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "leave and be happy. If not, they will be unsatisfied. You can", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "see the number of unsatisfied customers at the top left. If it", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "reaches 10, it is game over as I will fire you. Our ", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "customers can be very rough, so try not to stand in their way", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "or they will push you. Also, if you are wondering why they", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "are still going while you are reading this, its because im", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "preparing you for your future. Thank me later.", 0xF0);
}

void Tutorial::flagTen(Console& console)
{
    COORD c;

    c.Y = 19;
    c.X = 17;
    console.writeToBuffer(c, "The customer is now leaving. For each item a customer buys,", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "you will earn money. The amount you earn is shown on the top", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "right. You now have about 1min 30secs till the supermarket", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "closes. You can see the time left on the top middle.", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "Satisfy as many customers as you can. Good luck!", 0xF0);
}

void Tutorial::flagEleven(Console& console)
{
    COORD c;

    c.Y = 10;
    c.X = 17;
    console.writeToBuffer(c, "This is the end-of-work screen. After each day, I will summarise", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "what has happened throughout the day. Go to the home screen", 0xF0);
    c.Y += 1;
    console.writeToBuffer(c, "to continue", 0xF0);
}

bool Tutorial::getTutorialFlag(int number)
{
    return tutorialFlags[number];
}

void Tutorial::setTutorialFlag(int number, bool set)
{
    tutorialFlags[number] = set;
}

bool Tutorial::getProceed(void)
{
    return proceed;
}
