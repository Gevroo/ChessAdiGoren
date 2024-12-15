#pragma once
#include "Piece.h"


class Bishop : public Piece
{
public:
	bool moveValid(int x, int y);

};