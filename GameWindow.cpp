#include <iostream>
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <windowsx.h>
#include "GameWindow.h"
#include "Win32_Helper.h"
#include "Moonsweeper.h"
#include "Cell.h"
#include "MoonsweeperTypedef.h"

extern u_int g_WIDTH;
extern u_int g_HEIGHT;

GameWindow::GameWindow(HWND parentHwnd, Moonsweeper* moonsweeperPtr) :
    renderTarget(NULL), 
    factory(NULL),  
    iwicFactory(NULL) {

    this->parentHwnd = parentHwnd;
    this->moonsweeper = moonsweeperPtr;
}

GameWindow::GameWindow() {
    
}

GameWindow::~GameWindow() {
    
}

LRESULT GameWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HRESULT hr = S_OK;
    int mouseX = 0;
    int mouseY = 0;
    static bool hold = false;
    static bool tracking = false;
    MOUSE_BUTTONS_STATE mbs = LEFTCLICK;
    
    switch (uMsg) {
        case WM_CREATE:
            hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
            if (FAILED(hr)) {
                std::cout << "[GameWindow]: D2D1CreateFactory() failed. HRESULT = " << hr << "\n";
                exit(-1);
            }
            hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), reinterpret_cast<void**>(&iwicFactory));
            if (FAILED(hr)) {
                std::cout << "[GameWindow]: Couldn't create a IWICImagingFactory instance. HRESULT = " << hr << "\n";
                exit(-1);
            }
            hr = CreateGraphicsResources();
            std::cout << "[GameWindow]: Successfully created the window.\n";
            return 0;

        case WM_DESTROY:
            DiscardGraphicsResources();
            Win32_Helper::SafeRelease(&factory);
            Win32_Helper::SafeRelease(&iwicFactory);
            return 0;

        case WM_MOUSEMOVE:
            mouseX = GET_X_LPARAM(lParam);
            mouseY = GET_Y_LPARAM(lParam);

            if (!tracking) {
                TRACKMOUSEEVENT tme;
                tme.cbSize = sizeof(tme);
                tme.dwFlags = TME_LEAVE;
                tme.hwndTrack = m_hwnd;
                tme.dwHoverTime = HOVER_DEFAULT;
                TrackMouseEvent(&tme);
                tracking = true;
            }

            if (wParam & MK_LBUTTON) {
                OnHold(mouseX, mouseY);
                SendMessage(m_hwnd, WM_PAINT, NULL, NULL);
            }
            return 0;

        case WM_MOUSELEAVE:
            tracking = false;
            if (moonsweeper->game.gameActive) {
                moonsweeper->smileyWindow.state = DEFAULT;
                OnHold(NULL, NULL, true);
                SendMessage(moonsweeper->smileyWindow.Window(), WM_PAINT, NULL, NULL);
            }
            return 0;

        case WM_LBUTTONDOWN:
            if (!moonsweeper->game.gameActive) {
                return 0;
            }
            mouseX = GET_X_LPARAM(lParam);
            mouseY = GET_Y_LPARAM(lParam);
            moonsweeper->smileyWindow.state = SHOCKED;
            OnHold(mouseX, mouseY);
            SendMessage(moonsweeper->smileyWindow.Window(), WM_PAINT, NULL, NULL);
            return 0;

        case WM_LBUTTONUP:
            if (!moonsweeper->game.gameActive) {
                return 0;
            }
            mouseX = GET_X_LPARAM(lParam);
            mouseY = GET_Y_LPARAM(lParam);
            mbs = LEFTCLICK;
            if (wParam & MK_RBUTTON) {
                mbs = BOTH;
            }
            OnClick(mouseX, mouseY, mbs);
            SendMessage(moonsweeper->smileyWindow.Window(), WM_PAINT, NULL, NULL);
            return 0;

        case WM_RBUTTONUP:
            if (!moonsweeper->game.gameActive) {
                return 0;
            }
            mouseX = GET_X_LPARAM(lParam);
            mouseY = GET_Y_LPARAM(lParam);
            mbs = RIGHTCLICK;
            if (wParam & MK_LBUTTON) {
                mbs = BOTH;
            }
            OnClick(mouseX, mouseY, mbs);
            return 0;

        case WM_PAINT:
            OnPaint();
            return 0;
        }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}


