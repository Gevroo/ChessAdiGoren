#pragma once
#include "Piece.h"


class Bishop : public Piece
{
public:
	bool moveValid(int x, int y);
	Bishop(int x, int y, bool isWhite);

};