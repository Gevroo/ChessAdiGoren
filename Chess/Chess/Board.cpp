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

	this->_whitePlayer = new Player(7,4,true,-1);
	this->_blackPlayer = new Player(0,4,false,1);
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
			if (buildBoardCommand[i * BOARD_SIZE + j] == 'r') this->_pieces[i][j] = new Rook(i, j,  false);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'R') this->_pieces[i][j] = new Rook(i, j,  true);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'b') this->_pieces[i][j] = new Bishop(i, j, false);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'B') this->_pieces[i][j] = new Bishop(i, j, true);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'n') this->_pieces[i][j] = new Knight(i, j, false);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'N') this->_pieces[i][j] = new Knight(i, j, true);
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'p') this->_pieces[i][j] = new Pawn(i, j, false,_blackPlayer->getDirection());
			else if (buildBoardCommand[i * BOARD_SIZE + j] == 'P') this->_pieces[i][j] = new Pawn(i, j, true,_whitePlayer->getDirection());
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
	std::cout << "values[0]: " << values[0] << ", values[1]: " << values[1]
		<< ", values[2]: " << values[2] << ", values[3]: " << values[3] << std::endl;

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
	else if (!_pieces[values[0]][values[1]]->moveValid(values[2], values[3]) || !pathIsClear(values[0], values[1], values[2], values[3]))
		result = 6;
	if (result == 0 || result == 1 || result == 8)
	{
		this->movePiece(values[0],values[1],  values[2], values[3]);
		this->_turn = !_turn;
	}
	return result;
}
void Board::movePiece(int nowX, int nowY, int toX, int toY)
{
	Piece** pointer = &_pieces[nowX][nowY];
	this->_pieces[nowX][nowY]->changeCoords(toX,toY);
	this->_pieces[toX][toY] = *pointer;
	this->_pieces[nowX][nowY] = nullptr;
	//std::cout << _pieces[toX][toY]->getX() << _pieces[toX][toY]->getY();
	//if(_pieces[nowX][nowY] != nullptr) this->_pieces[nowX][nowY]->changeCoords(toX,toY);
}
bool Board::checkOnTheKing(bool whiteOrBlack)
{
	return true;
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
void Board::removePiece(int x, int y)
{
	_pieces[x][y] = nullptr;
}