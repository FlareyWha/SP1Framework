#pragma once
#include "Framework/console.h"
#include "game.h"
class Tutorial
{
private:
	bool tutorialFlags[10];
	bool moved[5];
	bool allTrue;

public:
	Tutorial();
	~Tutorial();
	void tutorial(Console& console, SGameChar& g_sChar, SMouseEvent& g_mouseEvent, SKeyEvent g_skKeyEvent[K_COUNT], double g_dElaspedWorkTime);
	void flagOne(Console& console);
	void flagTwo(Console& console, SGameChar& g_sChar, SKeyEvent g_skKeyEvent[K_COUNT]);
	void flagThree(Console& console, SGameChar& g_sChar, SKeyEvent g_skKeyEvent[K_COUNT]);
	void flagFour(Console& console);

	bool getTutorialFlag(int number);
};

