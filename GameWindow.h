#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#pragma once
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include "Win32_Helper.h"
#include "Win32_BaseWindow.h"
#include "MoonsweeperTypedef.h"

class Moonsweeper;
class Cell;

enum CHORD_RESULT {
    INVALID,
    CLEAR,
    MINE
};

class GameWindow : public BaseWindow<GameWindow>
{
public:
    GameWindow(HWND parentHwnd, Moonsweeper* moonsweeperPtr);
    GameWindow();
    ~GameWindow();

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    HRESULT CreateGraphicsResources();
    void DiscardGraphicsResources();
    void OnClick(int mouseX, int mouseY, MOUSE_BUTTONS_STATE mbs);
    void OnPaint();
    void OnHold(int mouseX, int mouseY, bool resetPrev = false);

    void ExpandEmptyCell(Cell& c);
    CHORD_RESULT ChordCell(Cell& c);

    void Show();
    //HRESULT LoadBitmapFromFile(PCWSTR path, ID2D1Bitmap** ppBitmap);
    BOOL Create(CREATEWNDSTRUCT& cws);

    IWICImagingFactory* GetIWICFactory();
    ID2D1HwndRenderTarget* GetRenderTarget();

    float width;
    float height;
    float x;
    float y;
private:
    HWND parentHwnd;
    Moonsweeper* moonsweeper = nullptr;
    LPCTSTR m_name = L"GameWindow";
    D2D1_RECT_F posRect;
    
    ID2D1Factory* factory = nullptr;
    ID2D1HwndRenderTarget* renderTarget = nullptr;
    IWICImagingFactory* iwicFactory = nullptr;
    ID2D1SolidColorBrush* redBrush = nullptr;

    ID2D1Bitmap* starBitmap = nullptr;
    ID2D1Bitmap* mineBitmap = nullptr;
    ID2D1Bitmap* notMineBitmap = nullptr;
    ID2D1Bitmap* oneBitmap = nullptr;
    ID2D1Bitmap* twoBitmap = nullptr;
    ID2D1Bitmap* threeBitmap = nullptr;
    ID2D1Bitmap* fourBitmap = nullptr;
    ID2D1Bitmap* fiveBitmap = nullptr;
    ID2D1Bitmap* sixBitmap = nullptr;
    ID2D1Bitmap* sevenBitmap = nullptr;
    ID2D1Bitmap* eightBitmap = nullptr;
    ID2D1Bitmap* hiddenBitmap = nullptr;
    ID2D1Bitmap* emptyBitmap = nullptr;
    ID2D1Bitmap* horizontalBorderBitmap = nullptr;
    ID2D1Bitmap* verticalBorderBitmap = nullptr;
    ID2D1Bitmap* lowerLeftCornerBitmap = nullptr;
    ID2D1Bitmap* lowerRightCornerBitmap = nullptr;
    
    D2D1_COLOR_F colorBlack = D2D1::ColorF(0.0f, 0.0f, 0.0f);
    D2D1_COLOR_F colorRed = D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f);
};

#endif