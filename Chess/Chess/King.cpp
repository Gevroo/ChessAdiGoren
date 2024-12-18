#include "King.h"
#include <math.h>
King::King(int x, int y, bool isWhite) : Piece(x, y, 'k', isWhite) {}

bool King::moveValid(int x, int y)
{
	return abs(this->_x - x) < 2 && abs(this->_y - y) < 2;
}
