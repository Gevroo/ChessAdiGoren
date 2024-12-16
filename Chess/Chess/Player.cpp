#include "Player.h"

Player::Player(int x, int y, bool isWhite, int direction)
{
	this->_king = new King(x,y, 'k', true);
	this->_direction = direction;
	this->_white = isWhite;
}
int Player::getDirection() const
{
	return this->_direction;
}