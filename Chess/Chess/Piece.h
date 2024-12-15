#pragma once
#include <string>

class Piece
{
public:
	Piece(int x, int y, std::string type, bool isWhite);
	int getX()const;
	int getY() const;
	std::string getType() const;
	bool whiteOrNot() const;
	void changeCoords(int newX, int newY);
	virtual bool moveValid(int x,int y) = 0;
protected:
	int _x;
	int _y;
	std::string _type;
	bool _isWhite;
};