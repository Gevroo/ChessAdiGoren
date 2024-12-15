#pragma once
#include "Piece.h"
#include "Player.h"

#define BOARD_SIZE 8
class Board
{
	bool pathIsClear();
	Board();
	void moveValid(std::string command);
	void movePiece(int nowX, int nowY, int toX, int toY);
	bool checkOnTheKing(bool whiteOrBlack);
	std::string craftBoard();
	bool inBounds(int x,int y);
	bool pieceExist(int x, int y,bool expectedColor)const;

private:
	Piece* _pieces[BOARD_SIZE][BOARD_SIZE];
	Player _player1;
	Player _player2;
	bool turn;//white = falseLOL
};