#include "Moonsweeper.h"

Moonsweeper::Moonsweeper(u_int width, u_int height) {
	/*mainWindow = MainWindow(L"Moonsweeper", width, height);
	CREATEWNDSTRUCT cws;
	cws.dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
	cws.nWidth = CW_USEDEFAULT;
	cws.nHeight = CW_USEDEFAULT;
	cws.x = CW_USEDEFAULT;
	cws.y = CW_USEDEFAULT;
    if (!mainWindow.Create(cws)) {
        std::cout << "Couldn't create the MainWindow\n.";
        exit(-1);
    }
    ShowWindow(mainWindow.Window(), 1);


	cws = CREATEWNDSTRUCT();
	cws.dwExStyle = 0;
    cws.dwStyle = WS_CHILD | WS_CLIPSIBLINGS;
    cws.x = CW_USEDEFAULT;
    cws.y = CW_USEDEFAULT;
    cws.nWidth = CW_USEDEFAULT;
    cws.nHeight = CW_USEDEFAULT;
	gameWindow = GameWindow(mainWindow.Window(), &mainWindow);
    gameWindow.Show();
	game = Game();*/
}

Moonsweeper::Moonsweeper(u_int width, u_int height, int nCmdShow) {
    mainWindow = MainWindow(L"Moonsweeper", width, height, this);
    CREATEWNDSTRUCT cws;
    cws.dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
    cws.nWidth = CW_USEDEFAULT;
    cws.nHeight = CW_USEDEFAULT;
    cws.x = CW_USEDEFAULT;
    cws.y = CW_USEDEFAULT;
    if (!mainWindow.Create(cws)) {
        std::cout << "[Moonsweeper]: Couldn't create mainWindow\n.";
        exit(-1);
    }
    ShowWindow(mainWindow.Window(), nCmdShow);

    game = Game(Game::DIFFICULTY::EXPERT, 30, 16, 99, NULL);
    //game = Game(Game::DIFFICULTY::CUSTOM, 30, 16, 1, 1501802516);
    //game.seed = /*1501802516*/;

    float tileHCount = game.height + infoWindow.heightTiles + hBorderCount;
    float tileWCount = game.width + vBorderCount;
    float tileHSize = mainWindow.height / tileHCount;
    float tileWSize = mainWindow.width / tileWCount;

    game.tileSize = tileHSize > tileWSize ? tileWSize : tileHSize;

    cws = CREATEWNDSTRUCT();
    cws.dwExStyle = 0;
    cws.dwStyle = WS_CHILD | WS_CLIPSIBLINGS;
    cws.x = CW_USEDEFAULT;
    cws.y = CW_USEDEFAULT;
    cws.nWidth = CW_USEDEFAULT;
    cws.nHeight = CW_USEDEFAULT;
    cws.hWndParent = mainWindow.Window();

    HRESULT hr = S_OK;
    gameWindow = GameWindow(mainWindow.Window(), this);
    gameWindow.width = game.tileSize * game.width;
    gameWindow.height = game.tileSize * game.height;
    cws.nWidth = gameWindow.width;
    cws.nHeight = gameWindow.height;
    if (!gameWindow.Create(cws)) {
        hr = HRESULT_FROM_WIN32(GetLastError());
        std::cout << "[Moonsweeper]: Couldn't create gameWindow.\n";
        exit(-1);
    }
    //ShowWindow(gameWindow.Window(), 1);
    gameWindow.Show();
    //mainWindow.SetChildPointers(gameWindow.Window());
    // InfoWindow

    cws = CREATEWNDSTRUCT();
    cws.dwExStyle = 0;
    cws.dwStyle = WS_CHILD | WS_CLIPSIBLINGS;
    cws.x = CW_USEDEFAULT;
    cws.y = CW_USEDEFAULT;
    cws.nWidth = gameWindow.width;
    cws.nHeight = game.tileSize * 2;
    cws.hWndParent = mainWindow.Window();
    infoWindow = InfoWindow(mainWindow.Window(), this);
    if (!infoWindow.Create(cws)) {
        std::cout << "[Moonsweeper]: Couldn't create infoWindow.\n";
        exit(-1);
    }
    infoWindow.Show();

    // SmileyWindow
    cws = CREATEWNDSTRUCT();
    cws.dwExStyle = 0;
    cws.dwStyle = WS_CHILD | WS_CLIPSIBLINGS;
    cws.x = CW_USEDEFAULT;
    cws.y = CW_USEDEFAULT;
    cws.nWidth = game.tileSize * 2;
    cws.nHeight = game.tileSize * 2;
    cws.hWndParent = mainWindow.Window();

    smileyWindow = SmileyWindow(mainWindow.Window(), this);
    if (!smileyWindow.Create(cws)) {
        std::cout << "[Moonsweeper]: Couldn't create smileyWindow.\n";
        exit(-1);
    }
    smileyWindow.Show();

    CalculateLayout();
}

