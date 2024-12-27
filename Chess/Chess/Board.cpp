#include "Board.h"
#include <iostream>

#define VALID_MOVE 0
#define CHECK 1
#define NO_PIECE 2
#define PIECE_EXIST 3
#define SELF_CHECK 4
//#define OUT_OF_BOUNDS 5//shouldnt be worried in frontend
#define STALEMATE 5
#define MOVE_CANT_MOVE_LIKE_THAT 6
//#define SAME_THING 7 shoudl'tn be worried about in frontend
#define EN_PASSENT 7
#define CHECKMATE 8
#define PROMOTION 9

#define white true
#define black false


bool Board::pathIsClear(int nowX,int nowY,int toX,int toY)
{
	if (_pieces[nowX][nowY]->getType() == 'n') return true;//Because knight can jump high
	int slopeX = toX - nowX;
	int slopeY = toY - nowY;

	int stepX = !slopeX ? 0 : slopeX > 0 ? 1 : -1;//checeking which way it should go relative to the positions
	int stepY = !slopeY ? 0 : slopeY > 0 ? 1 : -1;

	int currentX = stepX + nowX;	
	int currentY = stepY + nowY;//Doing first time
	while (currentX != toX || currentY != toY)//going through every possible square in the way and checking if theres a piece in there
	{
		
		if (_pieces[currentX][currentY] != nullptr) return false;
		currentX += stepX;
		currentY += stepY;
		
	}
	return true;
}
Board::Board(std::string buildBoardCommand)
{

	this->_whitePlayer = new Player(white,-1);
	this->_blackPlayer = new Player(black,1);
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
			if (buildBoardCommand[i * BOARD_SIZE + j] == 'r') this->_pieces[i][j] = new Rook(i, j, black);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'R') this->_pieces[i][j] = new Rook(i, j, white);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'b') this->_pieces[i][j] = new Bishop(i, j, black);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'B') this->_pieces[i][j] = new Bishop(i, j, white);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'n') this->_pieces[i][j] = new Knight(i, j, black);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'N') this->_pieces[i][j] = new Knight(i, j, white);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'q') this->_pieces[i][j] = new Queen(i, j, black);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'Q') this->_pieces[i][j] = new Queen(i, j, white);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'p') this->_pieces[i][j] = new Pawn(i, j, black, _blackPlayer->getDirection());
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'P') this->_pieces[i][j] = new Pawn(i, j, white, _whitePlayer->getDirection());
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'k')
			{
				_blackPlayer->setKing(new King(i, j, black));
				this->_pieces[i][j] = _blackPlayer->getKing();
			}
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'K') 
			{
				_whitePlayer->setKing(new King(i, j, white));
				this->_pieces[i][j] = _whitePlayer->getKing();
			}
			else this->_pieces[i][j] = nullptr;
		}
	}
}
Board::~Board()
{
	//Getting rid of the pointers
	delete this->_blackPlayer->getKing();
	delete this->_whitePlayer->getKing();
	delete this->_blackPlayer;
	delete this->_whitePlayer;
	for (int i = 0;i < BOARD_SIZE;i++)
	{
		for (int j = 0;j < BOARD_SIZE;j++)
		{
			delete _pieces[i][j];
		}
	}
	delete _pieces;
}
/*bool Board::pieceExist(int x, int y, bool expectedColor, bool exist) const
{
	return (exist && this->_pieces[x][y] != nullptr) && (!exist && this->_pieces[x][y] == nullptr) && this->_pieces[x][y]->whiteOrNot() == expectedColor;
}*/
int Board::moveValid(std::string command)
{
	int result = 0;
	int vals[] = {  8-static_cast<int>(command[1]-'0'),static_cast<int>(command[0] - 'a'), 8 - static_cast<int>(command[3]-'0'),static_cast<int>(command[2] - 'a') };
	//Translating the chess square index into a 2d array index
	//which the start is at the top left and is going right each row

	//if (!inBounds(vals[0], vals[1]) || !inBounds(vals[2], vals[3]))
	//	return OUT_OF_BOUNDS;//shoudlnt be worried in frontend
	/*else */ if (_pieces[vals[0]][vals[1]] == nullptr || _pieces[vals[0]][vals[1]]->whiteOrNot() != _turn)
		return NO_PIECE;
	//else if (vals[0] == vals[2] && vals[1] == vals[3])
	//	return SAME_THING;
	//else if (pieceExist(vals[0], vals[1], _turn, false))
	//		result = 2;
	else if(_pieces[vals[2]][vals[3]] != nullptr && _pieces[vals[2]][vals[3]]->whiteOrNot() == _turn)
		return PIECE_EXIST;
	else if ((_pieces[vals[2]][vals[3]] != nullptr && vals[1] == vals[3] && _pieces[vals[0]][vals[1]]->getType() == 'p') || (!trickPawn(vals[0], vals[1], vals[2], vals[3]) && (!_pieces[vals[0]][vals[1]]->moveValid(vals[2], vals[3]) || !pathIsClear(vals[0], vals[1], vals[2], vals[3]))))
		return MOVE_CANT_MOVE_LIKE_THAT;
	if (vals[1] != vals[3] && _pieces[vals[0]][vals[1]]->getType() == 'p' && _pieces[vals[2]][vals[3]] == nullptr)//meant that the moves worked
		result = EN_PASSENT;//en passent
	//this if is making my code feel stupid
	//Explanation: it will check if theres a piece infront of the pawn and if yes it will give an error, it will check if a pawn can eat,and lastly it will check manualy for all of the other pieces
	_enPassent = nullptr;
	if (result == VALID_MOVE || result == EN_PASSENT)
	{
		if (_pieces[vals[0]][vals[1]]->getType() == 'p' && abs(vals[0] - vals[2]) == 2)
			_enPassent = _pieces[vals[0]][vals[1]];



		this->swapPiece(vals[0],vals[1],  vals[2], vals[3]);
		Piece* rem = removePiece(vals[0], vals[1]);//i switched between them so now the unwanted one is the frst
		if (!(!_turn ? checkOnTheKing(_blackPlayer) : checkOnTheKing(_whitePlayer)) && Transformation(vals[2], vals[3]))
			result = PROMOTION;

		if (_turn ? checkOnTheKing(_blackPlayer) : checkOnTheKing(_whitePlayer))//Checking if you checked the other king
		{
			result = CHECK;//Check
			if (StaleMate(_turn ? _blackPlayer : _whitePlayer))//Both check and stalemate = checkmate
				result = CHECKMATE;//Chess

		}
		if (!_turn ? checkOnTheKing(_blackPlayer) : checkOnTheKing(_whitePlayer))//Checking if you checked the other king
		{
			
			result = SELF_CHECK;	
			_pieces[vals[0]][vals[1]] = rem;
			this->swapPiece(vals[0], vals[1], vals[2], vals[3]);

		}
		else
		{
			if (Transformation(vals[2], vals[3]))
				result = PROMOTION;
		}
		//if (emptyBoard(_turn) && StaleMate(_turn ? _blackPlayer : _whitePlayer))//Both check and stalemate = checkmate
		//	result = STALEMATE;
		if (result == EN_PASSENT) this->_turn = !_turn;
		if(result == VALID_MOVE || result == STALEMATE || result == CHECK || result == CHECKMATE || result == PROMOTION)//only moves to make move work
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
			{//Checking if theres a piece that can go to the king's square not including pawns
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
	return false;
}
std::string Board::craftBoard() const
{
	std::string output = "";
	for (int i = 0;i < BOARD_SIZE;i++)
	{
		for (int j = 0;j < BOARD_SIZE;j++)
		{
			char key = '#';
			if(this->_pieces[i][j] != nullptr)
				key = _pieces[i][j]->whiteOrNot() ? static_cast<char>(static_cast<int>(_pieces[i][j]->getType()) - 32): _pieces[i][j]->getType();
			output += key;//going through each square and adding wheter theres a piece

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
	_pieces[x][y] = nullptr;//like a pop in a stack
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
			if (_pieces[x + i][y + j] != nullptr && player->getKing() != nullptr && sameColor(player->getKing(), _pieces[x + i][y + j])) continue;
			this->swapPiece(x, y, x + i, y + j);
			Piece* rem = removePiece(x, y);
			bool result = checkOnTheKing(player);

			_pieces[x][y] = rem;
			this->swapPiece(x, y, x + i, y + j);
			if (!result) return false;

		}
	}

	for (int i = 0;i < BOARD_SIZE;i++)
	{
		for (int j = 0;j < BOARD_SIZE;j++)
		{
			if (_pieces[i][j] != nullptr && sameColor(player->getKing(), _pieces[i][j]))
			{
				if (pieceCanPreventMate(player, i, j))
					return false;
			}
		}
	}
	return true;
}
bool Board::pieceCanPreventMate(Player* player,int x,int y)
{
	for (int i = 0;i < BOARD_SIZE;i++)
	{
		for (int j = 0;j < BOARD_SIZE;j++)
		{
			if (!inBounds( i, j)) continue;
			if (!(_pieces[x][y]->moveValid( i,j) && pathIsClear(x, y,  i,  j)))continue;
			if (_pieces[i][j] != nullptr && sameColor(_pieces[x][y], _pieces[i][j])) continue;
			this->swapPiece(x, y,i,j);
			Piece* rem = removePiece(x, y);
			bool result = checkOnTheKing(player);

			_pieces[x][y] = rem;
			this->swapPiece(x, y, i,j);
			if (!result) return true;
		}
	}
	return false;
}
bool Board::Transformation(int x,int y)//If a pawn's next move will be out of the board it means that he reached the end
{
	//theres a piece there 100%
	if (_pieces[x][y]->getType() != 'p') return false;	
	bool isWhite = _pieces[x][y]->whiteOrNot();
	int dir = isWhite ? _whitePlayer->getDirection() : _blackPlayer->getDirection();
	std::cout << "Im still standing";
	if (!inBounds(x + dir, y))
	{
		//MAKE OVER TIME
		removePiece(x + dir, y);
		//_pieces[x][y] = nullptr;
		_pieces[x][y] = new Queen(x,y,isWhite);
		return true;
	}
	return false;
}
bool Board::emptyBoard(const bool whiteOrNot) const
{
	//if the board is empty from pieces and theres only the king
	for (int i = 0;i < BOARD_SIZE;i++)
	{
		for (int j = 0;j < BOARD_SIZE;j++)
		{
			if (_pieces[i][j]->getType() != 'k' && _pieces[i][j]->whiteOrNot() != whiteOrNot) return false;
		}
	}
	return true;
}


bool Board::sameColor(Piece* one, Piece* two) const
{
	//If theres no color then it cant be equal to smth
	return one != nullptr && two != nullptr && one->whiteOrNot() == two->whiteOrNot();
}