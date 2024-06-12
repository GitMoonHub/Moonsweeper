//#include <afxwin.h>
#include <iostream>
#include <string>
#include <windowsx.h>
#include "MainWindow.h"
#include "Win32_Helper.h"
#include "Moonsweeper.h"
#include "resource.h"

#define CUSTOM_SIZE_LIMIT 3

MainWindow::MainWindow(LPCWSTR name, u_int width, u_int height, Moonsweeper* moonsweeperPtr) :
m_name(name),
width(width),
height(height),
factory(NULL),
renderTarget(NULL),
moonsweeper(moonsweeperPtr) {
    
}

MainWindow::MainWindow() {

}

MainWindow::~MainWindow() {

}


LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HRESULT hr = S_OK;
    HRSRC hrsrc = NULL;
    switch (uMsg) {

        case WM_LBUTTONUP:
            moonsweeper->smileyWindow.state = DEFAULT;
            SendMessage(moonsweeper->smileyWindow.Window(), WM_PAINT, 0, 0);
            if (wParam & MK_LBUTTON) {
                break;
            }
            return 0;

        case WM_CREATE: {
            hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
            if (FAILED(hr)) {
                std::cout << "D2D1CreateFactory() failed. HRESULT = " << hr << "\n";
                exit(-1);
            }

            hr = CreateGraphicsResources();
            std::cout << "[MainWindow]: Successfully created the window.\n";
            Win32_Helper::DPIScale::Initialize(this->Window());
           /* hrsrc = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OPTIONS), MAKEINTRESOURCE(IDD_OPTIONS));
            if (hrsrc == NULL) {
                std::cout << "Couldn't find IDD_OPTIONS resource.\n";
                exit(-1);
            }*/

            //LoadResource(GetModuleHandle(NULL), hrsrc);
            //LockResource()
            

            return 0;

        case WM_DESTROY:
            DiscardGraphicsResources();
            Win32_Helper::SafeRelease(&factory);
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            OnPaint();
            if (moonsweeper->gameWindow.Window() != nullptr) {
                SendMessage(moonsweeper->gameWindow.Window(), WM_PAINT, NULL, NULL);
            }

            if (moonsweeper->infoWindow.Window() != nullptr) {
                SendMessage(moonsweeper->infoWindow.Window(), WM_PAINT, NULL, NULL);
            }

            if (moonsweeper->smileyWindow.Window() != nullptr) {
                SendMessage(moonsweeper->smileyWindow.Window(), WM_PAINT, NULL, NULL);
            }
            return 0;

        case WM_SIZE:
            Resize();
            return 0;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
            case IDM_OPTIONS:
                DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OPTIONS), m_hwnd, OptionsMessage, reinterpret_cast<LPARAM>(this));
                return 0;
            }
        }
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

