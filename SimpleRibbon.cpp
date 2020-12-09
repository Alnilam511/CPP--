#define WIN32_LEAN_AND_MEAN           

#include <windows.h>

#include "SimpleRibbon.h"
#include "RibbonFramework.h"

#include "GlobalVariable.h"
#include "Functions.h"

//#include "CommandHandler.h"
#include "SimpleRibbonUI.h"
#include <stack.h>
#define MAX_LOADSTRING 100

int BtnNum;
POINT* apt1;
TCHAR szBuffer0[MAX_PATH];
TCHAR szBuffer[MAX_PATH];
TCHAR szBuffer1[MAX_PATH];
HWND hStatic8;
HWND hStatic2;

HINSTANCE hInst;                                
WCHAR wszTitle[MAX_LOADSTRING];                 
WCHAR wszWindowClass[MAX_LOADSTRING];          

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
ULONG_PTR m_gdiplusToken;
POINT pt;
POINT ptt;
POINT pt1;
POINT ptt1;
POINT pt0;
int ismove = 0;                                   // 点击位置判断，0窗口外，1在hstatic4，2在hstatic5

int CanvasStatus = 0;                  //画布状态判断 |  1  |  2  |  3  |  4  |  5  |  6  |    上为int数字，下为对应状态
                                       //             |  4  |  4' | 4,5 | 4',5| 4,5'|4',5'|    4：窗口4，5：窗口5，带 ' ：局部放大状态
int m;
HANDLE hOut;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    AllocConsole();
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
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

void Drawlinemk2(HWND hWnd, POINT ptx, POINT ptty)
{
    int k, l;
    HDC hdc = GetDC(hWnd);
    GetClientRect(hWnd, &rect);
    k = l = 0;
    long i, j;
    int n=0;
    if (ptty.x > 900)
    {
        ptty.x = 900;
    }
    if (ptty.x < 0)
    {
        ptty.x = 0;
    }
    if (ptty.y >80)
    {
        ptty.y = 80;
    }
    if (ptty.y < 0)
    {
        ptty.y = 0;
    }
    i= min(ptx.x, ptty.x);
    j= max(ptx.x, ptty.x);
    rect.left = i;
    rect.right = j;
    HBRUSH hbrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &rect, hbrush);
    DeleteObject(hbrush);
    POINT* apt0;
    apt0 = new POINT;
    double* input=NULL;
    if (ismove == 1)
    {
        apt0 = aptt;
        input = gnum;
    }
    else
    {
       if (ismove == 2)
       {
         apt0 = apt;        
         input = gnum1;

       }
    }
   
    while (i > apt0[k].x)
    {
        k++;
    }
    l = k;
    while (j > apt0[l].x)
    {
        l++;
    }
    m = l - k;

    double* part = GetPart(input, l, k);
    LPCTSTR str;
    memset(szBuffer1, '\0', sizeof(szBuffer1));
    switch (BtnNum)
    {
    case cmdButton3: 
    {
        double min = input[k], max = input[k];
        for (int aa = k; aa <= l; aa++)
        {
            min = Minimum(min, input[aa]);
            max = Maximum(max, input[aa]);
        }
        str = L" 局部数据个数为：\n   %i\n 局部峰值为:\n   %e\n   %e\n";
        swprintf_s(szBuffer1, str, m + 1, max, min);
        break;
    }        
    case cmdButton4:
    {
        double average = Average(part, m + 1);
        double variance = Variance(part, m + 1);
        str = L" 局部数据个数:\n   %i\n 局部均值:\n   %e\n 局部方差:\n   %e\n";
        swprintf_s(szBuffer1, str, m + 1, average, variance);
        break;
    }
    case cmdButton5:
    {
        double kurtosis = Kurtosis(part, m + 1);
        str = L" 局部数据个数:\n   %i\n 局部峭度:\n   %e\n";
        swprintf_s(szBuffer1, str, m + 1, kurtosis);
        break;
    }
    }

    delete[]apt1;
    apt1 = new POINT[m];

    while (n<m)
    {
        apt1[n] = apt0[k];
        n++;
        k++;
    }
    hpen = CreatePen(PS_SOLID, 1, RGB(255,127,0));                 //DarkOrange1
    SelectObject(hdc, hpen);
    Polyline(hdc, apt1, m);
    DeleteObject(hpen);
}



