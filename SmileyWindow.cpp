#include <iostream>
#include "SmileyWindow.h"
#include "Win32_Helper.h"
#include "Moonsweeper.h"
#include "MoonsweeperTypedef.h"

SmileyWindow::SmileyWindow(HWND parentHwnd, Moonsweeper* moonsweeperPtr) :
    parentHwnd(parentHwnd),
    moonsweeper(moonsweeperPtr) {

}
SmileyWindow::SmileyWindow() {

}

SmileyWindow::~SmileyWindow() {

}


LRESULT SmileyWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HRESULT hr = S_OK;
    MOUSE_BUTTONS_STATE mbs = LEFTCLICK;
    switch (uMsg) {
        case WM_CREATE:
            hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
            if (FAILED(hr)) {
                std::cout << "[SmileyWindow]: D2D1CreateFactory() failed. HRESULT = " << hr << "\n";
                exit(-1);
            }
            hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), reinterpret_cast<void**>(&iwicFactory));
            if (FAILED(hr)) {
                std::cout << "[SmileyWindow]: Couldn't create a IWICImagingFactory instance. HRESULT = " << hr << "\n";
                exit(-1);
            }
            hr = CreateGraphicsResources();
            std::cout << "[SmileyWindow]: Successfully created the window.\n";
            return 0;

        case WM_DESTROY:
            DiscardGraphicsResources();
            Win32_Helper::SafeRelease(&factory);
            Win32_Helper::SafeRelease(&iwicFactory);
            return 0;

        case WM_LBUTTONDOWN:
            state = PUSHED;
            SendMessage(m_hwnd, WM_PAINT, 0, 0);
            return 0;

        case WM_LBUTTONUP:
            moonsweeper->timer = 0;
            moonsweeper->neverClicked = true;
            state = DEFAULT;
            moonsweeper->game.NewGame();
            SendMessage(moonsweeper->mainWindow.Window(), WM_PAINT, 0, 0);
            return 0;

        case WM_PAINT:
            OnPaint();
            return 0;
    }
    return DefWindowProc(this->Window(), uMsg, wParam, lParam);
}

HRESULT SmileyWindow::CreateGraphicsResources() {
    HRESULT hr = S_OK;
    // Check if there is no render target
    if (renderTarget == NULL) {

        D2D1_SIZE_U rtSize = D2D1::SizeU(this->width, this->height);
        // Create render target
        hr = factory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, rtSize),
            &renderTarget);
        if (FAILED(hr)) {
            std::cout << "[SmileyWindow]: HwndRenderTarget creation failed.\n";
            exit(-1);
        }

        // Bitmaps
        hr = Win32_Helper::LoadBitmapFromFile<SmileyWindow>(this, L"assets/Smiley.png", &smileyBitmap);
        if (FAILED(hr)) {
            std::cout << "[SmileyWindow]: SmileyBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<SmileyWindow>(this, L"assets/SmileyCool.png", &coolBitmap);
        if (FAILED(hr)) {
            std::cout << "[SmileyWindow]: CoolBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<SmileyWindow>(this, L"assets/SmileyDead.png", &deadBitmap);
        if (FAILED(hr)) {
            std::cout << "[SmileyWindow]: DeadBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<SmileyWindow>(this, L"assets/SmileyPushed.png", &pushedBitmap);
        if (FAILED(hr)) {
            std::cout << "[SmileyWindow]: PushedBitmap creation failed.\n";
        }

        hr = Win32_Helper::LoadBitmapFromFile<SmileyWindow>(this, L"assets/SmileyShocked.png", &shockedBitmap);
        if (FAILED(hr)) {
            std::cout << "[SmileyWindow]: ShockedBitmap creation failed.\n";
        }
    }

    return hr;
}

void SmileyWindow::DiscardGraphicsResources() {
    using namespace Win32_Helper;

    //SafeRelease(&factory);
    SafeRelease(&renderTarget);
    //SafeRelease(&iwicFactory);
    SafeRelease(&smileyBitmap);
    SafeRelease(&coolBitmap);
    SafeRelease(&shockedBitmap);
    SafeRelease(&deadBitmap);
    SafeRelease(&pushedBitmap);

}

void SmileyWindow::OnPaint() {
    HRESULT hr = CreateGraphicsResources();

    if (SUCCEEDED(hr)) {
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);
        D2D1_RECT_F rc = D2D1::RectF(0.0f, 0.0f, this->width, this->height);
        renderTarget->BeginDraw();
        renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Gold));

        switch (state) {
            case DEFAULT:
                renderTarget->DrawBitmap(smileyBitmap, &rc);
                break;
            case PUSHED:
                renderTarget->DrawBitmap(pushedBitmap, &rc);
                break;
            case DEAD:
                renderTarget->DrawBitmap(deadBitmap, &rc);
                break;
            case COOL:
                renderTarget->DrawBitmap(coolBitmap, &rc);
                break;
            case SHOCKED:
                renderTarget->DrawBitmap(shockedBitmap, &rc);
                break;
        }
        
        renderTarget->EndDraw();

        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }
    }
}

void SmileyWindow::Show() {
    ShowWindow(m_hwnd, SW_SHOW);
}

BOOL SmileyWindow::Create(CREATEWNDSTRUCT& cws) {
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
    cws.hMenu = reinterpret_cast<HMENU>(CHILD_WINDOW_ID::SMILEY_WINDOW_ID);

    REGWNDSTRUCT rws;
    rws.lpszClassName = m_name;
    rws.hCursor = LoadCursor(NULL, IDC_ARROW);
    return BaseWindow::Create(cws, rws);
}

IWICImagingFactory* SmileyWindow::GetIWICFactory() {
    return this->iwicFactory;
}

ID2D1HwndRenderTarget* SmileyWindow::GetRenderTarget() {
    return this->renderTarget;
}