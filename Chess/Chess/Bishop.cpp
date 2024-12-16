#include "Bishop.h"

bool Bishop::moveValid(int x, int y)
{
	return abs(this->_x - x) == abs(this->_y - y);//Checking if its in the same diagonal line 
}
Bishop::Bishop(int x, int y, bool isWhite): Piece(x, y, 'b', isWhite) {}