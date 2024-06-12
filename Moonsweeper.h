#ifndef MOONSWEEPER_H
#define MOONSWEEPER_H
#pragma once
#include "MainWindow.h"
#include "GameWindow.h"
#include "Game.h"
#include "SmileyWindow.h"
#include "InfoWindow.h"

class Moonsweeper
{
	
public:
	Moonsweeper(u_int width = 800, u_int height = 600);
	Moonsweeper(u_int width = 800, u_int height = 600, int nCmdShow = 1);
	~Moonsweeper();

	void Run();
	void CalculateLayout();

	Game game;
	MainWindow mainWindow;
	SmileyWindow smileyWindow;
	GameWindow gameWindow;
	InfoWindow infoWindow;

	int iWTotalHeight = 0;
	int tileWCount = 0;
	const int hBorderCount = 3;
	const int vBorderCount = 2;
	double timer = 0.0f;
	bool neverClicked = true;
	
	D2D1_COLOR_F const colorGrid = D2D1::ColorF(0.75f, 0.75f, 0.75f, 1.0f);
};

#endif