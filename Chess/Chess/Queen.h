#pragma once
#include "Rook.h"
#include "Bishop.h"

class Queen : public Rook, public Bishop
{
public:
	bool moveValid(int x, int y);
	//Queen(int x, int y, bool isWhite);
};


