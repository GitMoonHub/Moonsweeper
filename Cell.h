#ifndef CELL_H
#define CELL_H
#pragma once
#include <winsock.h>

class Cell
{
public:
	u_int adjacentMines = 0;
	u_int x = 0;
	u_int y = 0;
	bool isFlagged = false;
	bool isMine = false;
	bool hasExploded = false;
	bool isDiscovered = false;
	bool isPushed = false;

	Cell(u_int x, u_int y, bool m);
	Cell();
	~Cell();

private:

};

#endif