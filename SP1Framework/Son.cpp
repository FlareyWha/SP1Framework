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
	int getsSick{};
	if (daysUnfed == 0) {
		getsSick = rand() % 100 + 1;
	}
	else if (daysUnfed != 0) {
		int multiplier = 100 / (daysUnfed * 1.1);
		getsSick = rand() % multiplier + 1;
	}

	if (getsSick <= 15) {
		isSick();
	}
}

