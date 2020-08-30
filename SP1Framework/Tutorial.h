#pragma once
#include "Framework/console.h"
#include "game.h"
#include "Player.h"
#include "Box.h"
class Tutorial
{
private:
	bool tutorialFlags[20];
	bool pressed[6];
	bool allTrue;
	bool proceed;
	bool alreadyPlayed[20];

public:
	Tutorial();
	~Tutorial();
	void tutorial(Console& console, SGameChar& g_sChar, SMouseEvent& g_mouseEvent, SKeyEvent g_skKeyEvent[K_COUNT], double g_dElaspedWorkTime, Player p, WORD boxColour, double& tutorialTimer);
	void flagOne(Console& console);
	void flagTwo(Console& console, SGameChar& g_sChar, SKeyEvent g_skKeyEvent[K_COUNT]);
	void flagThree(Console& console, SGameChar& g_sChar, SKeyEvent g_skKeyEvent[K_COUNT]);
	void flagFour(Console& console, SGameChar& g_sChar, SKeyEvent g_skKeyEvent[K_COUNT], WORD boxColour);
	void flagFive(Console& console, SGameChar& g_sChar, SKeyEvent g_skKeyEvent[K_COUNT], WORD boxColour);
	void flagSix(Console& console);
	void flagSeven();
	void flagEight(Console& console);
	void flagNine(Console& console);
	void flagTen(Console& console);
	void flagEleven(Console& console);

	bool getTutorialFlag(int number);
	void setTutorialFlag(int number, bool set);
	bool getProceed(void);
};

