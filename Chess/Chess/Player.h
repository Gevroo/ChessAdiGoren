#pragma once

#include "King.h"
class Player
{
public:
	Player(bool isWhite, int direction);
	int getDirection() const;
	Piece* getKing() const;
	void setKing(Piece* all_hail_the_king);
private:
	Piece* _king;
	bool _white;
	int _direction;
};