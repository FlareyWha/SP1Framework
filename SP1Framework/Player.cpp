#include "Player.h"

Player::Player() : strikes{ 0 }, savings{ 0 }, dayEarnings{ 0 }, unsatisfiedCustomers{ 0 }, p_bRentPaid(false), rent{ 200 }, medicine{ 100 }, food{ 20 }
{
	holdingProduct = false;
}

Player::~Player() {

}

Powerup Player::getPowerups()
{
	return Powerup();
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
	dayEarnings = (i * (5 + 2 * (getPowerups().getRCustomerslvl()))) + dayEarnings;
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

void Player::payRent() {
	savings = savings - rent;
}

void Player::payFood() {
	savings = savings - food;
}

void Player::payMedicine() {
	savings = savings - medicine;
}

void Player::isRentPaid()
{
	if (!p_bRentPaid) {
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
