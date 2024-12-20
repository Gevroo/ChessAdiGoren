#include "Board.h"
#include <iostream>

bool Board::pathIsClear(int nowX,int nowY,int toX,int toY)
{
	if (_pieces[nowX][nowY]->getType() == 'n') return true;//Because knight can jump high
	int slopeX = toX - nowX;
	int slopeY = toY - nowY;

	int stepX = !slopeX ? 0 : slopeX > 0 ? 1 : -1;
	int stepY = !slopeY ? 0 : slopeY > 0 ? 1 : -1;

	int currentX = stepX + nowX;
	int currentY = stepY + nowY;//Doing first time
	while (currentX != toX || currentY != toY)
	{
		
		if (_pieces[currentX][currentY] != nullptr) return false;
		currentX += stepX;
		currentY += stepY;
		
	}
	return true;
}
Board::Board(std::string buildBoardCommand)
{

	this->_whitePlayer = new Player(true,-1);
	this->_blackPlayer = new Player(false,1);
	this->_turn = buildBoardCommand[64] == '0';
	//_pieces[0][0] = new Rook(0, 0, true,"r");
	//this->
	//_pieces = {
	//	
	//}
	for (int i = 0;i < BOARD_SIZE;i++)
	{
		for (int j = 0 ;j < BOARD_SIZE;j++)
		{
			if (buildBoardCommand[i * BOARD_SIZE + j] == 'r') this->_pieces[i][j] = new Rook(i, j, false);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'R') this->_pieces[i][j] = new Rook(i, j, true);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'b') this->_pieces[i][j] = new Bishop(i, j, false);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'B') this->_pieces[i][j] = new Bishop(i, j, true);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'n') this->_pieces[i][j] = new Knight(i, j, false);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'N') this->_pieces[i][j] = new Knight(i, j, true);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'q') this->_pieces[i][j] = new Queen(i, j, false);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'Q') this->_pieces[i][j] = new Queen(i, j, true);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'p') this->_pieces[i][j] = new Pawn(i, j, false, _blackPlayer->getDirection());
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'P') this->_pieces[i][j] = new Pawn(i, j, true, _whitePlayer->getDirection());
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'k')
			{
				_blackPlayer->setKing(new King(i, j, false));
				this->_pieces[i][j] = _blackPlayer->getKing();
			}
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'K') 
			{
				_whitePlayer->setKing(new King(i, j, true));
				this->_pieces[i][j] = _whitePlayer->getKing();
			}
			else this->_pieces[i][j] = nullptr;
		}
	}
}
/*bool Board::pieceExist(int x, int y, bool expectedColor, bool exist) const
{
	return (exist && this->_pieces[x][y] != nullptr) && (!exist && this->_pieces[x][y] == nullptr) && this->_pieces[x][y]->whiteOrNot() == expectedColor;
}*/
int Board::moveValid(std::string command)
{
	int result = 0;
	int vals[] = {  8-static_cast<int>(command[1]-'0'),static_cast<int>(command[0] - 'a'), 8 - static_cast<int>(command[3]-'0'),static_cast<int>(command[2] - 'a') };


	if (!inBounds(vals[0], vals[1]) || !inBounds(vals[2], vals[3]))
		return 5;
	else if (_pieces[vals[0]][vals[1]] == nullptr || _pieces[vals[0]][vals[1]]->whiteOrNot() != _turn)
		return 2;
	else if (vals[0] == vals[2] && vals[1] == vals[3])
		return 7;
	//else if (pieceExist(vals[0], vals[1], _turn, false))
	//		result = 2;
	else if(_pieces[vals[2]][vals[3]] != nullptr && _pieces[vals[2]][vals[3]]->whiteOrNot() == _turn)
		return 3;
	else if ((_pieces[vals[2]][vals[3]] != nullptr && vals[1] == vals[3] && _pieces[vals[0]][vals[1]]->getType() == 'p') || (!trickPawn(vals[0], vals[1], vals[2], vals[3]) && (!_pieces[vals[0]][vals[1]]->moveValid(vals[2], vals[3]) || !pathIsClear(vals[0], vals[1], vals[2], vals[3]))))
		return 6;
	//this if is making my code feel stupid
	//Explanation: it will check if theres a piece infront of the pawn and if yes it will give an error, it will check if a pawn can eat,and lastly it will check manualy for all of the other pieces
	_enPassent = nullptr;
	if (result == 0)
	{
		if (_pieces[vals[0]][vals[1]]->getType() == 'p' && abs(vals[0] - vals[2]) == 2)
			_enPassent = _pieces[vals[0]][vals[1]];



		this->swapPiece(vals[0],vals[1],  vals[2], vals[3]);
		Piece* rem = removePiece(vals[0], vals[1]);//i switched between them so now the unwanted one is the frst

		if (_turn ? checkOnTheKing(_blackPlayer) : checkOnTheKing(_whitePlayer))//Checking if you checked the other king
		{
			result = 1;//Check
			if (StaleMate(_turn ? _blackPlayer : _whitePlayer))//Both check and stalemate = checkmate
				result = 8;//Chess
			Transformation(vals[2], vals[3]);
		}
		if (!_turn ? checkOnTheKing(_blackPlayer) : checkOnTheKing(_whitePlayer))//Checking if you checked the other king
		{
			
			result = 4;	
			_pieces[vals[0]][vals[1]] = rem;
			this->swapPiece(vals[0], vals[1], vals[2], vals[3]);

		}
		else
		{
			Transformation(vals[2], vals[3]);//it will check transformation both if you checked or not, just not when you are threathend a check
		}
		//if (emptyBoard() && StaleMate(_turn ? _blackPlayer : _whitePlayer))//Both check and stalemate = checkmate
		//	result = 8;

		if(result == 0 || result == 1 || result == 8)//only moves to make move work
			this->_turn = !_turn;
	}
	return result;
}
void Board::swapPiece(int nowX, int nowY, int toX, int toY)
{

	Piece* pointerNow = _pieces[nowX][nowY];
	Piece* pointerTo = _pieces[toX][toY];
	if(this->_pieces[nowX][nowY] != nullptr)this->_pieces[nowX][nowY]->changeCoords(toX,toY);
	if(this->_pieces[toX][toY] != nullptr)this->_pieces[toX][toY]->changeCoords(nowX,nowY);
	this->_pieces[toX][toY] = pointerNow;
	this->_pieces[nowX][nowY] = pointerTo;
}
bool Board::checkOnTheKing(Player* player)
{
	Piece* king = player->getKing();
	int kingY = king->getY();
	int kingX = king->getX();
	int dir = player->getDirection();
	Piece* pawn;
	for (int i = 0;i < BOARD_SIZE;i++)
	{
		for (int j = 0;j < BOARD_SIZE;j++)
		{
			if (_pieces[i][j] != nullptr && _pieces[i][j]->getType() != 'p' && !sameColor(_pieces[i][j],king) && _pieces[i][j]->moveValid(king->getX(), king->getY()) && pathIsClear(i, j,king->getX(), king->getY()))
			{
				std::cout << _pieces[i][j]->getType();
				return true;
			}
		}
	}
	if (inBounds(kingX + dir, kingY - 1))
	{
		pawn = _pieces[kingX + dir][kingY - 1];//left upper pawn(theortically)
		if (pawn != nullptr && pawn->getType() == 'p' && !sameColor(pawn, king))
			return true;
	}
	if (inBounds(kingX + dir, kingY + 1))
	{
		pawn = _pieces[kingX + dir][kingY + 1];//right upper pawn(theortically)
		if (pawn != nullptr && pawn->getType() == 'p' && !sameColor(pawn, king))
			return true;
	}
	/*for (int i = king->getX() + 1;i < BOARD_SIZE;i++)
	{
		if (_pieces[i][king->getY()] == nullptr) continue;
		if (pathIsClear(king->getX(), king->getY(), i, king->getY()) && _pieces[i][king->getY()]->moveValid(king->getX(), king->getY()))
			return true;
	}
	for (int i = king->getX() - 1;i > 0;i--)
	{
		if (_pieces[i][king->getY()] == nullptr) continue;
		if (pathIsClear(king->getX(), king->getY(), i, king->getY()) && _pieces[i][king->getY()]->moveValid(king->getX(), king->getY()))
			return true;
	}*/
	return false;
}
std::string Board::craftBoard()
{
	std::string output = "";
	for (int i = 0;i < BOARD_SIZE;i++)
	{
		for (int j = 0;j < BOARD_SIZE;j++)
		{
			char key = '#';
			if(this->_pieces[i][j] != nullptr)
				key = _pieces[i][j]->whiteOrNot() ? static_cast<char>(static_cast<int>(_pieces[i][j]->getType()) - 32): _pieces[i][j]->getType();
			output += key;

		}
		output += '\n';
	}
	return output;
}
bool Board::inBounds(int x, int y)
{
	return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}
