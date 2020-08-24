#include "Son.h"

Son::Son() : bSick{ false }, btoggleFood{ false }, NoOfDaysUnfed{0}
{
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

void Son::increaseNODUnfed()
{
	NoOfDaysUnfed++;
}

void Son::decreaseNODUnfed()
{
	NoOfDaysUnfed--;
}

void Son::resetNODUnfed()
{
	NoOfDaysUnfed = 0;
}

int Son::getNODUnfed()
{
	return NoOfDaysUnfed;
}

void Son::ChancesOfFallingSick(int daysUnfed) {
	int getsSick = rand() % 100 + 1;
	if (getsSick <= 20) {
		isSick();
	}
}

