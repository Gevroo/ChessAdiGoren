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
bool Board::pieceExist(int x,int y,bool expectedColor,bool exist) const
{
	return (exist && this->_pieces[x][y] != nullptr) && (!exist && this->_pieces[x][y] == nullptr) && this->_pieces[x][y]->whiteOrNot() == expectedColor;
}
int Board::moveValid(std::string command)
{
	int result = 0;
	int values[] = {  8-static_cast<int>(command[1]-'0'),static_cast<int>(command[0] - 'a'), 8 - static_cast<int>(command[3]-'0'),static_cast<int>(command[2] - 'a') };

	//if(_pieces[values[0]][values[1]] != nullptr)
	//std::cout << _pieces[values[0]][values[1]]->whiteOrNot() << std::endl;
	if (!inBounds(values[0], values[1]) || !inBounds(values[2], values[3]))
		result = 5;
	else if (_pieces[values[0]][values[1]] == nullptr || _pieces[values[0]][values[1]]->whiteOrNot() != _turn)
		result = 2;
	else if (values[0] == values[2] && values[1] == values[3])
		result = 7;
	//else if (pieceExist(values[0], values[1], _turn, false))
	//		result = 2;
	else if(_pieces[values[2]][values[3]] != nullptr && _pieces[values[2]][values[3]]->whiteOrNot() == _turn)
		result = 3;
	else if ((_pieces[values[2]][values[3]] != nullptr && values[1] == values[3] && _pieces[values[0]][values[1]]->getType() == 'p') || (!trickPawn(values[0], values[1], values[2], values[3]) && (!_pieces[values[0]][values[1]]->moveValid(values[2], values[3]) || !pathIsClear(values[0], values[1], values[2], values[3]))))
		result = 6;
	//this if is making my code feel stupid
	//Explanation: it will check if theres a piece infront of the pawn and if yes it will give an error, it will check if a pawn can eat,and lastly it will check manualy for all of the other pieces
	_enPassent = nullptr;
	if (result == 0 || result == 8)
	{
		if (_pieces[values[0]][values[1]]->getType() == 'p' && abs(values[0] - values[2]) == 2)
		{
			_enPassent = _pieces[values[0]][values[1]];
			std::cout << "en passent avaialiabe";
		}
		this->movePiece(values[0],values[1],  values[2], values[3]);
		Piece* rem = removePiece(values[0], values[1]);//i switched between them so now the unwanted one is the frst
		if (_turn ? checkOnTheKing(_blackPlayer) : checkOnTheKing(_whitePlayer))//Checking if you checked the other king
			result = 1;
		if (!_turn ? checkOnTheKing(_blackPlayer) : checkOnTheKing(_whitePlayer))//Checking if you checked the other king
		{
			result = 4;	
			_pieces[values[0]][values[1]] = rem;
			this->movePiece(values[0], values[1], values[2], values[3]);

		}
		if(result == 0 || result == 1)
			this->_turn = !_turn;
	}
	return result;
}
void Board::movePiece(int nowX, int nowY, int toX, int toY)
{
	//if (_pieces[nowX][nowY]->getType() == 'k' || _pieces[nowX][nowY]->getType() == 'p')
	//	std::cout << "how";
	Piece* pointerNow = _pieces[nowX][nowY];
	Piece* pointerTo = _pieces[toX][toY];
	if(this->_pieces[nowX][nowY] != nullptr)this->_pieces[nowX][nowY]->changeCoords(toX,toY);
	if(this->_pieces[toX][toY] != nullptr)this->_pieces[toX][toY]->changeCoords(nowX,nowY);
	this->_pieces[toX][toY] = pointerNow;
	this->_pieces[nowX][nowY] = pointerTo;
	//std::cout << _pieces[toX][toY]->getX() << _pieces[toX][toY]->getY();
	//if(_pieces[nowX][nowY] != nullptr) this->_pieces[nowX][nowY]->changeCoords(toX,toY);
}
bool Board::checkOnTheKing(Player* player)
{
	Piece* thisKing = player->getKing();
	
	for (int i = 0;i < BOARD_SIZE;i++)
	{
		for (int j = 0;j < BOARD_SIZE;j++)
		{
			if (_pieces[i][j] != nullptr && _pieces[i][j]->getType() != 'p' && _pieces[i][j]->whiteOrNot() != thisKing->whiteOrNot() && _pieces[i][j]->moveValid(thisKing->getX(), thisKing->getY()) && pathIsClear(i, j,thisKing->getX(), thisKing->getY()))
			{
				std::cout << _pieces[i][j]->getType();
				return true;
			}
		}
	}
	if ((inBounds(thisKing->getX() + player->getDirection(), thisKing->getY() - 1) && _pieces[thisKing->getX() + player->getDirection()][thisKing->getY() - 1] != nullptr && _pieces[thisKing->getX() + player->getDirection()][thisKing->getY() - 1]->getType() == 'p' && _pieces[thisKing->getX() + player->getDirection()][thisKing->getY() - 1]->whiteOrNot() != thisKing->whiteOrNot()) || (inBounds(thisKing->getX() + player->getDirection(), thisKing->getY() + 1) && _pieces[thisKing->getX() + player->getDirection()][thisKing->getY() + 1] != nullptr && _pieces[thisKing->getX() + player->getDirection()][thisKing->getY() + 1]->getType() == 'p' && _pieces[thisKing->getX() + player->getDirection()][thisKing->getY() + 1]->whiteOrNot() != thisKing->whiteOrNot()))
		return true;
		/*for (int i = thisKing->getX() + 1;i < BOARD_SIZE;i++)
	{
		if (_pieces[i][thisKing->getY()] == nullptr) continue;
		if (pathIsClear(thisKing->getX(), thisKing->getY(), i, thisKing->getY()) && _pieces[i][thisKing->getY()]->moveValid(thisKing->getX(), thisKing->getY()))
			return true;
	}
	for (int i = thisKing->getX() - 1;i > 0;i--)
	{
		if (_pieces[i][thisKing->getY()] == nullptr) continue;
		if (pathIsClear(thisKing->getX(), thisKing->getY(), i, thisKing->getY()) && _pieces[i][thisKing->getY()]->moveValid(thisKing->getX(), thisKing->getY()))
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
				key = _pieces[i][j]->whiteOrNot() == true ? static_cast<char>(static_cast<int>(_pieces[i][j]->getType()) - 32): _pieces[i][j]->getType();
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