Moonsweeper::~Moonsweeper() {

}

void Moonsweeper::Run() {

    MSG msg = { };

    LARGE_INTEGER last_time;
    QueryPerformanceCounter(&last_time);
    LARGE_INTEGER current_time;
    LARGE_INTEGER timer_freq;
    QueryPerformanceFrequency(&timer_freq);
    std::cout << timer_freq.QuadPart << std::endl;
    double d_ltime = static_cast<double>(last_time.QuadPart);
    const double d_freq = static_cast<double>(timer_freq.QuadPart);
    //double d_frametime_limit = 1.0 / 60.0;

    while (true) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        QueryPerformanceCounter(&current_time);
        double d_ctime = static_cast<double>(current_time.QuadPart);
        double d_dtime = d_ctime - d_ltime;

        d_dtime /= d_freq;
        if (d_dtime >= 1.0f) {
            PostMessage(mainWindow.Window(), WM_PAINT, NULL, NULL);
            if (!neverClicked  && game.gameActive) {
                timer += d_dtime;
            }
            d_ltime = d_ctime + (d_dtime - 1.0f);
        }

    }
}

void Moonsweeper::CalculateLayout() {
    RECT rc;
    GetClientRect(mainWindow.Window(), &rc);
    D2D1_SIZE_F mwSize = D2D1::SizeF(rc.right, rc.bottom);

    // Find the various size of the windows
    int tileHCount = game.height + infoWindow.heightTiles + hBorderCount;
    tileWCount = game.width + vBorderCount;
    float tileHSize = mwSize.height / tileHCount;
    float tileWSize = mwSize.width / tileWCount;
    iWTotalHeight = infoWindow.heightTiles + 2; // Total height of infoWindow (2 out of 3 hBorders)

    ID2D1HwndRenderTarget* rt;

    game.tileSize = tileHSize > tileWSize ? tileWSize : tileHSize;

    // GameWindow positioning
    if (gameWindow.Window() != nullptr) {
        gameWindow.x = mwSize.width / 2.0f - (tileWCount * game.tileSize) / 2.0f;
        gameWindow.y = mwSize.height / 2.0f - (tileHCount * game.tileSize) / 2.0f + (iWTotalHeight * game.tileSize);
        gameWindow.width = tileWCount * game.tileSize;
        gameWindow.height = (tileHCount - iWTotalHeight) * game.tileSize;

        rt = gameWindow.GetRenderTarget();
        if (rt != nullptr) {
            rt->Resize(D2D1::SizeU(gameWindow.width, gameWindow.height));
        }

        SetWindowPos(
            gameWindow.Window(),
            NULL,
            gameWindow.x,
            gameWindow.y,
            gameWindow.width,
            gameWindow.height,
            0
        );
    }

    // InfoWindow positioning
    if (infoWindow.Window() != nullptr) {
        infoWindow.x = gameWindow.x;
        infoWindow.y = gameWindow.y - iWTotalHeight * game.tileSize;
        infoWindow.width = gameWindow.width;
        infoWindow.height = iWTotalHeight * game.tileSize;

        rt = infoWindow.GetRenderTarget();
        if (rt != nullptr) {
            rt->Resize(D2D1::SizeU(infoWindow.width, infoWindow.height));
        }
        infoWindow.posRect = D2D1::RectF(0, 0, infoWindow.width, infoWindow.height);
        SetWindowPos(
            infoWindow.Window(),
            NULL,
            infoWindow.x,
            infoWindow.y,
            infoWindow.width,
            infoWindow.height,
            0
        );
    }

    // SmileyWindow positioning
    if (smileyWindow.Window() != nullptr) {
        smileyWindow.x = infoWindow.x + (infoWindow.width / 2.0f - game.tileSize);
        smileyWindow.y = infoWindow.y + (infoWindow.height / 2.0f - game.tileSize);
        smileyWindow.width = 2 * game.tileSize;
        smileyWindow.height = smileyWindow.width;

        rt = smileyWindow.GetRenderTarget();
        if (rt != nullptr) {
            rt->Resize(D2D1::SizeU(smileyWindow.width, smileyWindow.height));
        }
        smileyWindow.posRect = D2D1::RectF(0, 0, smileyWindow.width, smileyWindow.height);

        SetWindowPos(
            smileyWindow.Window(),
            NULL,
            smileyWindow.x,
            smileyWindow.y,
            smileyWindow.width,
            smileyWindow.height,
            0
        );
    }

    if (mainWindow.Window() != nullptr) {
        SendMessage(mainWindow.Window(), WM_PAINT, 0, 0);
    }
}