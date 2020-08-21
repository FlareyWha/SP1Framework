#include "Son.h"

Son::Son() : bSick{ false }, btoggleFood{ false }{
}

Son::~Son() {
	 
}

void Son::isSick() {
	bSick = true;
}

void Son::Recovers() {
	bSick = false;
}

bool Son::getStatus() {
	return bSick;
}

void Son::isFed()
{
	if (btoggleFood == false) {
		btoggleFood = true;
	}
	else
	{
		btoggleFood = false;
	}
}

bool Son::getStatusFed()
{
	return btoggleFood;
}

void Son::ChancesOfFallingSick(Son s) {
	int getsSick = rand() % 10 + 1;
	if (getsSick == 1) {
		s.isSick();
	}
}