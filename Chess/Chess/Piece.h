#pragma once
#include <string>

class Piece
{
public:
	Piece(int x, int y, char type, bool isWhite);
	Piece();
	int getX()const;
	int getY() const;
	char getType() const;
	bool whiteOrNot() const;
	void changeCoords(int newX, int newY);
	virtual bool moveValid(int x,int y) = 0;
protected:
	int _x;
	int _y;
	char _type;
	bool _isWhite;
};