INT_PTR CALLBACK MainWindow::OptionsMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    int code = NULL;
    int id = NULL;
    int value = NULL;
    BOOL success = FALSE;
    static MainWindow* main = nullptr;
    static Moonsweeper* ms = nullptr;
    static Game::DIFFICULTY difficulty = Game::DIFFICULTY::BEGINNER;

    // Get pointer to singleton object
    if (ms == nullptr && main != nullptr) {
        ms = main->GetMoonsweeper();
    }

    switch (uMsg) {
    case WM_INITDIALOG:
        std::cout << "WM_INITDIALOG.\n";
        if (reinterpret_cast<void*>(lParam) == nullptr) {
            std::cout << "[OPTIONS]: Pointer to main window (lParam) was null.\n";
            return FALSE;
        }
        main = reinterpret_cast<MainWindow*>(lParam);
        main->hDlgOptions = hwnd;
        main->hBtnOk = GetDlgItem(hwnd, IDC_BUTTON_OK);
        main->hBtnCancel = GetDlgItem(hwnd, IDC_BUTTON_CANCEL);
        main->hComboBoxDiff = GetDlgItem(hwnd, IDC_COMBO_DIFFICULTY);
        main->hEditSeed = GetDlgItem(hwnd, IDC_EDIT_SEED);
        main->hEditWidth = GetDlgItem(hwnd, IDC_EDIT_WIDTH);
        main->hEditHeight = GetDlgItem(hwnd, IDC_EDIT_HEIGHT);
        main->hEditMine = GetDlgItem(hwnd, IDC_EDIT_MINE);
        main->hStaticSeed = GetDlgItem(hwnd, IDC_STATIC_SEED);
        ms = main->GetMoonsweeper();

        difficulty = ms->game.difficulty;

        // Set the edit texts to their current respective values
        Edit_SetText(main->hEditWidth, std::to_wstring(ms->game.width).c_str());
        Edit_SetText(main->hEditHeight, std::to_wstring(ms->game.height).c_str());
        Edit_SetText(main->hEditMine, std::to_wstring(ms->game.mineCount).c_str());

        // Limit the possible size to < 1000
        Edit_LimitText(main->hEditWidth, CUSTOM_SIZE_LIMIT);
        Edit_LimitText(main->hEditHeight, CUSTOM_SIZE_LIMIT);
        
        
        // Add the combobox items
        ComboBox_AddString(main->hComboBoxDiff, L"Beginner");
        ComboBox_AddString(main->hComboBoxDiff, L"Intermediate");
        ComboBox_AddString(main->hComboBoxDiff, L"Expert");
        ComboBox_AddString(main->hComboBoxDiff, L"Custom");
        //DialogInit

        switch (ms->game.difficulty) {
            case Game::DIFFICULTY::BEGINNER:
                ComboBox_SetCurSel(main->hComboBoxDiff, 0);
                break;
            case Game::DIFFICULTY::INTERMEDIATE:
                ComboBox_SetCurSel(main->hComboBoxDiff, 1);
                break;
            case Game::DIFFICULTY::EXPERT:
                ComboBox_SetCurSel(main->hComboBoxDiff, 2);
                break;
            case Game::DIFFICULTY::CUSTOM:
                ComboBox_SetCurSel(main->hComboBoxDiff, 3);
                break;
        }

        if (ms->game.seed != NULL) {
            Static_SetText(main->hStaticSeed, std::to_wstring(ms->game.seed).c_str());
            Edit_SetText(main->hEditSeed, std::to_wstring(ms->game.seed).c_str());
        }
        else {
            Static_SetText(main->hStaticSeed, L"Random");
        }

        return TRUE;

    case WM_COMMAND:
        id = LOWORD(wParam);
        code = HIWORD(wParam);
        switch (id) {
        case IDC_BUTTON_OK:
            switch (code) {
                case BN_CLICKED:
                    
                    // Check seed
                    value = GetDlgItemInt(main->hDlgOptions, IDC_EDIT_SEED, &success, FALSE);
                    if (success) {
                        ms->game.seed = value;
                        ms->game.NewGame();
                    }
                    else {
                        ms->game.seed = NULL;
                    }
                    
                    // Check custom diff
                    value = ComboBox_GetCurSel(main->hComboBoxDiff);
                    if (value != CB_ERR) {
                        switch (static_cast<Game::DIFFICULTY>(value)) {
                        case Game::DIFFICULTY::BEGINNER:
                            ms->game.difficulty = Game::DIFFICULTY::BEGINNER;
                            ms->game.width = 9;
                            ms->game.height = 9;
                            ms->game.mineCount = 10;
                            break;
                        case Game::DIFFICULTY::INTERMEDIATE:
                            ms->game.difficulty = Game::DIFFICULTY::INTERMEDIATE;
                            ms->game.width = 16;
                            ms->game.height = 16;
                            ms->game.mineCount = 40;
                            break;
                        case Game::DIFFICULTY::EXPERT:
                            ms->game.difficulty = Game::DIFFICULTY::EXPERT;
                            ms->game.width = 30;
                            ms->game.height = 16;
                            ms->game.mineCount = 99;
                            break;
                        case Game::DIFFICULTY::CUSTOM:
                            ms->game.difficulty = Game::DIFFICULTY::CUSTOM;
                            Edit_Enable(main->hEditHeight, TRUE);
                            Edit_Enable(main->hEditWidth, TRUE);
                            Edit_Enable(main->hEditMine, TRUE);
                            
                            int strLen = Edit_GetTextLength(main->hEditMine) + 1;
                            if (strLen < CUSTOM_SIZE_LIMIT + 1) {
                                strLen = CUSTOM_SIZE_LIMIT + 1;
                            }
                            wchar_t* buffer = new wchar_t[strLen];
                            try {
                                Edit_GetText(main->hEditMine, buffer, strLen);
                                ms->game.mineCount = std::stoi(buffer);
                                Edit_GetText(main->hEditWidth, buffer, CUSTOM_SIZE_LIMIT + 1);
                                ms->game.width = std::stoi(buffer);
                                Edit_GetText(main->hEditHeight, buffer, CUSTOM_SIZE_LIMIT + 1);
                                ms->game.height = std::stoi(buffer);
                            }
                            catch (std::invalid_argument const& e) {
                                ms->game.mineCount = 99;
                                ms->game.width = 30;
                                ms->game.height = 16;
                                ms->game.difficulty = Game::DIFFICULTY::EXPERT;
                            }
                            delete[] buffer;

                            break;
                        }
                        // Check if we changed the difficulty, if so, generate a new game
                        if (static_cast<Game::DIFFICULTY>(value) != difficulty) {
                            ms->game.NewGame();
                            ms->CalculateLayout();
                            //difficulty = ms->game.difficulty;
                            //SendMessage(ms->gameWindow.Window(), WM_PAINT, NULL, NULL);
                        }
                        SendMessage(ms->gameWindow.Window(), WM_PAINT, NULL, NULL);
                    }
                    break;
            }
            // Fall through
        case IDC_BUTTON_CANCEL:
            if (main != nullptr) {
                main->hDlgOptions = nullptr;
                main->hBtnOk = nullptr;
                main->hBtnCancel = nullptr;
                main->hComboBoxDiff = nullptr;
                main->hEditSeed = nullptr;
                main->hEditWidth = nullptr;
                main->hEditHeight = nullptr;
                main->hEditMine = nullptr;
                main->hStaticSeed = nullptr;
                main = nullptr;
            }
            EndDialog(hwnd, NULL);
            return TRUE;

        case IDC_COMBO_DIFFICULTY:
            switch (code) {
            case CBN_SELCHANGE:
                value = ComboBox_GetCurSel(main->hComboBoxDiff);

                // Disable the edits
                Edit_Enable(main->hEditHeight, FALSE);
                Edit_Enable(main->hEditWidth, FALSE);
                Edit_Enable(main->hEditMine, FALSE);

                switch (static_cast<Game::DIFFICULTY>(value)) {
                case Game::DIFFICULTY::BEGINNER:
                    Edit_SetText(main->hEditWidth, L"9");
                    Edit_SetText(main->hEditHeight, L"9");
                    Edit_SetText(main->hEditMine, L"10");
                    break;
                case Game::DIFFICULTY::INTERMEDIATE:
                    Edit_SetText(main->hEditWidth, L"16");
                    Edit_SetText(main->hEditHeight, L"16");
                    Edit_SetText(main->hEditMine, L"40");
                    break;
                case Game::DIFFICULTY::EXPERT:
                    Edit_SetText(main->hEditWidth, L"30");
                    Edit_SetText(main->hEditHeight, L"16");
                    Edit_SetText(main->hEditMine, L"99");
                    break;
                case Game::DIFFICULTY::CUSTOM:
                    Edit_Enable(main->hEditHeight, TRUE);
                    Edit_Enable(main->hEditWidth, TRUE);
                    Edit_Enable(main->hEditMine, TRUE);
                    Edit_SetText(main->hEditWidth, L"");
                    Edit_SetText(main->hEditHeight, L"");
                    Edit_SetText(main->hEditMine, L"");
                    break;
                }

                return TRUE;
            }
        }
        return TRUE;

    case WM_CLOSE:

        EndDialog(hwnd, NULL);
        return TRUE;
    }

    return FALSE;
}