HRESULT GameWindow::CreateGraphicsResources() {
    HRESULT hr = S_OK;
    // Check if there is no render target
    if (renderTarget == NULL) {

        RECT rc;
        GetClientRect(parentHwnd, &rc);
        D2D1_SIZE_F mwSize = D2D1::SizeF(rc.right, rc.bottom);
        
        D2D1_SIZE_U rtSize = D2D1::SizeU(this->width, this->height);
        // Create render target
        hr = factory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, rtSize),
            &renderTarget);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: HwndRenderTarget creation failed.\n";
            exit(-1);
        }

        // Brushes
        hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.0f, 0.0f), &redBrush);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: RedBrush creation failed.\n";
            exit(-1);
        }

        // Bitmaps
        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/Star.png", &starBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: StarBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/Mine.png", &mineBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: WilberBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/notMine.png", &notMineBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: NotWilberBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/one.png", &oneBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: OneBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/two.png", &twoBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: TwoBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/three.png", &threeBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: ThreeBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/four.png", &fourBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: FourBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/five.png", &fiveBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: FiveBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/six.png", &sixBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: SixBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/seven.png", &sevenBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: SevenBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/eight.png", &eightBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: EightBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/HiddenTile.png", &hiddenBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: HiddenBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/EmptyTile.png", &emptyBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: EmptyBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/HorizontalBorder.png", &horizontalBorderBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: HorizontalBorderBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/VerticalBorder.png", &verticalBorderBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: VerticalBorderBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/lowerLeftCorner.png", &lowerLeftCornerBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: LowerLeftCornerBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<GameWindow>(this, L"assets/lowerRightCorner.png", &lowerRightCornerBitmap);
        if (FAILED(hr)) {
            std::cout << "[GameWindow]: LowerRightCornerBitmap creation failed.\n";
        }
    
    }
    return hr;
}

void GameWindow::DiscardGraphicsResources() {
    using namespace Win32_Helper;

    SafeRelease(&factory);
    SafeRelease(&renderTarget);
    SafeRelease(&iwicFactory);
    SafeRelease(&redBrush);
    SafeRelease(&starBitmap);
    SafeRelease(&mineBitmap);
    SafeRelease(&notMineBitmap);
    SafeRelease(&oneBitmap);
    SafeRelease(&twoBitmap);
    SafeRelease(&threeBitmap);
    SafeRelease(&fourBitmap);
    SafeRelease(&fiveBitmap);
    SafeRelease(&sixBitmap);
    SafeRelease(&sevenBitmap);
    SafeRelease(&eightBitmap);
    SafeRelease(&hiddenBitmap);
    SafeRelease(&emptyBitmap);
    SafeRelease(&horizontalBorderBitmap);
    SafeRelease(&verticalBorderBitmap);
    SafeRelease(&lowerLeftCornerBitmap);
    SafeRelease(&lowerRightCornerBitmap);
}

