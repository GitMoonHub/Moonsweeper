#ifndef GAME_H
#define GAME_H
#pragma once
#include <vector>
#include "Cell.h"

class Game
{
public:
	enum class DIFFICULTY {
		BEGINNER,
		INTERMEDIATE,
		EXPERT,
		CUSTOM
	};
	std::vector<std::vector<Cell>>board;
	int flagsLeft = -1;
	u_int width = 0;
	u_int height = 0;
	int mineCount = 0;
	float tileSize = 20.0f;
	u_int seed = NULL;
	bool gameActive = false;
	bool won = false;
	int tilesLeft = -1;
	DIFFICULTY difficulty = DIFFICULTY::EXPERT;

	Game(DIFFICULTY dif = DIFFICULTY::EXPERT, u_int w = 30, u_int h = 16, u_int m = 99, u_int seed = NULL);
	~Game();

	void NewGame();
	void NewGame(int mineCount, u_int w, u_int h, DIFFICULTY dif = DIFFICULTY::CUSTOM);
	bool CheckForWin();
	void SetTileSize(u_int width, u_int height);
	void CountAdjacentMines(Cell& c);
	void PrintDiscoveredGrid();
};

#endif