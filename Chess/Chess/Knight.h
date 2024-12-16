#pragma once
#include "Piece.h"


class Knight : public Piece
{
public:
	bool moveValid(int x, int y);
	Knight(int x, int y, bool isWhite);
};