void GameWindow::OnClick(int mx, int my, MOUSE_BUTTONS_STATE mbs) {
    
    float xOffsetDiff = 0.0f;

    /*if (moonsweeper->game.difficulty == Game::DIFFICULTY::BEGINNER 
        || moonsweeper->game.difficulty == Game::DIFFICULTY::INTERMEDIATE) {
        xOffsetDiff = 15 - (float) moonsweeper->game.width / 2.0f;
        xOffsetDiff *= moonsweeper->game.tileSize;
    }*/

    int tileX = (mx - xOffsetDiff - moonsweeper->game.tileSize) / moonsweeper->game.tileSize;   // offset for border
    int tileY = my / moonsweeper->game.tileSize;
    if (tileY >= moonsweeper->game.height || tileX >= moonsweeper->game.width) {
        return;
    }
    Cell& c = moonsweeper->game.board[tileX][tileY];
    CHORD_RESULT cr = INVALID;

    switch (mbs) {
    case LEFTCLICK:
        if (moonsweeper->neverClicked) {
            moonsweeper->neverClicked = false;
            moonsweeper->timer = 0;
        }

        if (c.isMine) {
            c.hasExploded = true;
            moonsweeper->game.gameActive = false;
            moonsweeper->smileyWindow.state = DEAD;
            SendMessage(moonsweeper->smileyWindow.Window(), WM_PAINT, 0, 0);
            break;
        }
        else if (c.isDiscovered) {
            moonsweeper->smileyWindow.state = DEFAULT;
            SendMessage(moonsweeper->smileyWindow.Window(), WM_PAINT, 0, 0);
            return;
        }
        
        c.isDiscovered = true;
        moonsweeper->game.tilesLeft--;
        moonsweeper->smileyWindow.state = DEFAULT;

        // We need to expand from the empty cell
        if (c.adjacentMines == 0) {
            ExpandEmptyCell(c);
        }
        // Check if game won
        if (moonsweeper->game.CheckForWin()) {
            moonsweeper->smileyWindow.state = COOL;
            moonsweeper->game.won = true;
            moonsweeper->game.gameActive = false;
        }
        SendMessage(moonsweeper->smileyWindow.Window(), WM_PAINT, 0, 0);
        break;

    case RIGHTCLICK:
        if (moonsweeper->neverClicked) {
            moonsweeper->neverClicked = false;
            moonsweeper->timer = 0;
        }

        if (c.isDiscovered) {
            break;
        }
        if (c.isFlagged) {
            c.isFlagged = false;
            moonsweeper->game.flagsLeft++;
            SendMessage(moonsweeper->infoWindow.Window(), WM_PAINT, 0, 0);
            break;
        }
        else if (c.isMine) {
            c.isFlagged = true;
            moonsweeper->game.flagsLeft--;

            if (moonsweeper->game.CheckForWin()) {
                moonsweeper->smileyWindow.state = COOL;
                moonsweeper->game.won = true;
                moonsweeper->game.gameActive = false;    
            }
            SendMessage(moonsweeper->mainWindow.Window(), WM_PAINT, 0, 0);
            break;
        }
        c.isFlagged = true;
        moonsweeper->game.flagsLeft--;
        SendMessage(moonsweeper->infoWindow.Window(), WM_PAINT, 0, 0);
        break;

    case BOTH:
        if (moonsweeper->neverClicked) {
            moonsweeper->neverClicked = false;
            moonsweeper->timer = 0;
        }

        cr = ChordCell(c);

        if (cr == CHORD_RESULT::MINE) {
            c.hasExploded = true;
            moonsweeper->game.gameActive = false;
            moonsweeper->smileyWindow.state = DEAD;
            SendMessage(moonsweeper->mainWindow.Window(), WM_PAINT, 0, 0);
        }
        else if (cr == CHORD_RESULT::CLEAR) {
            moonsweeper->smileyWindow.state = DEFAULT;

            // Check if game won
            if (moonsweeper->game.CheckForWin()) {
                moonsweeper->smileyWindow.state = COOL;
                moonsweeper->game.won = true;
                moonsweeper->game.gameActive = false;
            }
            SendMessage(moonsweeper->mainWindow.Window(), WM_PAINT, 0, 0);
        }
        break;
    }
    SendMessage(m_hwnd, WM_PAINT, 0, 0);
}

