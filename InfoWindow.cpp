#include "InfoWindow.h"
#include "Moonsweeper.h"
#include "MoonsweeperTypedef.h"
#include "Win32_Helper.h"

InfoWindow::InfoWindow(HWND parentHwnd, Moonsweeper* moonsweeperPtr) :
	moonsweeper(moonsweeperPtr), 
	parentHwnd(parentHwnd) {

}

InfoWindow::InfoWindow() {

}

InfoWindow::~InfoWindow() {

}

LRESULT InfoWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HRESULT hr = S_OK;
    switch (uMsg) {
        case WM_CREATE:
            hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
            if (FAILED(hr)) {
                std::cout << "[InfoWindow]: D2D1CreateFactory() failed. HRESULT = " << hr << "\n";
                exit(-1);
            }
            hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), reinterpret_cast<void**>(&iwicFactory));
            if (FAILED(hr)) {
                std::cout << "[InfoWindow]: Couldn't create a IWICImagingFactory instance. HRESULT = " << hr << "\n";
                exit(-1);
            }
            hr = CreateGraphicsResources();
            std::cout << "[InfoWindow]: Successfully created the window.\n";
            return 0;

        case WM_DESTROY:
            DiscardGraphicsResources();
            Win32_Helper::SafeRelease(&factory);
            Win32_Helper::SafeRelease(&iwicFactory);
            return 0;

        case WM_LBUTTONUP:
            moonsweeper->smileyWindow.state = DEFAULT;
            SendMessage(moonsweeper->smileyWindow.Window(), WM_PAINT, 0, 0);
            return 0;

        case WM_PAINT:
            OnPaint();
            return 0;

    }
	return DefWindowProc(this->Window(), uMsg, wParam, lParam);
}

HRESULT InfoWindow::CreateGraphicsResources() {
    HRESULT hr = S_OK;
    // Check if there is no render target
    if (renderTarget == NULL) {
        RECT rc;
        GetClientRect(m_hwnd, &rc);
        posRect = D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom);
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

         // Bitmaps
         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/VerticalBorder.png", &verticalBorderBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: VerticalBorderBitmap creation failed.\n";
         }

         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/HorizontalBorder.png", &horizontalBorderBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: HorizontalBorderBitmap creation failed.\n";
         }

         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/upperLeftCorner.png", &upperLeftCornerBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: UpperLeftCornerBitmap creation failed.\n";
         }

         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/upperRightCorner.png", &upperRightCornerBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: UpperRightCornerBitmap creation failed.\n";
         }

         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/middleLeftBorder.png", &middleLeftBorderBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: MiddleLeftBorder creation failed.\n";
         }

         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/middleRightBorder.png", &middleRightBorderBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: MiddleRightBorder creation failed.\n";
         }

         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/scoreZero.png", &scoreZeroBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: ScoreZeroBitmap creation failed.\n";
         }

         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/scoreOne.png", &scoreOneBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: ScoreOneBitmap creation failed.\n";
         }

         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/scoreTwo.png", &scoreTwoBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: ScoreTwoBitmap creation failed.\n";
         }

         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/scoreThree.png", &scoreThreeBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: ScoreThreeBitmap creation failed.\n";
         }

         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/scoreFour.png", &scoreFourBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: ScoreFourBitmap creation failed.\n";
         }

         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/scoreFive.png", &scoreFiveBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: ScoreFiveBitmap creation failed.\n";
         }

         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/scoreSix.png", &scoreSixBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: ScoreSixBitmap creation failed.\n";
         }

         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/scoreSeven.png", &scoreSevenBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: ScoreSevenBitmap creation failed.\n";
         }

         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/scoreEight.png", &scoreEightBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: ScoreEightBitmap creation failed.\n";
         }

         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/scoreNine.png", &scoreNineBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: ScoreNineBitmap creation failed.\n";
         }

         hr = Win32_Helper::LoadBitmapFromFile<InfoWindow>(this, L"assets/scoreMinus.png", &scoreMinusBitmap);
         if (FAILED(hr)) {
             std::cout << "[InfoWindow]: ScoreMinusBitmap creation failed.\n";
         }
    }
}

