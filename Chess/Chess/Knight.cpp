#include "Knight.h"
#include <math.h>

bool Knight::moveValid(int x, int y)
{
	return (abs(_x - x) == 1 && abs(_y - y) == 2) || (abs(_x - x) == 2 && abs(_y - y) == 1);
}
Knight::Knight(int x, int y, bool isWhite) : Piece(x, y, 'n', isWhite) {}