HRESULT MainWindow::CreateGraphicsResources() {
    HRESULT hr = S_OK;
    // Check if there is no render target
    if (renderTarget == NULL) {
        RECT rc;
        GetClientRect(m_hwnd, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        // Create render target
        hr = factory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &renderTarget);
        if(FAILED(hr)) {
            std::cout << "[MainWindow]: RenderTarget creation failed.\n";
            exit(-1);
        }
    }
    return hr;
}

void MainWindow::DiscardGraphicsResources() {
    using namespace Win32_Helper;
    SafeRelease(&renderTarget);
}

void MainWindow::OnPaint() {
    HRESULT hr = CreateGraphicsResources();

    if (SUCCEEDED(hr)) {
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);

        renderTarget->BeginDraw();
        renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::WhiteSmoke));

        hr = renderTarget->EndDraw();

        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }
        EndPaint(m_hwnd, &ps);
    }
}

void MainWindow::Resize() {
    if (renderTarget != NULL) {
        RECT rc;
        GetClientRect(m_hwnd, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
        renderTarget->Resize(size);
        moonsweeper->CalculateLayout();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

BOOL MainWindow::Create(CREATEWNDSTRUCT& cws) {

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
    HMENU hMenu = CreateMenu();
    AppendMenu(hMenu, MF_STRING, 1, TEXT("Options"));
    cws.hMenu = hMenu;
    cws.lpWindowName = L"Moonsweeper";
    REGWNDSTRUCT rws;
    rws.lpszClassName = m_name;
    rws.hCursor = LoadCursor(NULL, IDC_ARROW);    
    return BaseWindow::Create(cws, rws);
}

Moonsweeper* MainWindow::GetMoonsweeper() {
    return this->moonsweeper;
}