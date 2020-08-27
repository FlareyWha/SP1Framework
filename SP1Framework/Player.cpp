#include "Player.h"

Player::Player() : strikes{ 0 }, savings{ 0 }, dayEarnings{ 0 }, unsatisfiedCustomers{ 0 }, p_bRentPaid(false), rent{ 200 }, medicine{ 100 }, food{ 30 }, PowerupsPtr{ new Powerup }
{
	holdingProduct = false;
	pos.setX(18);
	pos.setY(1);
}

Player::~Player() {

}

Powerup* Player::getPowerups()
{
	return PowerupsPtr;
}

int Player::getStrikes() {
	return strikes;
}

int Player::getSavings() {
	return savings;
}

int Player::getTotalEarned()
{
	return dayEarnings;
}

void Player::AddDayEarnings(int i)
{
	dayEarnings = (i * (5 + 2 * (getPowerups()->getRCustomerslvl()))) + dayEarnings;
}

void Player::resetDayEarnings()
{
	dayEarnings = 0;
}

int Player::getUnsatisfiedCustomers()
{
	return unsatisfiedCustomers;
}

void Player::increaseUnsatisfiedCustomers()
{
	unsatisfiedCustomers++;
}

void Player::resetUnsatisfiedCustomers()
{
	unsatisfiedCustomers = 0;
}

void Player::receiveStrike() {
	strikes++;
}

void Player::receivePay(int pay) {
	savings = savings + pay;
}

void Player::takePay(int pay)
{
	savings -= pay;
}

void Player::payRent() {
	isRentPaid();
	if (p_bRentPaid == true) {
		savings = savings - rent;
	}
	else if (p_bRentPaid == false) {
		receivePay(rent);
	}
}

void Player::payFood(Son* cPtr) {
	cPtr->isFed();
	if (cPtr->getStatusFed() == true) {
		savings = savings - food;
	}
	else if (cPtr->getStatusFed() == false) {
		receivePay(food);
	}
}

void Player::payMedicine(Son* cPtr) {
	cPtr->isTreated();
	if (cPtr->getTreatState() == true) {
		savings = savings - medicine;
	}
	else if (cPtr->getTreatState() == false) {
		receivePay(medicine);
	}
}

void Player::isRentPaid()
{
	if (p_bRentPaid == false) {
		p_bRentPaid = true;
	}
	else if (p_bRentPaid == true) {
		p_bRentPaid = false;
	}
}

bool Player::getRentStatus()
{
	return p_bRentPaid;
}

void Player::holdsProduct() {
	holdingProduct = true;
}

bool Player::isHoldingProduct() {
	return holdingProduct;
}

void Player::releaseProduct() {
	holdingProduct = false;
}

void Player::setRent(int r) {
	rent = r;
}
void Player::setFood(int f) {
	food = f;
}
void Player::setMedicine(int m) {
	medicine = m;
}

int Player::getRent()
{
	return rent;
}

int Player::getFood()
{
	return food;
}

int Player::getMedicine()
{
	return medicine;
}

void Player::purchasecheaperFood(Player* p)
{
	if (PowerupsPtr->getFoodlvl() < 3) {
		PowerupsPtr->incFoodlvl();
		p->takePay(50);
		p->setFood(30 - PowerupsPtr->getFoodlvl() * 5);
	}
}

void Player::purchasecheaperRent(Player* p)
{
	if (PowerupsPtr->getRentlvl() < 3) {
		PowerupsPtr->incRentlvl();
		p->takePay(50);
		p->setRent(200 - PowerupsPtr->getRentlvl() * 15);
	}
}

void Player::purchaseplayerShoes(Player* p)
{
	if (PowerupsPtr->getShoeslvl() < 2) {
		PowerupsPtr->incShoeslvl();
		p->takePay(100);
	}
}

void Player::purchaseslowerCustomers(Player* p)
{
	if (PowerupsPtr->getSCustomerslvl() < 2) {
		PowerupsPtr->incSCustomerslvl();
		p->takePay(100);
	}
}

void Player::purchaserichCustomers(Player* p)
{
	if (PowerupsPtr->getRCustomerslvl() < 3) {
		PowerupsPtr->incRCustomerslvl();
		p->takePay(50);
	}
}

void Player::purchasethriftyCustomers(Player* p)
{
	if (PowerupsPtr->getTCustomerslvl() < 3) {
		PowerupsPtr->incTCustomerslvl();
		p->takePay(50);
	}
}