void InfoWindow::DiscardGraphicsResources() {
    using namespace Win32_Helper;

    //SafeRelease(&factory);
    SafeRelease(&renderTarget);
    //SafeRelease(&iwicFactory);
    SafeRelease(&horizontalBorderBitmap);
    SafeRelease(&verticalBorderBitmap);
    SafeRelease(&upperLeftCornerBitmap);
    SafeRelease(&upperRightCornerBitmap);
    SafeRelease(&middleLeftBorderBitmap);
    SafeRelease(&middleRightBorderBitmap);
    SafeRelease(&scoreZeroBitmap);
    SafeRelease(&scoreOneBitmap);
    SafeRelease(&scoreTwoBitmap);
    SafeRelease(&scoreThreeBitmap);
    SafeRelease(&scoreFourBitmap);
    SafeRelease(&scoreFiveBitmap);
    SafeRelease(&scoreSixBitmap);
    SafeRelease(&scoreSevenBitmap);
    SafeRelease(&scoreEightBitmap);
    SafeRelease(&scoreNineBitmap);
    SafeRelease(&scoreMinusBitmap);
}

void InfoWindow::OnPaint() {
    HRESULT hr = CreateGraphicsResources();

    if (SUCCEEDED(hr)) {
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);

        renderTarget->BeginDraw();
        renderTarget->Clear(moonsweeper->colorGrid);

        D2D1_RECT_F rect = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);
        const int xEnd = moonsweeper->tileWCount;

        // Upper left corner
        rect.left = 0;
        rect.right = rect.left + moonsweeper->game.tileSize;
        rect.top = 0;
        rect.bottom = rect.top + moonsweeper->game.tileSize;
        renderTarget->DrawBitmap(upperLeftCornerBitmap, rect);

        // Upper right corner
        rect.left = (xEnd - 1) * moonsweeper->game.tileSize;
        rect.right = rect.left + moonsweeper->game.tileSize;
        rect.top = 0;
        rect.bottom = rect.top + moonsweeper->game.tileSize;
        renderTarget->DrawBitmap(upperRightCornerBitmap, rect);
        
        // Middle right border
        rect.top = (moonsweeper->iWTotalHeight - 1) * moonsweeper->game.tileSize;
        rect.bottom = rect.top + moonsweeper->game.tileSize;
        renderTarget->DrawBitmap(middleRightBorderBitmap, rect);

        // Middle left border
        rect.left = 0;
        rect.right = rect.left + moonsweeper->game.tileSize;
        renderTarget->DrawBitmap(middleLeftBorderBitmap, rect);

        // Vertical borders
        const int lRightBorder = (xEnd - 1) * moonsweeper->game.tileSize;
        const int rRightBorder = lRightBorder + moonsweeper->game.tileSize;

        for (int i = 1; i < moonsweeper->iWTotalHeight; i++) {
            // Get y coordinate
            rect.top = i * moonsweeper->game.tileSize;
            rect.bottom = rect.top + moonsweeper->game.tileSize;

            // Left side
            rect.left = 0;
            rect.right = moonsweeper->game.tileSize;
            renderTarget->DrawBitmap(verticalBorderBitmap, rect);

            // Right side
            rect.left = lRightBorder;
            rect.right = rRightBorder;
            renderTarget->DrawBitmap(verticalBorderBitmap, rect);
        }


        // Horizontal Borders
        const int tBottomBorder = (moonsweeper->iWTotalHeight - 1) * moonsweeper->game.tileSize;
        const int bBottomBorder = tBottomBorder + moonsweeper->game.tileSize;

        for (int i = 1; i < xEnd - 1; i++) {
            // Get x coordinate
            rect.left = i * moonsweeper->game.tileSize;
            rect.right = rect.left + moonsweeper->game.tileSize;

            // Top border
            rect.top = 0;
            rect.bottom = rect.top + moonsweeper->game.tileSize;
            renderTarget->DrawBitmap(horizontalBorderBitmap, rect);

            // Bottom border
            rect.top = tBottomBorder;
            rect.bottom = bBottomBorder;
            renderTarget->DrawBitmap(horizontalBorderBitmap, rect);
        }

        // Flag lefts scoreboard
        int hundo = 0;
        int teno = 0;
        int uno = 0;    // lol

        // First number
        rect.left = (moonsweeper->game.difficulty == Game::DIFFICULTY::BEGINNER ? (firstNumX - 1 ) * moonsweeper->game.tileSize : firstNumX * moonsweeper->game.tileSize);
        rect.right = rect.left + moonsweeper->game.tileSize;
        rect.top = scoreboardY * moonsweeper->game.tileSize;
        rect.bottom = rect.top + moonsweeper->game.tileSize * 2;

        if (moonsweeper->game.flagsLeft > 99) {
            hundo = moonsweeper->game.flagsLeft / 100;
            switch (hundo) {
            case 0:
                renderTarget->DrawBitmap(scoreZeroBitmap, rect);
                break;
            case 1:
                renderTarget->DrawBitmap(scoreOneBitmap, rect);
                break;
            case 2:
                renderTarget->DrawBitmap(scoreTwoBitmap, rect);
                break;
            case 3:
                renderTarget->DrawBitmap(scoreThreeBitmap, rect);
                break;
            case 4:
                renderTarget->DrawBitmap(scoreFourBitmap, rect);
                break;
            case 5:
                renderTarget->DrawBitmap(scoreFiveBitmap, rect);
                break;
            case 6:
                renderTarget->DrawBitmap(scoreSixBitmap, rect);
                break;
            case 7:
                renderTarget->DrawBitmap(scoreSevenBitmap, rect);
                break;
            case 8:
                renderTarget->DrawBitmap(scoreEightBitmap, rect);
                break;
            case 9:
                renderTarget->DrawBitmap(scoreNineBitmap, rect);
                break;
            }
        }
        else {
            moonsweeper->game.flagsLeft < 0 ? renderTarget->DrawBitmap(scoreMinusBitmap, rect) : renderTarget->DrawBitmap(scoreZeroBitmap, rect);
        }

        
        // Second number
        rect.left = (moonsweeper->game.difficulty == Game::DIFFICULTY::BEGINNER ? (secondNumX - 1 )* moonsweeper->game.tileSize : secondNumX * moonsweeper->game.tileSize) - 2;  // floating point precision error correction (I think?)
        rect.right = rect.left + moonsweeper->game.tileSize;

        teno = (moonsweeper->game.flagsLeft - hundo * 100) / 10;
        switch (teno) {
        case 0:
            renderTarget->DrawBitmap(scoreZeroBitmap, rect);
            break;
        case 1:
            renderTarget->DrawBitmap(scoreOneBitmap, rect);
            break;
        case 2:
            renderTarget->DrawBitmap(scoreTwoBitmap, rect);
            break;
        case 3:
            renderTarget->DrawBitmap(scoreThreeBitmap, rect);
            break;
        case 4:
            renderTarget->DrawBitmap(scoreFourBitmap, rect);
            break;
        case 5:
            renderTarget->DrawBitmap(scoreFiveBitmap, rect);
            break;
        case 6:
            renderTarget->DrawBitmap(scoreSixBitmap, rect);
            break;
        case 7:
            renderTarget->DrawBitmap(scoreSevenBitmap, rect);
            break;
        case 8:
            renderTarget->DrawBitmap(scoreEightBitmap, rect);
            break;
        case 9:
            renderTarget->DrawBitmap(scoreNineBitmap, rect);
            break;
        }

        
        // Third number
        rect.left = (moonsweeper->game.difficulty == Game::DIFFICULTY::BEGINNER ? (thirdNumX - 1) * moonsweeper->game.tileSize : thirdNumX * moonsweeper->game.tileSize) - 2;   // ''
        rect.right = rect.left + moonsweeper->game.tileSize;
        uno = (moonsweeper->game.flagsLeft - hundo * 100 - teno * 10) % 10;
        switch (uno) {
        case 0:
            renderTarget->DrawBitmap(scoreZeroBitmap, rect);
            break;
        case 1:
            renderTarget->DrawBitmap(scoreOneBitmap, rect);
            break;
        case 2:
            renderTarget->DrawBitmap(scoreTwoBitmap, rect);
            break;
        case 3:
            renderTarget->DrawBitmap(scoreThreeBitmap, rect);
            break;
        case 4:
            renderTarget->DrawBitmap(scoreFourBitmap, rect);
            break;
        case 5:
            renderTarget->DrawBitmap(scoreFiveBitmap, rect);
            break;
        case 6:
            renderTarget->DrawBitmap(scoreSixBitmap, rect);
            break;
        case 7:
            renderTarget->DrawBitmap(scoreSevenBitmap, rect);
            break;
        case 8:
            renderTarget->DrawBitmap(scoreEightBitmap, rect);
            break;
        case 9:
            renderTarget->DrawBitmap(scoreNineBitmap, rect);
            break;
        }


        // Timer scoreboard
        // First number
        rect.left = (moonsweeper->game.difficulty == Game::DIFFICULTY::BEGINNER ? (moonsweeper->tileWCount - thirdNumX) * moonsweeper->game.tileSize : (moonsweeper->tileWCount - thirdNumX - 1) * moonsweeper->game.tileSize);
        rect.right = rect.left + moonsweeper->game.tileSize;
        hundo = moonsweeper->timer / 100;

        switch (hundo) {
        case 0:
            renderTarget->DrawBitmap(scoreZeroBitmap, rect);
            break;
        case 1:
            renderTarget->DrawBitmap(scoreOneBitmap, rect);
            break;
        case 2:
            renderTarget->DrawBitmap(scoreTwoBitmap, rect);
            break;
        case 3:
            renderTarget->DrawBitmap(scoreThreeBitmap, rect);
            break;
        case 4:
            renderTarget->DrawBitmap(scoreFourBitmap, rect);
            break;
        case 5:
            renderTarget->DrawBitmap(scoreFiveBitmap, rect);
            break;
        case 6:
            renderTarget->DrawBitmap(scoreSixBitmap, rect);
            break;
        case 7:
            renderTarget->DrawBitmap(scoreSevenBitmap, rect);
            break;
        case 8:
            renderTarget->DrawBitmap(scoreEightBitmap, rect);
            break;
        case 9:
            renderTarget->DrawBitmap(scoreNineBitmap, rect);
            break;
        }

        // Second Number
        rect.left = (moonsweeper->game.difficulty == Game::DIFFICULTY::BEGINNER ? (moonsweeper->tileWCount - secondNumX) * moonsweeper->game.tileSize : (moonsweeper->tileWCount - secondNumX - 1) * moonsweeper->game.tileSize) - 2;
        rect.right = rect.left + moonsweeper->game.tileSize;

        teno = (moonsweeper->timer - hundo * 100) / 10;
        switch (teno) {
        case 0:
            renderTarget->DrawBitmap(scoreZeroBitmap, rect);
            break;
        case 1:
            renderTarget->DrawBitmap(scoreOneBitmap, rect);
            break;
        case 2:
            renderTarget->DrawBitmap(scoreTwoBitmap, rect);
            break;
        case 3:
            renderTarget->DrawBitmap(scoreThreeBitmap, rect);
            break;
        case 4:
            renderTarget->DrawBitmap(scoreFourBitmap, rect);
            break;
        case 5:
            renderTarget->DrawBitmap(scoreFiveBitmap, rect);
            break;
        case 6:
            renderTarget->DrawBitmap(scoreSixBitmap, rect);
            break;
        case 7:
            renderTarget->DrawBitmap(scoreSevenBitmap, rect);
            break;
        case 8:
            renderTarget->DrawBitmap(scoreEightBitmap, rect);
            break;
        case 9:
            renderTarget->DrawBitmap(scoreNineBitmap, rect);
            break;
        }

        // Third Number
        rect.left = (moonsweeper->game.difficulty == Game::DIFFICULTY::BEGINNER ? (moonsweeper->tileWCount - firstNumX) * moonsweeper->game.tileSize : (moonsweeper->tileWCount - firstNumX - 1) * moonsweeper->game.tileSize) - 3;
        rect.right = rect.left + moonsweeper->game.tileSize;
        uno = (static_cast<int>(moonsweeper->timer) - hundo * 100 - teno * 10) % 10;

        switch (uno) {
        case 0:
            renderTarget->DrawBitmap(scoreZeroBitmap, rect);
            break;
        case 1:
            renderTarget->DrawBitmap(scoreOneBitmap, rect);
            break;
        case 2:
            renderTarget->DrawBitmap(scoreTwoBitmap, rect);
            break;
        case 3:
            renderTarget->DrawBitmap(scoreThreeBitmap, rect);
            break;
        case 4:
            renderTarget->DrawBitmap(scoreFourBitmap, rect);
            break;
        case 5:
            renderTarget->DrawBitmap(scoreFiveBitmap, rect);
            break;
        case 6:
            renderTarget->DrawBitmap(scoreSixBitmap, rect);
            break;
        case 7:
            renderTarget->DrawBitmap(scoreSevenBitmap, rect);
            break;
        case 8:
            renderTarget->DrawBitmap(scoreEightBitmap, rect);
            break;
        case 9:
            renderTarget->DrawBitmap(scoreNineBitmap, rect);
            break;
        }

        renderTarget->EndDraw();

        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }
        EndPaint(m_hwnd, &ps);
    }
}

void InfoWindow::Show() {
    ShowWindow(m_hwnd, SW_SHOW);
}

BOOL InfoWindow::Create(CREATEWNDSTRUCT& cws) {
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
    cws.hMenu = reinterpret_cast<HMENU>(CHILD_WINDOW_ID::INFO_WINDOW_ID);

    REGWNDSTRUCT rws;
    rws.lpszClassName = m_name;
    rws.hCursor = LoadCursor(NULL, IDC_ARROW);
    return BaseWindow::Create(cws, rws);
}

IWICImagingFactory* InfoWindow::GetIWICFactory() {
    return iwicFactory;
}

ID2D1HwndRenderTarget* InfoWindow::GetRenderTarget() {
    return renderTarget;
}