void Drawlinemk3(HWND hWnd){
    if (hWnd == hStatic4)
    {
        hWnd = hStatic6;
    }
    if (hWnd == hStatic5)
    {
        hWnd = hStatic7;
    }
    POINT* apt2;
    int k = 0;
    HDC hdc = GetDC(hWnd);
    GetClientRect(hWnd, &rect);
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    SetBkColor(hdc, RGB(255, 255, 255));
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
    hpen = CreatePen(PS_DASHDOT, 1, RGB(220, 220, 220));                   //Gainsboro
    SelectObject(hdc, hpen);
    int i = rect.bottom / 5;  int j = rect.right / 5;
    while (i < rect.bottom)
    {
        MoveToEx(hdc, 0, i, NULL);
        LineTo(hdc, rect.right, i);
        i = i + rect.bottom / 5;
    }
    while (j < rect.right)
    {
        MoveToEx(hdc, j, 0, NULL);
        LineTo(hdc, j, rect.bottom);
        j = j + rect.right / 5;
    }
    DeleteObject(hpen);
    hpen = CreatePen(PS_SOLID, 1, RGB(24, 116, 205));                      //DodgerBlue
    SelectObject(hdc, hpen);
    apt2 = new POINT[m];
    while (k<m && m != 1 )
    {
        apt2[k].x = k * rect.right / (m-1);
        apt2[k].y = apt1[k].y * rect.bottom / 80;
        k++;
    }
    Polyline(hdc, apt2, m);
    DeleteObject(hpen);
    delete[]apt2;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //int wmId, wmEvent;    
    PAINTSTRUCT ps;
    HDC hdc;
    HDC hdcStatic;
    static HFONT hFont;
    static HWND hStatic1;
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
            FALSE/*斜体?*/, FALSE/*下划线?*/, FALSE/*删除线?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("微软雅黑"));

        hStatic3 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            15, 198, 153, 294, hWnd, (HMENU)3, NULL, NULL);

        hStatic1 = CreateWindow(TEXT("STATIC"), TEXT("数据结果"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | SS_CENTER,
            20, 200, 143, 30, hWnd, (HMENU)btn1, NULL, NULL);

        hStatic2 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE ,
            20, 230, 143, 130, hWnd, (HMENU)btn2, NULL, NULL);

        hStatic8 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE,
            20, 360, 143, 130, hWnd, (HMENU)8, NULL, NULL);

        hStatic4 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE ,
            200, 170, 900, 80, hWnd, (HMENU)4, NULL, NULL);

        hStatic5 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE ,
            200, 430, 900, 80, hWnd, (HMENU)5, NULL, NULL);

        hStatic6 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE,
            200, 250, 900, 180, hWnd, (HMENU)6, NULL, NULL);

        hStatic7 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE,
            200, 510, 900, 180, hWnd, (HMENU)7, NULL, NULL);

        SendMessage(hStatic1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(hStatic2, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(hStatic8, WM_SETFONT, (WPARAM)hFont, NULL);


    case WM_CTLCOLORSTATIC: 
    {
        hdcStatic = (HDC)wParam;
        SetBkMode(hdcStatic, TRANSPARENT);
    }

    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        WriteConsole(hOut, L"111 \n", 6, NULL, NULL);
        if (ismove == 0)
        {
            switch (CanvasStatus)
            {
            case 1:
            {
                DrawLine(hStatic4, gnum, gn);
                break;
            }
            case 2:
            {
                DrawLine(hStatic4, gnum, gn);
                ismove = 1;
                Drawlinemk2(hStatic4, pt, ptt);
                Drawlinemk3(hStatic4);
                ismove = 0;
                break;
            }
            case 3:
            {
                DrawLine(hStatic4, gnum, gn);
                DrawLine(hStatic5, gnum1, gn1);
                break;
            }
            case 4:
            {
                DrawLine(hStatic4, gnum, gn);
                ismove = 1;
                Drawlinemk2(hStatic4, pt, ptt);
                Drawlinemk3(hStatic4);
                ismove = 0;
                DrawLine(hStatic5, gnum1, gn1);
                break;
            }
            case 5:
            {
                DrawLine(hStatic4, gnum, gn);
                DrawLine(hStatic5, gnum1, gn1);
                ismove = 2;
                Drawlinemk2(hStatic5, pt1, ptt1);
                Drawlinemk3(hStatic5);
                ismove = 0;
                break;
            }
            case 6:
            {
                DrawLine(hStatic4, gnum, gn);
                ismove = 1;
                Drawlinemk2(hStatic4, pt, ptt);
                Drawlinemk3(hStatic4);
                DrawLine(hStatic5, gnum1, gn1);
                ismove = 2;
                Drawlinemk2(hStatic5, pt1, ptt1);
                Drawlinemk3(hStatic5);
                ismove = 0;
                break;
            }

            }
        }
        EndPaint(hWnd, &ps);
        break;
    }        

    case WM_LBUTTONDOWN: 
    {
        pt0 = pt;
        GetCursorPos(&pt);
        GetWindowRect(hStatic4, &rect);
        if (PtInRect(&rect, pt))
        {
            ScreenToClient(hStatic4, &pt);
            ismove = 1;
        }
        else
        {
            GetWindowRect(hStatic5, &rect);
            if (PtInRect(&rect, pt)&&IsWindowVisible(hStatic5))
            {
                pt1 = pt;
                pt = pt0;
                ScreenToClient(hStatic5, &pt1);
                ismove = 2;
            }
            else
            {
                ismove = 0;
                pt = pt0;
            }

        }
        return 0;
    }

    case WM_LBUTTONUP:
    {
        if (ismove == 1)
        {
            Drawlinemk3(hStatic4);
            switch (CanvasStatus)
            {
                case 1:
                {
                    CanvasStatus = 2;
                    break;
                }
                case 3:
                {
                    CanvasStatus = 4;
                    break;
                }
                case 5:
                {
                    CanvasStatus = 6;
                    break;
                }
            }
            
        }
        if (ismove == 2)
        {
            Drawlinemk3(hStatic5);
            switch (CanvasStatus)
            {
                case 3:
                {
                    CanvasStatus = 5;
                    break;
                }
                case 4:
                {
                    CanvasStatus = 6;
                    break;
                }
            }
        }
        ismove = 0;
        return 0;
    }

    case WM_MOUSEMOVE: 
    {
        if (ismove == 1)
        {
            GetCursorPos(&ptt);
            ScreenToClient(hStatic4, &ptt);
            DrawLine(hStatic4, gnum, gn);
            Drawlinemk2(hStatic4, pt, ptt);
            SetWindowText(hStatic8, szBuffer1);
        }
        if (ismove == 2)
        {
            GetCursorPos(&ptt1);
            ScreenToClient(hStatic5, &ptt1);
            DrawLine(hStatic5, gnum1, gn1);
            Drawlinemk2(hStatic5, pt1, ptt1);
            SetWindowText(hStatic8, szBuffer1);
        }
        return 0;
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
    return 0;
}