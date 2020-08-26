#include "Player.h"

Player::Player() : strikes{ 0 }, savings{0}, dayEarnings{0}, unsatisfiedCustomers{0}, p_bRentPaid(false)
{
	holdingProduct = false;
}

Player::~Player() {

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
	dayEarnings = i* 5 + dayEarnings;
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
	savings = savings - 200;
}

void Player::payFood() {
	savings = savings - 30;
}

void Player::payMedicine() {
	savings = savings - 100;
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
