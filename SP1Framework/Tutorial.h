#pragma once
#include "Framework/console.h"
#include "game.h"
#include "Player.h"
#include "Box.h"
class Tutorial
{
private:
	bool tutorialFlags[10];
	bool pressed[6];
	bool allTrue;

public:
	Tutorial();
	~Tutorial();
	void tutorial(Console& console, SGameChar& g_sChar, SMouseEvent& g_mouseEvent, SKeyEvent g_skKeyEvent[K_COUNT], double g_dElaspedWorkTime, Player p, WORD boxColour);
	void flagOne(Console& console);
	void flagTwo(Console& console, SGameChar& g_sChar, SKeyEvent g_skKeyEvent[K_COUNT]);
	void flagThree(Console& console, SGameChar& g_sChar, SKeyEvent g_skKeyEvent[K_COUNT]);
	void flagFour(Console& console, SGameChar& g_sChar, SKeyEvent g_skKeyEvent[K_COUNT], WORD boxColour);

	bool getTutorialFlag(int number);
};

