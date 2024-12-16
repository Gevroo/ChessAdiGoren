#pragma once
#include "Piece.h"


class King : public Piece
{
public:
	King(int x, int y,bool isWhite,char type);
	bool moveValid(int x, int y);
private:
	bool moved;//for castling which i prob wont even try

};