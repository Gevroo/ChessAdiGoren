#pragma once
#include <string>
#include "Piece.h"

class Rook : virtual public Piece
{
public:
	bool moveValid(int x, int y) ;
	Rook(int x, int y, bool isWhite);
	Rook();
};