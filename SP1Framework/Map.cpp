#include "Map.h"
#include "Framework\console.h"
#include <fstream>
#include <string>
#include <Windows.h>
#include <iostream>
using namespace std;	
Map::Map(): mapArray {0}
{
	//small is the normal console size, big is the map size.
} 

Map::~Map()
{
}


char Map::getGrid(int y, int x)
{
	return mapArray[y][x];
}

void Map::setGrid(int y, int x, char character)
{
	mapArray[y][x] = character;
}

void Map::chooseMap(int lvl, Console& console) //this is to choose the map to print out
{
	switch (lvl)
	{
	case 0:
	{
		std::fstream menu("MainMenu.txt");
		printMap(menu, console);
		break;
	}
	case 1:
	{
		std::fstream tutorialLevel("Tutorial level.txt");
		printMap(tutorialLevel, console);
		break;
	}
	}
}

//put in the level you want to print out, the size of how big it will be and the console obj
//if unsure follow case 1 example.
void Map::printMap(std::fstream& level, Console& console)
{
	COORD c = console.getConsoleSize();
	string output;

	for (int pGy = 0; pGy < 25; pGy++)
	{
		c.Y = pGy;
		getline(level, output);
		for (int pGx = 0; pGx < 80; pGx++)
		{
			c.X = pGx;
			mapArray[pGy][pGx] = output[pGx];
			switch (mapArray[pGy][pGx])
			{
			case ' ':
				console.writeToBuffer(c, ' ', 0x00); //void -- black
				break;
			case 'A':
				console.writeToBuffer(c, ' ', 0x80); //wall -- dark gray
				break;
			case 'B':
				console.writeToBuffer(c, ' ', 0x60); //shelf -- dark yellow
				break;
			case 'C':
				console.writeToBuffer(c, ' ', 0x20); //customer -- dark green
				break;
			case '0':
				console.writeToBuffer(c, ' ', 0xF0); //floor -- white
				break;
			case '1':
				console.writeToBuffer(c, ' ', 0x50); //toilet paper -- dark magenta
				break;
			case '2':
				console.writeToBuffer(c, ' ', 0x10); //instant noodles -- dark blue
				break;
			case '3':
				console.writeToBuffer(c, ' ', 0xB0); //canned food -- light cyan
				break;
			case '4':
				console.writeToBuffer(c, ' ', 0xE0); //rice -- light yellow
				break;
			case '5':
				console.writeToBuffer(c, ' ', 0xA0); //vegetables -- light green
				break;
			case '6':
				console.writeToBuffer(c, ' ', 0x90); //bandages -- light blue
				break;
			}
		}
	}
}

char Map::collision(int currentY, int currentX, int moveToY, int moveToX, Map& map)
{
	return (map.getGrid((currentY + (moveToY)), (currentX + (moveToX))));
}

