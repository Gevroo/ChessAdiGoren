#pragma once
#include <string>
#include "Piece.h"

class Rook : public Piece
{
public:
	bool moveValid(int x, int y) ;
	//Rook(int x, int y, bool isWhite);

};