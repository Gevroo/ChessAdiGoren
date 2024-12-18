#include "Player.h"

Player::Player(bool isWhite, int direction)
{
	//this->_king = new King(x,y, 'k', true);
	this->_direction = direction;
	this->_white = isWhite;
}
int Player::getDirection() const
{
	return this->_direction;
}
Piece* Player::getKing() const
{
	return this->_king;
}
void Player::setKing(Piece* all_hail_the_king)
{
	this->_king = all_hail_the_king;
}