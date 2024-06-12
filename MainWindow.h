#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <dwrite.h>
#pragma comment(lib, "Dwrite")
#include <wincodec.h>
#include <string>
#include <vector>
#include "Win32_BaseWindow.h"

class Moonsweeper;

class MainWindow : public BaseWindow<MainWindow> {

public:

	MainWindow(LPCWSTR name, u_int width, u_int height, Moonsweeper* moonsweeperPtr);
	MainWindow();
	~MainWindow();
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK OptionsMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	HRESULT CreateGraphicsResources(); 
	void DiscardGraphicsResources();
	void OnPaint();
	void Resize();
	BOOL Create(CREATEWNDSTRUCT& cws);

	Moonsweeper* GetMoonsweeper();

	float width;
	float height;
	bool customDiff = false;

private:
	Moonsweeper* moonsweeper;
	LPCWSTR m_name;

	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* renderTarget;

	HWND hBtnCancel = nullptr;
	HWND hBtnOk = nullptr;
	HWND hComboBoxDiff = nullptr;
	HWND hDlgOptions = nullptr;
	HWND hEditSeed = nullptr;
	HWND hEditWidth = nullptr;
	HWND hEditHeight = nullptr;
	HWND hEditMine = nullptr;
	HWND hStaticSeed = nullptr;
};

#endif