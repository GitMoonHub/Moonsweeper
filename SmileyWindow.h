#ifndef SMILEYWINDOW_H
#define SMILEYWINDOW_H

#pragma once
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <wincodec.h>
#include "Win32_BaseWindow.h"
#include "MoonsweeperTypedef.h"

enum SMILEY_STATE {
	DEFAULT,
	PUSHED,
	COOL,
	DEAD,
	SHOCKED
};

class Moonsweeper;

class SmileyWindow : public BaseWindow<SmileyWindow>
{
public:
	SmileyWindow(HWND parentHwnd, Moonsweeper* moonsweeperPtr);
	SmileyWindow();
	~SmileyWindow();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	HRESULT CreateGraphicsResources();
	void DiscardGraphicsResources();

	void OnPaint();
	void Show();
	BOOL Create(CREATEWNDSTRUCT& cws);

	IWICImagingFactory* GetIWICFactory();
	ID2D1HwndRenderTarget* GetRenderTarget();

	float width = 0;
	float height = 0;
	float x = 0;
	float y = 0;
	D2D1_RECT_F posRect;
	SMILEY_STATE state = DEFAULT;

private:
	LPCTSTR m_name = L"SmileyWindow";
	HWND parentHwnd;
	Moonsweeper* moonsweeper = nullptr;
	
	ID2D1Factory* factory = nullptr;
	ID2D1HwndRenderTarget* renderTarget = nullptr;
	IWICImagingFactory* iwicFactory = nullptr;
	ID2D1Bitmap* smileyBitmap = nullptr;
	ID2D1Bitmap* coolBitmap = nullptr;
	ID2D1Bitmap* shockedBitmap = nullptr;
	ID2D1Bitmap* deadBitmap = nullptr;
	ID2D1Bitmap* pushedBitmap = nullptr;
};

#endif