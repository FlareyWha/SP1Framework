#include "Son.h"
#include "Player.h"

Son::Son() : bSick{ false }, btoggleFood{ false }, bHospitalised{ false }, NoOfDaysUnfed{ 0 }, NoOfDaysSick { 0 }
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

void Son::isHosp()
{
	bHospitalised = true;
}

void Son::resetHospState()
{
	bHospitalised = false;
}

bool Son::getHospState()
{
	return bHospitalised;
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

void Son::resetFed()
{
	btoggleFood = false;
}

bool Son::getStatusFed()
{
	return btoggleFood;
}

void Son::isTreated()
{
	if (btoggleMedicine == false) {
		btoggleMedicine = true;
	}
	else {
		btoggleMedicine = false;
	}
}

bool Son::getTreatState()
{
	return btoggleMedicine;
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

void Son::increaseNODSick()
{
	NoOfDaysSick++;
}

void Son::resetNODSick()
{
	NoOfDaysSick = 0;
}

int Son::getNODSick()
{
	return NoOfDaysSick;
}

void Son::ChancesOfFallingSick(int daysUnfed) {
	int getsSick{};
	if (daysUnfed == 0) {
		getsSick = rand() % 100 + 1;
	}
	else if (daysUnfed != 0) {
		int multiplier = 100 - (daysUnfed * 15);
		getsSick = rand() % multiplier + 1;
	}

	if (getsSick <= 15) {
		isSick();
	}
}

