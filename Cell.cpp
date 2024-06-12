#include "Cell.h"

Cell::Cell(u_int x, u_int y, bool m) {
	this->x = x;
	this->y = y;
	this->isMine = m;
}

Cell::Cell() {

}

Cell::~Cell() {

}