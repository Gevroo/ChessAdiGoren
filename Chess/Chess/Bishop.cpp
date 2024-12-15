#include "Bishop.h"

bool Bishop::moveValid(int x, int y)
{
	return this->_x - x == this->_y;//Checking if its in the same diagonal line 
}