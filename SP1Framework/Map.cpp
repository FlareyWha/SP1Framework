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


int Map::getGrid(int y, int x)
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
	const int charMapX = 80; //initializes charMap
	const int charMapY = 25;
	char charMap[charMapX][charMapY];
	string output;
	for (int pGx = 0; pGx < charMapX; pGx++)
	{
		for (int pGy = 0; pGy < charMapY; pGy++)
		{
			charMap[pGx][pGy] = ' ';
		}
	}

	for (int pGy = 0; pGy < charMapY; pGy++)
	{
		c.Y = pGy;
		getline(level, output);
		for (int pGx = 0; pGx < charMapX; pGx++)
		{
			c.X = pGx;
			charMap[pGx][pGy] = output[pGx];
			switch (charMap[pGx][pGy])
			{
			case ' ':
				console.writeToBuffer(c, ' ', 0x00); //void -- black
				break;
			case 'a':
				console.writeToBuffer(c, ' ', 0x80); //wall -- dark gray
				break;
			case 'b':
				console.writeToBuffer(c, ' ', 0x60); //shelf -- dark yellow
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
				console.writeToBuffer(c, ' ', 0x20); //vegetables -- dark green
				break;
			case '6':
				console.writeToBuffer(c, ' ', 0x90); //bandages -- light blue
				break;
			}
		}
	}
}

//{
//	COORD c = console.getConsoleSize();
//	std::string output;
//	/*
//	int** mapArray = new int* [row];
//
//	for (int y = 0; y < row; y++)
//	{
//		c.Y = y;
//		*(mapArray + y) = new int[column];
//		getline(level, output);
//		
//		for (int x = 0; x < column; x++)
//		{
//			c.X = x;
//			int value;
//			value = output[x] - 48;
//			*(*(mapArray + y) + x) = value;
//	*/
//	for (int y = 0; y < 25; y++)
//	{
//		c.Y = y;
//		getline(level, output);
//
//		for (int x = 0; x < 80; x++)
//		{
//			c.X = x;
//			int value;
//			value = output[x] - 48;
//			mapArray[y][x] = value;
//
//			switch (mapArray[y][x])
//			{
//			case 0:
//				console.writeToBuffer(c, ' ', 0xF0); //white
//				break;
//			case 1:
//				console.writeToBuffer(c, ' ', 0x88); //gold
//				break;
//			case 2:
//				console.writeToBuffer(c, ' ', 0x66); //dark gray
//				break;
//			case 3:
//				console.writeToBuffer(c, ' ', 0x77); //light gray
//				break;
//			case 4:
//				console.writeToBuffer(c, ' ', 0x55); //toilet paper purple
//				break;
//			case 5:
//				console.writeToBuffer(c, ' ', 0x111); //instant noodle dark blue
//				break;
//			case 6:
//				console.writeToBuffer(c, ' ', 0xBB); //canned food teal
//				break;
//			case 7:
//				console.writeToBuffer(c, ' ', 0xEE); //rice cream
//				break;
//			case 8:
//				console.writeToBuffer(c, ' ', 0xAA); //vegetable green
//				break;
//			case 9:
//				console.writeToBuffer(c, ' ', 0x99); //bandages blue
//				break;
//
//
//			}
//		}
//
//		//for (int y = 0; y < row; y++)
//		//{
//		//	delete[] (mapArray + y);
//		//}
//
//		//delete[] mapArray;
//	}
//
//	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//
//	//figure out how to delete map ltr
//}