void GameWindow::OnPaint() {
    HRESULT hr = CreateGraphicsResources();
    
    if (SUCCEEDED(hr)) {
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);

        renderTarget->BeginDraw();
        renderTarget->Clear(moonsweeper->colorGrid);
        float xOffsetDiff = 0.0f;

        /*if (moonsweeper->game.difficulty == Game::DIFFICULTY::BEGINNER 
            || moonsweeper->game.difficulty == Game::DIFFICULTY::INTERMEDIATE) {
            xOffsetDiff = 15.0f - (float)moonsweeper->game.width / 2.0f;
        }*/

        D2D1_RECT_F rect = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);
        const int xEnd = moonsweeper->game.board.size() + moonsweeper->vBorderCount;
        const int xOffset = moonsweeper->vBorderCount / 2;
        for (int x = 0; x < xEnd; x++) {
            rect.left = (x + xOffsetDiff) * moonsweeper->game.tileSize;
            rect.right = rect.left + moonsweeper->game.tileSize;
            const int yEnd = moonsweeper->game.board[0].size() + 1;
            
            for (int y = 0; y < yEnd; y++) {
                rect.top = y * moonsweeper->game.tileSize;
                rect.bottom = rect.top + moonsweeper->game.tileSize;

                // Lower left corner
                if (x == 0 && y == yEnd - 1) {
                    renderTarget->DrawBitmap(lowerLeftCornerBitmap, rect);
                    continue;
                }
                // Lower right corner
                else if (x == xEnd - 1 && y == yEnd - 1) {
                    renderTarget->DrawBitmap(lowerRightCornerBitmap, rect);
                    continue;
                }
                // Vertical border
                if (x == 0 || x == xEnd - 1) {
                    renderTarget->DrawBitmap(verticalBorderBitmap, rect);
                    continue;
                }
                // Horizontal border
                else if (y == yEnd - 1) {
                    renderTarget->DrawBitmap(horizontalBorderBitmap, rect);
                    continue;
                }
                Cell& c = moonsweeper->game.board[x - xOffset][y];

                if (!moonsweeper->game.gameActive) {
                    if (c.isFlagged && !c.isMine) {
                        renderTarget->DrawBitmap(notMineBitmap, rect);
                    }
                    else if (c.hasExploded) {
                        renderTarget->FillRectangle(rect, redBrush);
                        renderTarget->DrawBitmap(mineBitmap, rect);
                    }
                    else if (c.isFlagged) {
                        renderTarget->DrawBitmap(starBitmap, rect);
                    }
                    else if (c.isMine) {
                        renderTarget->DrawBitmap(mineBitmap, rect);
                    }
                    else if (c.isDiscovered) {
                        switch (c.adjacentMines) {
                        case 0:
                            renderTarget->DrawBitmap(emptyBitmap, rect);
                            break;
                        case 1:
                            renderTarget->DrawBitmap(oneBitmap, rect);
                            break;
                        case 2:
                            renderTarget->DrawBitmap(twoBitmap, rect);
                            break;
                        case 3:
                            renderTarget->DrawBitmap(threeBitmap, rect);
                            break;
                        case 4:
                            renderTarget->DrawBitmap(fourBitmap, rect);
                            break;
                        case 5:
                            renderTarget->DrawBitmap(fiveBitmap, rect);
                            break;
                        case 6:
                            renderTarget->DrawBitmap(sixBitmap, rect);
                            break;
                        case 7:
                            renderTarget->DrawBitmap(sevenBitmap, rect);
                            break;
                        case 8:
                            renderTarget->DrawBitmap(eightBitmap, rect);
                            break;
                        }
                    }
                    else {
                        renderTarget->DrawBitmap(hiddenBitmap, rect);
                    }
                }
                else {
                    if (c.isFlagged) {
                        renderTarget->DrawBitmap(starBitmap, rect);
                    }
                    else if (c.hasExploded) {
                        renderTarget->FillRectangle(rect, redBrush);
                        renderTarget->DrawBitmap(mineBitmap, rect);
                    }
                    else if (c.isPushed) {
                        renderTarget->DrawBitmap(emptyBitmap, rect);
                    }
                    else {
                        renderTarget->DrawBitmap(hiddenBitmap, rect);
                    }
                }
                if (c.isDiscovered) {
                    switch (c.adjacentMines) {
                        case 0:
                            renderTarget->DrawBitmap(emptyBitmap, rect);
                            break;
                        case 1:
                            renderTarget->DrawBitmap(oneBitmap, rect);
                            break;
                        case 2:
                            renderTarget->DrawBitmap(twoBitmap, rect);
                            break;
                        case 3:
                            renderTarget->DrawBitmap(threeBitmap, rect);
                            break;
                        case 4:
                            renderTarget->DrawBitmap(fourBitmap, rect);
                            break;
                        case 5:
                            renderTarget->DrawBitmap(fiveBitmap, rect);
                            break;
                        case 6:
                            renderTarget->DrawBitmap(sixBitmap, rect);
                            break;
                        case 7:
                            renderTarget->DrawBitmap(sevenBitmap, rect);
                            break;
                        case 8:
                            renderTarget->DrawBitmap(eightBitmap, rect);
                            break;
                    }
                }                
            }
        }

        renderTarget->EndDraw();

        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }
        EndPaint(m_hwnd, &ps);
    }
}

void GameWindow::OnHold(int mouseX, int mouseY, bool resetPrev) {

    static int prevTileX = 0;
    static int prevTileY = 0;

    float xOffsetDiff = 0.0f;

    /*if (moonsweeper->game.difficulty == Game::DIFFICULTY::BEGINNER
        || moonsweeper->game.difficulty == Game::DIFFICULTY::INTERMEDIATE) {
        xOffsetDiff = 150.f - (float)moonsweeper->game.width / 2.0f;
        xOffsetDiff *= moonsweeper->game.tileSize;
    }*/

    int tileX = (mouseX - xOffsetDiff - moonsweeper->game.tileSize) / moonsweeper->game.tileSize;   // offset for border
    int tileY = mouseY / moonsweeper->game.tileSize;
    Cell& prevCell = moonsweeper->game.board[prevTileX][prevTileY];

    // Only executed when the mouse leaves the ClientArea while holding down LBUTTON
    if (resetPrev) {
        prevCell.isPushed = false;
        SendMessage(m_hwnd, WM_PAINT, NULL, NULL);
        return;
    }

    if (tileX == prevTileX && tileY == prevTileY) {
        return;
    }

    if (tileY >= moonsweeper->game.height || tileX >= moonsweeper->game.width) {
        tileX = 0;
        tileY = 0;
        prevCell.isPushed = false;
        moonsweeper->smileyWindow.state = SMILEY_STATE::DEFAULT;
        SendMessage(moonsweeper->smileyWindow.Window(), WM_PAINT, NULL, NULL);
        SendMessage(m_hwnd, WM_PAINT, NULL, NULL);
        return;
    }

    Cell& c = moonsweeper->game.board[tileX][tileY];

    if (!c.isDiscovered && !c.isFlagged) {
        c.isPushed = true;
        moonsweeper->smileyWindow.state = SHOCKED;
        SendMessage(moonsweeper->smileyWindow.Window(), WM_PAINT, NULL, NULL);
    }
    if (prevCell.isPushed && !(tileX == prevTileX && tileY == prevTileY)) {
        prevCell.isPushed = false;
    }
    
    SendMessage(m_hwnd, WM_PAINT, NULL, NULL);
    prevTileX = tileX;
    prevTileY = tileY;
}

