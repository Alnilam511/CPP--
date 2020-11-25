#define WIN32_LEAN_AND_MEAN           

#include <windows.h>

#include "SimpleRibbon.h"
#include "RibbonFramework.h"

#include "GlobalVariable.h"

//#include "CommandHandler.h"
#include "SimpleRibbonUI.h"
#include <stack.h>
#define MAX_LOADSTRING 100

int a;
TCHAR szBuffer[MAX_PATH];

HINSTANCE hInst;                                
WCHAR wszTitle[MAX_LOADSTRING];                 
WCHAR wszWindowClass[MAX_LOADSTRING];          

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
ULONG_PTR m_gdiplusToken;
POINT pt;
POINT ptt;
BOOL ismove=FALSE;
int k, l,m;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        return FALSE;
    }


    // Initialize global strings.
    LoadString(hInstance, IDS_APP_TITLE, wszTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_SIMPLERIBBON, wszWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    // Main message loop.
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CoUninitialize();

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application has 'well formed' small icons associated
//    with it.
//
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = 0;    // Don't use CS_HREDRAW or CS_VREDRAW with a Ribbon
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SIMPLERIBBON));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = wszWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window.
//
//   COMMENTS:
//
//        In this function, an instance handle is saved in a global variable and
//        create and display the main program window.
//
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;

    hInst = hInstance; // Store instance handle in our global variable.

    hWnd = CreateWindow(wszWindowClass, L"TEST No.1", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu.
//  WM_PAINT    - Paint the main window.
//  WM_DESTROY  - post a quit message and return.
//
//

#define btn1 1
#define btn2 2

void Drawlinemk2(HWND hWnd, POINT pt1, POINT ptt1)
{
    HDC hdc = GetDC(hWnd);
    GetClientRect(hWnd, &rect);
    k = l = 0;
    long i, j;
    int n=0;
    i= min(pt1.x, ptt1.x);
    j= max(pt1.x, ptt1.x);
    rect.left = i;
    rect.right = j;
    HBRUSH hbrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &rect, hbrush);
    DeleteObject(hbrush);
    while (i >= aptt[k].x)
    {
        k++;
    }
    l = k;
    while (j >= aptt[l].x)
    {
        l++;
    }
    m = l - k + 1;
    POINT *apt1;
    apt1 = new POINT[m];
    
    while (n<=m)
    {
        apt1[n] = aptt[k];
        n++;
        k++;
    }
    Polyline(hdc, apt1, n);
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //int wmId, wmEvent;    
    PAINTSTRUCT ps;
    HDC hdc;
    HDC hdcStatic;
    static HFONT hFont;
    static HWND hStatic1;
    static HWND hStatic2;
    static HWND hStatic3;
    bool initSuccess;

    switch (message)
    {
    case WM_CREATE:
        // Initializes the Ribbon framework.
        initSuccess = InitializeFramework(hWnd);
        if (!initSuccess) 
        {
            return -1;
        }

        hFont = CreateFont(-14, -7, 0, 0, 400 ,
            FALSE/*б��?*/, FALSE/*�»���?*/, FALSE/*ɾ����?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("΢���ź�"));

        hStatic3 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            15, 198, 153, 184, hWnd, (HMENU)3, NULL, NULL);

        hStatic1 = CreateWindow(TEXT("STATIC"), TEXT("���ݽ��"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | SS_CENTER,
            20, 200, 143, 30, hWnd, (HMENU)btn1, NULL, NULL);

        hStatic2 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE ,
            20, 230, 143, 150, hWnd, (HMENU)btn2, NULL, NULL);

        hStatic4 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE ,
            200, 170, 900, 80, hWnd, (HMENU)3, NULL, NULL);

        hStatic5 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE ,
            200, 430, 900, 80, hWnd, (HMENU)3, NULL, NULL);

        hStatic6 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE,
            200, 250, 900, 180, hWnd, (HMENU)3, NULL, NULL);

        hStatic7 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE,
            200, 510, 900, 180, hWnd, (HMENU)3, NULL, NULL);

        SendMessage(hStatic1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(hStatic2, WM_SETFONT, (WPARAM)hFont, NULL);



    case WM_CTLCOLORSTATIC: {
        hdcStatic = (HDC)wParam;
        SetBkMode(hdcStatic, TRANSPARENT);
    }
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_LBUTTONDOWN: {
        GetCursorPos(&pt);
        GetWindowRect(hStatic4, &rect);
        if (PtInRect(&rect, pt))
        {
            ScreenToClient(hStatic4, &pt);
            ismove = TRUE;
        }
        else {
            ismove = FALSE;
        }
        break;
    }

    case WM_LBUTTONUP: {
        if (ismove == TRUE)
        {

        }
        ismove = FALSE;
        break;
    }

    case WM_MOUSEMOVE: {
        if (ismove == TRUE)
        {
            GetCursorPos(&ptt);
            ScreenToClient(hStatic4, &ptt);
            DrawLine(hStatic4, gnum, gn);
            Drawlinemk2(hStatic4, pt, ptt);
        }        
        break;
    }
        
    case WM_DESTROY:

        DeleteObject(hFont);

        // Tears down the Ribbon framework.
        DestroyFramework();
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    switch (a)
    {
    case cmdButton3:
    case cmdButton4:
    case cmdButton5:
    case cmdButton6:
    case cmdButton7:
    case cmdButton8:
    case cmdButton9:
    case cmdButton10:
    case cmdButton11:
    case cmdButton12:
    case cmdButton13:
    case cmdButton14:
    case cmdButton15:
    case cmdButton16:
        SetWindowText(hStatic2, szBuffer);
        break;
    }

    return 0;
}

