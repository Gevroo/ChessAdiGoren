#pragma once
#include "Piece.h"

class Pawn : public Piece
{
public:
	Pawn(int x, int y, bool isWhite,int direction);
	bool moveValid(int x, int y);
	bool moveValid(bool pieceThere,int x, int y);

private:
	bool _moved;
	int _direction;//-1 or 1
};