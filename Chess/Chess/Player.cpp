#include "Player.h"

Player::Player(int x, int y, bool isWhite, int direction)
{
	this->_king = new King(x,y,isWhite,"k");
	this->_direction = direction;
	this->_white = isWhite;
}