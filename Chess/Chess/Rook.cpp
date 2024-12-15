#include "Rook.h"


bool Rook::moveValid(int x, int y) 
{
	return x == this->_x || y == this->_y;
}
//Rook::Rook(int x, int y, bool isWhite) : Piece(x,y,isWhite){}