void GameWindow::ExpandEmptyCell(Cell& c) {
    const int xStart = (c.x > 0 ? -1 : 0);
    const int yStart = (c.y > 0 ? -1 : 0);
    const int xEnd = (c.x < moonsweeper->game.width - 1 ? 2 : 1);
    const int yEnd = (c.y < moonsweeper->game.height - 1 ? 2 : 1);

    for (int x = xStart; x < xEnd; x++) {
        for (int y = yStart; y < yEnd; y++) {
            Cell& adj = moonsweeper->game.board[c.x + x][c.y + y];

            // Already visited this tile
            if (adj.isDiscovered) {
                continue;
            }
            adj.isDiscovered = true;
            moonsweeper->game.tilesLeft -= 1;

            // If no adjacent mines, recursively invoke
            // this function for that tile
            if (adj.adjacentMines == 0) {
                ExpandEmptyCell(adj);
            }
        }
    }
}

CHORD_RESULT GameWindow::ChordCell(Cell& c) {

    const int xStart = (c.x > 0 ? -1 : 0);
    const int yStart = (c.y > 0 ? -1 : 0);
    const int xEnd = (c.x < moonsweeper->game.width - 1 ? 2 : 1);
    const int yEnd = (c.y < moonsweeper->game.height - 1 ? 2 : 1);

    int flagCount = 0;
    for (int x = xStart; x < xEnd; x++) {
        for (int y = yStart; y < yEnd; y++) {
            Cell& adj = moonsweeper->game.board[c.x + x][c.y + y];

            // Already visited this tile
            if (adj.isDiscovered) {
                continue;
            }
            // Check for flag
            if (adj.isFlagged) {
                flagCount++;
            }
        }
    }

    // If tile not satisfied, we return
    if (c.adjacentMines != flagCount) {
        return INVALID;
    }

    for (int x = xStart; x < xEnd; x++) {
        for (int y = yStart; y < yEnd; y++) {
            Cell& adj = moonsweeper->game.board[c.x + x][c.y + y];

            // Already visited this tile or flagged tile
            if (adj.isDiscovered || adj.isFlagged) {
                continue;
            }
            // Check for mine
            if (adj.isMine) {
                return MINE;
            }

            // Reveal tile
            if (!adj.isDiscovered) {
                moonsweeper->game.tilesLeft--;
                adj.isDiscovered = true;
                if (adj.adjacentMines == 0) {
                    ExpandEmptyCell(adj);
                }
            }
        }
    }

    return CLEAR;
}

void GameWindow::Show() {
    ShowWindow(this->m_hwnd, SW_SHOW);
}

BOOL GameWindow::Create(CREATEWNDSTRUCT& cws) {
    if (cws.x < 0) {
        cws.x = CW_USEDEFAULT;
    }
    if (cws.y < 0) {
        cws.y = CW_USEDEFAULT;
    }

    if (cws.nWidth < 1) {
        cws.nWidth = width;
    }
    else {
        width = cws.nWidth;
    }
    if (cws.nHeight < 1) {
        cws.nHeight = height;
    }
    else {
        height = cws.nHeight;
    }

    cws.lpClassName = m_name;
    cws.hMenu = reinterpret_cast<HMENU>(CHILD_WINDOW_ID::GAME_WINDOW_ID);
    REGWNDSTRUCT rws;
    rws.lpszClassName = m_name;
    rws.hCursor = LoadCursor(NULL, IDC_ARROW);
    return BaseWindow::Create(cws, rws);
}

IWICImagingFactory* GameWindow::GetIWICFactory() {
    return iwicFactory;
}

ID2D1HwndRenderTarget* GameWindow::GetRenderTarget() {
    return renderTarget;
}