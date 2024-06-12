#ifndef UNICODE
#define UNICODE
#endif 
//#include <afxwin.h>
#include <windows.h>
#include <iostream>
#include <profileapi.h>
#include "Moonsweeper.h"
#include "Win32_Helper.h"

u_int g_WIDTH = 800;
u_int g_HEIGHT = 600;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    AllocConsole();
    Win32_Helper::BindCrtHandlesToStdHandles(false, true, true);

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        std::cout << "Failed to initialize the COM library.\n";
        exit(-1);
    }

    Moonsweeper ms = Moonsweeper(g_WIDTH, g_HEIGHT, nCmdShow);

    ms.Run();
    CoUninitialize();
    return 0;
}