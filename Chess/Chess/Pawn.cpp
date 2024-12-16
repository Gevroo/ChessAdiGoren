#include "Pawn.h"


bool Pawn::moveValid(int x, int y)
{
	return (this->_x + this->_direction == x && _y == y) || (this->_x + this->_direction * 2 == x && _y == y && !this->_moved);
}
Pawn::Pawn(int x, int y, bool isWhite, int direction) : Piece(x,y,'p', isWhite)
{
	this->_direction = direction;
	this->_moved = false;
}
