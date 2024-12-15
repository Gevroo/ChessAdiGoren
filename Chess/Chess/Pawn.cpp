#include "Pawn.h"


bool Pawn::moveValid(int x, int y)
{
	return (this->_y + this->_direction == y) || (this->_y + this->_direction * 2 == y && !this->_moved);
}
//Pawn::Pawn(int x, int y, bool isWhite, int direction) : Piece(x,y,isWhite)
//{
//	this->_direction = direction;
//}
