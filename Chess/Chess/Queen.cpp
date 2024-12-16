#include "Queen.h"


bool Queen::moveValid(int x, int y)
{
	return Rook::moveValid(x, y) || Bishop::moveValid(x, y);
}
//Queen::Queen(int x, int y, bool isWhite) : Piece(x, y,'q',isWhite){}