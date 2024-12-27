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
	~Board();
	int moveValid(std::string command);
	void swapPiece(int nowX, int nowY, int toX, int toY);
	bool checkOnTheKing(Player* player);
	std::string craftBoard() const;
	bool inBounds(int x,int y);
	Piece* removePiece(int x,int y);
	bool trickPawn(int nowX, int nowY, int toX, int toY);
	bool StaleMate(Player* player);
	bool Transformation(int x,int y);
	bool emptyBoard(const bool whiteOrNot) const;
	bool sameColor(Piece* one, Piece* two) const;
	bool pieceCanPreventMate(Player* player, int x, int y);
private:
	Piece* _pieces[BOARD_SIZE][BOARD_SIZE];
	Player* _whitePlayer;
	Player* _blackPlayer;
	Piece* _enPassent;
	bool _turn;//white = true
};