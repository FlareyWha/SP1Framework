#include "Player.h"

Player::Player() : strikes{ 0 }, savings{0}, dayEarnings{0}
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

void Player::AddDayEarnings(int earned)
{
	dayEarnings += earned;
}

void Player::resetDayEarnings()
{
	dayEarnings = 0;
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

void Player::holdsProduct() {
	holdingProduct = true;
}

bool Player::isHoldingProduct() {
	return holdingProduct;
}

void Player::releaseProduct() {
	holdingProduct = false;
}