Piece* Board::removePiece(int x, int y)
{
	Piece* rtnPiece = _pieces[x][y];
	_pieces[x][y] = nullptr;
	return rtnPiece;
}
//In this function Im checking if the pawn can eat smth,if yes i will trick the pawn class to think that the move is possible
bool Board::trickPawn(int nowX, int nowY, int toX, int toY)
{
	if (_pieces[nowX][nowY]->getType() != 'p') return false;
	//Checks for en passent
	int direction = _pieces[nowX][nowY]->whiteOrNot() ? _whitePlayer->getDirection() : _blackPlayer->getDirection();
	if (_enPassent != nullptr && abs(toY - nowY) == 1 && nowX+direction == toX && _enPassent == _pieces[toX - direction][toY])
	{
		removePiece(toX - direction,toY);
		return true;
	}
	//no en passent
	if (_pieces[toX][toY] == nullptr) return false;
	
	if (nowX + direction == toX && abs(toY - nowY) == 1)
	{
		_pieces[nowX][nowY]->moveValid(nowX + direction, nowY);//To update move
		return true;
	}
	
	return false;
}
bool Board::StaleMate(Player* player)//if its false so stalemate
{
	int x = player->getKing()->getX();
	int y = player->getKing()->getY();
	for (int i = -1;i <= 1;i++)
	{
		for (int j = -1;j <= 1;j++)
		{
			if (j == i) continue;
			if (!inBounds(x + i, y + j)) continue;
			if (_pieces[x + i][y + j] != nullptr && player->getKing() != nullptr && sameColor(player->getKing(),_pieces[x+i][y+j])) continue;
			this->swapPiece(x, y, x+i, y+j);
			Piece* rem = removePiece(x, y);
			bool result = checkOnTheKing(player);

			_pieces[x][y] = rem;
			this->swapPiece(x, y, x + i, y + j);
			if (!result) return false;

		}
	}
	return true;
}
void Board::Transformation(int x,int y)
{
	//theres a piece there 100%
	bool isWhite = _pieces[x][y]->whiteOrNot();
	int dir = isWhite ? _whitePlayer->getDirection() : _blackPlayer->getDirection();
	std::cout << "Im still standing";
	if (!inBounds(x + dir, y))
	{
		std::cout << "Im still standing";
		//MAKE OVER TIME
		removePiece(x + dir, y);
		_pieces[x + dir][y] = new Queen(x,y,isWhite);
	}
}
bool Board::emptyBoard()
{
	//if the board is empty from pieces and theres only the king
	for (int i = 0;i < BOARD_SIZE;i++)
	{
		for (int j = 0;j < BOARD_SIZE;j++)
		{
			if (_pieces[i][j]->getType() != 'p') return false;
		}
	}
	return true;
}
bool Board::sameColor(Piece* one, Piece* two)
{
	//If theres no color then it cant be equal to smth
	return one != nullptr && two != nullptr && one->whiteOrNot() == two->whiteOrNot();
}