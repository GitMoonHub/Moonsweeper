#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#pragma once
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <wincodec.h>
#include "Win32_BaseWindow.h"

class Moonsweeper;

class InfoWindow : public BaseWindow<InfoWindow>
{
public:
	InfoWindow(HWND parentHwnd, Moonsweeper* moonsweeperPtr);
	InfoWindow();
	~InfoWindow();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	HRESULT CreateGraphicsResources();
	void DiscardGraphicsResources();

	void OnPaint();
	void Show();
	BOOL Create(CREATEWNDSTRUCT& cws);

	IWICImagingFactory* GetIWICFactory();
	ID2D1HwndRenderTarget* GetRenderTarget();

	float x = 0.0f;
	float y = 0.0f;
	float width;
	float height;
	int heightTiles = 2;
	D2D1_RECT_F posRect;

private:
	HWND parentHwnd;
	LPCTSTR m_name = L"InfoWindow";
	Moonsweeper* moonsweeper = nullptr;

	int scoreboardY = 1;
	int firstNumX = 2;
	int secondNumX = 3;
	int thirdNumX = 4;

	ID2D1Factory* factory = nullptr;
	ID2D1HwndRenderTarget* renderTarget = nullptr;
	IWICImagingFactory* iwicFactory = nullptr;
	
	ID2D1Bitmap* horizontalBorderBitmap = nullptr;
	ID2D1Bitmap* verticalBorderBitmap = nullptr;
	ID2D1Bitmap* upperLeftCornerBitmap = nullptr;
	ID2D1Bitmap* upperRightCornerBitmap = nullptr;
	ID2D1Bitmap* middleLeftBorderBitmap = nullptr;
	ID2D1Bitmap* middleRightBorderBitmap = nullptr;

	ID2D1Bitmap* scoreZeroBitmap = nullptr;
	ID2D1Bitmap* scoreOneBitmap = nullptr;
	ID2D1Bitmap* scoreTwoBitmap = nullptr;
	ID2D1Bitmap* scoreThreeBitmap = nullptr;
	ID2D1Bitmap* scoreFourBitmap = nullptr;
	ID2D1Bitmap* scoreFiveBitmap = nullptr;
	ID2D1Bitmap* scoreSixBitmap = nullptr;
	ID2D1Bitmap* scoreSevenBitmap = nullptr;
	ID2D1Bitmap* scoreEightBitmap = nullptr;
	ID2D1Bitmap* scoreNineBitmap = nullptr;
	ID2D1Bitmap* scoreMinusBitmap = nullptr;
};

#endif