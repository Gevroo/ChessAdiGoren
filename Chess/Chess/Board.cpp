#include "Board.h"


bool Board::pathIsClear()
{

}
Board::Board()
{
	//this->_player1 = new Player(7,4,true,1);
	//this->_player1 = new Player(0,4,false,-1);
}
bool Board::pieceExist(int x,int y,bool expectedColor) const
{
	return this->_pieces[x][y] != nullptr && this->_pieces[x][y]->whiteOrNot() == expectedColor;
}
void Board::moveValid(std::string command)
{
	int result = 0;
	int values[] = { static_cast<int>(command[0] - 'a'), static_cast<int>(command[1]), static_cast<int>(command[2] - 'a'),static_cast<int>(command[3])};
	if (values[0] == values[2] && values[1] == values[3])
		result = 7;
	else if (inBounds(values[2], values[3]))
		result = 5;
	else if (pieceExist(values[0], values[1], turn))
		result = 2;
	else if (pieceExist(values[2], values[3], !turn))
		result = 3;
	else if (!_pieces[values[0]][values[1]]->moveValid(values[2], values[3]))
		result = 6;

}
void Board::movePiece(int nowX, int nowY, int toX, int toY)
{
	this->_pieces[toX][toY] = this->_pieces[nowX][nowY];
	
	//this->_pieces[nowX][nowY] = nullptr;
}
bool Board::checkOnTheKing(bool whiteOrBlack)
{
	
}
std::string Board::craftBoard()
{
	std::string output = "";
	for (int i = 0;i < BOARD_SIZE;i++)
	{
		for (int j = 0;j < BOARD_SIZE;j++)
		{
			output += (this->_pieces[i][j] == NULL)? "#" : _pieces[i][j]->getType();
		}
		//output += "\n"; not sure if needed
	}
	return output;
}
bool Board::inBounds(int x, int y)
{
	return x > 0 && x < 8 && y > 0 && y < 8;
}