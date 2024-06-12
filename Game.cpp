#include <iostream>
#include <random>
#include "Game.h"

Game::Game(DIFFICULTY dif, u_int w, u_int h, u_int m, u_int seed) {
	
	switch (dif) {
	case DIFFICULTY::BEGINNER:
		this->height = 9;
		this->width = 9;
		this->flagsLeft = 10;
		this->mineCount = 10;
		break;
	case DIFFICULTY::INTERMEDIATE:
		this->height = 16;
		this->width = 16;
		this->flagsLeft = 40;
		this->mineCount = 40;
		break;
	case DIFFICULTY::EXPERT:
		this->height = 16;
		this->width = 30;
		this->flagsLeft = 99;
		this->mineCount = 99;
		break;
	case DIFFICULTY::CUSTOM:
		this->height = h;
		this->width = w;
		this->flagsLeft = m;
		this->mineCount = m;
	}
	difficulty = dif;
	tilesLeft = height * width - mineCount;

	this->seed = seed;

	NewGame();
}

Game::~Game() {

}

void Game::NewGame() {

	switch (difficulty) {
	case DIFFICULTY::BEGINNER:
		this->height = 9;
		this->width = 9;
		this->flagsLeft = 10;
		this->mineCount = 10;
		break;
	case DIFFICULTY::INTERMEDIATE:
		this->height = 16;
		this->width = 16;
		this->flagsLeft = 40;
		this->mineCount = 40;
		break;
	case DIFFICULTY::EXPERT:
		this->height = 16;
		this->width = 30;
		this->flagsLeft = 99;
		this->mineCount = 99;
		break;
	}
	this->gameActive = true;
	this->tilesLeft = width * height - mineCount;
	this->flagsLeft = mineCount;

	board = std::vector<std::vector<Cell>>(width, std::vector<Cell>(height, Cell()));

	for (int x = 0; x < this->board.size(); x++) {
		for (int y = 0; y < this->board[0].size(); y++) {
			board[x][y] = Cell(x, y, false);
		}
	}

	std::random_device rd;
	int rdSeed = rd();
	std::minstd_rand gen = (seed == NULL ? std::minstd_rand(rdSeed) : std::minstd_rand(seed));
	std::uniform_int_distribution<> distW(0, width - 1);
	std::uniform_int_distribution<> distH(0, height - 1);

	for (int i = 0; i < mineCount; i++) {
		int x = distW(gen);
		int y = distH(gen);
		while (board[x][y].isMine) {
			x = distW(gen);
			y = distH(gen);
		}
		board[x][y] = Cell(x, y, true);
	}

	for (int x = 0; x < this->board.size(); x++) {
		for (int y = 0; y < this->board[0].size(); y++) {
			Cell& c = this->board[x][y];
			CountAdjacentMines(c);
		}
	}
}

void Game::NewGame(int mineCount, u_int w, u_int h, DIFFICULTY dif) {
	if (w * h <= mineCount) {
		return;
	}

	this->flagsLeft = mineCount;
	this->mineCount = mineCount;
	this->width = w;
	this->height = h;
	if (dif != DIFFICULTY::CUSTOM) {
		this->difficulty = dif;
	}

	Game::NewGame();
	return;
}

bool Game::CheckForWin() {
	return tilesLeft == 0 && flagsLeft == 0;
}

void Game::SetTileSize(u_int width, u_int height) {
	float tileW = width / this->width;
	float tileH = height / this->height;
	this->tileSize = (tileW > tileH ? tileH : tileW);
}

void Game::CountAdjacentMines(Cell& c) {
	int xStart = (c.x > 0 ? -1 : 0);
	int yStart = (c.y > 0 ? -1 : 0);
	int xEnd = (c.x < this->width - 1 ? 2 : 1);
	int yEnd = (c.y < this->height - 1 ? 2 : 1);
	int count = 0;

	for (int x = xStart; x < xEnd; x++) {
		for (int y = yStart; y < yEnd; y++) {
			Cell& adj = this->board[c.x + x][c.y + y];
			if (adj.x == c.x && adj.y == c.y) {
				continue;
			}
			if (adj.isMine) {
				count++;
			}
		}
	}

	c.adjacentMines = count;
}

void Game::PrintDiscoveredGrid() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Cell& c = board[x][y];
			
			if (c.isDiscovered) {
				if (c.isMine) {
					std::cout << 'X' << ' ';
				}
				else if (c.adjacentMines == 0) {
					std::cout << '.' << ' ';
				}
				else {
					std::cout << c.adjacentMines << ' ';
				}
			}
			else {
				std::cout << '=' << ' ';
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}