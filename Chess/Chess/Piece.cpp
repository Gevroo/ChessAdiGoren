#include "Piece.h"
#include <iostream>

int Piece::getX() const
{
	return this->_x;
}
int Piece::getY() const
{
	return this->_y;
}
char Piece::getType() const
{
	return this->_type;
}
bool Piece::whiteOrNot() const
{
	return this->_isWhite;
}
void Piece::changeCoords(int newX, int newY)
{
	this->_x = newX;
	this->_y = newY;
}
Piece::Piece(int x, int y, char type, bool isWhite)
{
	this->_x = x;
	this->_y = y;
	this->_type = type;
	this->_isWhite = isWhite;
}