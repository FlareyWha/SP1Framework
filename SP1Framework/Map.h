#pragma once
#include <iostream>
#include <fstream>
#include "Framework\console.h"

class Map
{
private:
	char mapArray[25][80];

public:
	Map();
	~Map();
	
	char getGrid(int y, int x);
	void setGrid(int y, int x, char character);
	void chooseMap(int lvl, Console& console);
	void printMap(std::fstream& level, Console& console);
	char collision(int currentY, int currentX, int moveToY, int moveToX, Map& map);
	bool isNegative(int num1, int num2);
};

