#pragma once

#include "King.h"
class Player
{
public:
	Player(int x, int y, bool isWhite, int direction);
	int getDirection() const;
private:
	Piece* _king;
	bool _white;
	int _direction;
};