#include "Queen.h"


bool Queen::moveValid(int x, int y)
{
	return Rook::moveValid(x, y) || Bishop::moveValid(x, y);
}