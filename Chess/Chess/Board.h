#pragma once

#include "Player.h"
#include "Queen.h"
#include "Pawn.h"
#include "King.h"
#include "Knight.h"
//With queen i get all rook bishop and piece
#define BOARD_SIZE 8
class Board
{
public:
	bool pathIsClear(int nowX, int nowY, int toX, int toY);
	Board(std::string buildBoardCommand);
	int moveValid(std::string command);
	void movePiece(int nowX, int nowY, int toX, int toY);
	bool checkOnTheKing(Player* player);
	std::string craftBoard();
	bool inBounds(int x,int y);
	Piece* removePiece(int x,int y);
	bool pieceExist(int x, int y, bool expectedColor, bool exist)const;
	bool trickPawn(int nowX, int nowY, int toX, int toY);
	bool StaleMate(Player* player);
	void Transformation(int x,int y);
private:
	Piece* _pieces[BOARD_SIZE][BOARD_SIZE];
	Player* _whitePlayer;
	Player* _blackPlayer;
	Piece* _enPassent;
	bool _turn;//white = true
};