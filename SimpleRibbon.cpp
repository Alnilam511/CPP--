#define WIN32_LEAN_AND_MEAN           

#include <windows.h>

#include "SimpleRibbon.h"
#include "RibbonFramework.h"

#include "GlobalVariable.h"
#include "Functions.h"
#include <fft.h>            //调用SP++信号库- 傅里叶变换函数
#include <wft.h>            //加窗函数->短时傅里叶
#include <cwt.h>            //小波变换
#include "WaveletPack.h"    //小波包变换
#include "EMD.h"            //经验模态分解
#include <iomanip>

//#include "CommandHandler.h"
#include "SimpleRibbonUI.h"
#include <stack.h>
#define MAX_LOADSTRING 100

using namespace splab;

int BtnNum = 0;
POINT* apt1;
TCHAR szBuffer0[MAX_PATH];
TCHAR szBuffer[MAX_PATH];
TCHAR szBuffer1[MAX_PATH];
HWND hStatic8;
HWND hStatic2;
HWND hComboBox;
HFONT hFont;

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
double* onum = NULL;                               //old num
double* onum1 = NULL;
double* pnum1 = NULL;
double* pnum2 = NULL;
int ismove = 0;                                   // 点击位置判断，0窗口外，1在hstatic4，2在hstatic5

int CanvasStatus = 0;                  //画布状态判断 |  1  |  2  |  3  |  4  |  5  |  6  |    上为int数字，下为对应状态
                                       //             |  4  |  4' | 4,5 | 4',5| 4,5'|4',5'|    4：窗口4，5：窗口5，带 ' ：局部放大状态
int m;                                 //   截取数据长度

int l1, l2, k1, k2, m1, m2;

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

    hWnd = CreateWindow(wszWindowClass, L"TEST No.1", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_MAXIMIZE,
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

void DrawCoordinate(HWND hWnd, HFONT hfont, double* num, int n,int nn,int len)
{
    AxisText laxis[5];
    AxisText raxis[5];
    HDC hdc = GetDC(hWnd);
    SelectObject(hdc, hfont);
    GetClientRect(hWnd, &rect);
    rect.right = rect.right - 1;
    SetTextAlign(hdc, TA_RIGHT);
    double min = num[0], max = num[0];
    for (int l = 1; l < len; l++)
    {
        max = Maximum(max, num[l]);
        min = Minimum(min, num[l]);
    }
    max = Maximum(fabs(max), fabs(min));
    max = (long)max + 1;

    for (int i = 0; i < 5; i++)
    {
        laxis[i].x = -5;
        laxis[i].y = (rect.bottom * i / 4) - 10;
        laxis[i].num = (int)(max - max * i / 2);
        wchar_t str[10];
        wsprintf(str, L"%d", laxis[i].num);
        TextOut(hdc, laxis[i].x, laxis[i].y, str, lstrlen(str));
    }
    SetTextAlign(hdc, TA_CENTER);
    for (int i = 0; i < 5; i++)
    {
        raxis[i].x = (rect.right * i / 4);
        raxis[i].y = rect.bottom + 5;
        raxis[i].num = n + (int)((nn-n) * i / 4);
        wchar_t str[10];
        wsprintf(str, L"%d", raxis[i].num);
        TextOut(hdc, raxis[i].x, raxis[i].y, str, lstrlen(str));
    }

    return;
}


void Drawlinemk2(HWND hWnd, POINT ptx, POINT ptty)
{
    double* input = NULL;
    double* output = NULL;
    delete[]input;
    delete[]output;
    int n2=0;

    typedef double Type;
    Vector < complex<Type> > Xk, Yk;
    Vector <Type> xn, yn;
    int k, l;
    k = l = 0;
    long i, j;
    int n = 0;

    HDC hdc = GetDC(hWnd);
    HDC hDCMem;                                             //双重缓冲
    HBITMAP hBmpMem;
    hDCMem = CreateCompatibleDC(hdc);
    hBmpMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
    SelectObject(hDCMem, hBmpMem);

    if (hWnd == hStatic4)                                                        //////////////Drawline1开始//////////////////
    {
        input = gnum;
        n2 = gn;
    }
    if (hWnd == hStatic5)
    {
        input = gnum1;
        n2 = gn1;
    }


    GetClientRect(hWnd, &rect);
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    SetBkColor(hDCMem, RGB(255, 255, 255));
    FillRect(hDCMem, &rect, hBrush);
    DeleteObject(hBrush);

    hpen = CreatePen(PS_DASHDOT, 1, RGB(220, 220, 220));              //Gainsboro
    SelectObject(hDCMem, hpen);
    int i1 = rect.bottom / 4;  int j1 = rect.right / 4;
    while (i1 < rect.bottom)
    {
        MoveToEx(hDCMem, 0, i1, NULL);
        LineTo(hDCMem, rect.right, i1);
        i1 = i1 + rect.bottom / 4;
    }
    while (j1 < rect.right)
    {
        MoveToEx(hDCMem, j1, 0, NULL);
        LineTo(hDCMem, j1, rect.bottom);
        j1 = j1 + rect.right / 4;
    }
    DeleteObject(hpen);
    rect.right = rect.right - 1;
    rect.bottom = rect.bottom - 1;
    hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(hDCMem, hpen);
    MoveToEx(hDCMem, 0, 0, NULL);
    LineTo(hDCMem, rect.right, 0);
    LineTo(hDCMem, rect.right, rect.bottom);
    LineTo(hDCMem, 0, rect.bottom);
    LineTo(hDCMem, 0, 0);
    DeleteObject(hpen);

    if (input != NULL)
    {
        hpen = CreatePen(PS_SOLID, 1, RGB(24, 116, 205));             //DodgerBlue
        SelectObject(hDCMem, hpen);
        int kk = 0;
        double min = input[0], max = input[0];
        for (int ll = 1; ll < n2; ll++)
        {
            max = Maximum(max, input[ll]);
            min = Minimum(min, input[ll]);
        }
        max = Maximum(fabs(max), fabs(min));
        max = (long)max + 1;
        if (hWnd == hStatic4)                            //hstatic4 aptt
        {
            delete[]aptt;
            aptt = new POINT[n2];
            while (kk < n2)
            {
                aptt[kk].x = kk * rect.right / (n2 - 1);
                aptt[kk].y = (long)((-input[kk]) * rect.bottom / (max * 2)) + rect.bottom / 2;
                kk++;
            }
            Polyline(hDCMem, aptt, n2);
        }
        else if (hWnd == hStatic5)                          //hstatic5 apt
        {
            delete apt;
            apt = new POINT[n2];
            while (kk < n2)
            {
                apt[kk].x = kk * rect.right / (n2 - 1);
                apt[kk].y = (long)((-input[kk]) * rect.bottom / (max * 2)) + rect.bottom / 2;
                kk++;
            }
            Polyline(hDCMem, apt, n2);

        }
        else 
        {
            delete apts;
            apts = new POINT[n2];
            while (kk < n2)
            {
                apts[kk].x = kk * rect.right / (n2 - 1);
                apts[kk].y = (long)((-input[kk]) * rect.bottom / (max * 2)) + rect.bottom / 2;
                kk++;
            }
            Polyline(hDCMem, apts, n2);

        }
        DeleteObject(hpen);

    }                                                                              ///////////////Drawline1结束//////////////////


    GetClientRect(hWnd, &rect);
    rect.bottom = rect.bottom - 1;
    if (ptty.x >= 900)
    {
        ptty.x = 899;
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
    POINT* apt0;
    apt0 = new POINT;
    if (hWnd == hStatic4)
    {
        apt0 = aptt;
        input = gnum;
    }
    else
    {
       if (hWnd == hStatic5)
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
    if (m == 0)
    {
        return;
    }

    double* part = GetPart(input, l, k);

    if (hWnd == hStatic4)
    {
        l1 = l;
        k1 = k;
        m1 = m;
    }
    if (hWnd == hStatic5)
    {
        l2 = l;
        k2 = k;
        m2 = m;
    }

    LPCTSTR str;
    memset(szBuffer1, '\0', sizeof(szBuffer1));
                                                              /////////////////////局部计算/////////////////////////
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
    case cmdButton8:
    {
        int bb = 0;
        for (int aa = k; aa <= l; aa++)
        {
            xn.resize(m + 1);
            xn[bb] = input[aa];
            bb++;
        }
        Xk = fftr2c(xn);
        output = new double[m + 1];
        for (int cc = 0; cc < m + 1; ++cc)
        {
            output[cc] = abs(Xk[cc]) * abs(Xk[cc]);
        }
        DrawLine(hStatic7, output, m + 1);
        DrawCoordinate(hStatic7, hFont, output, 0, m + 1, m + 1);
        break;
    }
    case cmdButton9:
    {
        //互谱
        break;
    }
    case cmdButton10:
    {
        int bb = 0;
        for (int aa = k; aa <= l; aa++)
        {
            xn.resize(m + 1);
            xn[bb] = input[aa];
            bb++;
        }
        Xk = fftr2c(xn);
        output = new double[m + 1];
        for (int cc = 0; cc < m + 1; ++cc)
        {
            output[cc] = abs(Xk[cc]);
            Yk.resize(m + 1);
            Yk[cc] = complex<Type>(log(output[cc]), 0);
        }
        yn = ifftc2r(Yk);
        DrawLine(hStatic7, yn, m + 1);
        DrawCoordinate(hStatic7, hFont, yn, 0, m + 1, m + 1);
        break;
    }
    case cmdButton11:
    {
        //短时傅里叶
        break;
    }
    case cmdButton12:
    {
        //小波
        break;
    }
    case cmdButton13:
    {
        //小波包分析
        int dbN = 1, stages = 3, numf = 1, bb;
        float thrshold = 0.5;
        n = m + 1;
        FILE* fp, * fp1;

        ofstream OutFile("WaveletPack-Part.txt");
        for (int aa = k; aa <= l; aa++)
        {
            OutFile << input[aa] << endl;
        }
        OutFile.close();

        //p = szFile + ofn.nFileOffset;
        //char nchar[256] = { 0 };
        //WideCharToMultiByte(CP_ACP, 0, p, -1, nchar, 256, NULL, NULL);
        const char* constc = nullptr;
        constc = { "WaveletPack-Part.txt" };

        if ((fp = fopen(constc, "r")) == NULL)
        {
            printf("cann't open input file\n");
            exit(-3);
        }
        for (bb = 0; bb < n; bb++)
            fscanf_s(fp, "%f", &dataio[bb]);

        // wavelet transformation
        dwpt_filter(dbN, stages, numf, thrshold);

        // out put result
        if ((fp1 = fopen("WaveletPackOutput-Part.txt", "w+")) == NULL)
        {
            printf("cann't open output file\n");
            exit(-4);
        }
        for (bb = 0; bb < n; bb++)
            fprintf(fp1, "%10.4f\n", dataio[bb]);
        double *snum = NULL;
        delete[]snum;
        snum = new double[n];
        for (i = 0; i < n; i++)
        {
            snum[i] = dataio[i];
        }
        DrawLine(hStatic7, snum, n);
        DrawCoordinate(hStatic7, hFont, snum, 0, n, n);
        fclose(fp);
        fclose(fp1);
        break;
    }
    case cmdButton14:
    {
        //希尔伯特变换
        double* hnum = NULL;
        delete[]hnum;
        hnum = new double[m+1];
        int bb = 0;
        for (int aa = k; aa <= l; aa++)
        {
            xn.resize(m + 1);
            xn[bb] = input[aa];
            hnum[bb] = 0;
            bb++;
        }
        Xk = fftr2c(xn);
        n = m + 1;
        if ((n > 0) && ((2 * floor(n / 2)) == (n))) // n is even
        {
            hnum[0] = 1;
            hnum[n / 2] = 1;

            for (int cc = 1; cc < n / 2; cc++)
            {
                hnum[cc] = 2;
            }
        }
        else //n is odd
        {
            if (n > 0)
            {
                hnum[0] = 1;
                for (int cc = 1; cc < (n + 1) / 2; cc++)
                {
                    hnum[cc] = 2;
                }
            }
        }
        for (int cc = 0; cc < n; cc++)
        {
            double xr = real(Xk[cc]);
            double xi = imag(Xk[cc]);
            xr = xr * hnum[cc];
            xi = xi * hnum[cc];
            Xk.resize(n);
            Xk[cc] = complex<Type>(xr, xi);
        }
        Yk = ifftc2c(Xk);
        output = new double[n];
        for (int cc = 0; cc < n; cc++)
        {
            output[cc] = imag(Yk[cc]);
        }
        DrawLine(hStatic7, output, n);
        DrawCoordinate(hStatic7, hFont, output, 0, n, n);
        break;
    }
    case cmdButton15:
    {
        //经验模态
        n = m + 1;
        double* s = NULL;
        double* t = NULL;
        delete[]s;
        delete[]t;
        s = new double[n];
        t = new double[n];

        int bb = 0;
        for (int aa = k; aa <= l; aa++)
        {
            s[bb] = input[aa];
            bb++;
        }
        for (int cc = 0; cc < n; cc++)
        {
            t[cc] = 1.0 * cc / n;
        }
        double yp1 = s[0] / t[0];
        double ypn = s[n] / t[n];
        double* y2 = spline1(t, s, n, yp1, ypn);
        output = new double[n];
        for (int cc = 0; cc < n; cc++)
        {
            double x = t[cc];
            double y = spline2(t, s, y2, n, x);
            output[cc] = y;
        }
        DrawLine(hStatic7, output, n);
        DrawCoordinate(hStatic7, hFont, output, 0, n, n);
        break;
    }
    }                                         
                                                              /////////////////////////////////////////////////////
    delete[]apt1;
    apt1 = new POINT[m];
    n = 0;
    while (n<m)
    {
        apt1[n] = apt0[k];
        n++;
        k++;
    }
    hpen = CreatePen(PS_SOLID, 1, RGB(255,127,0));                 //DarkOrange1
    SelectObject(hDCMem, hpen);
    Polyline(hDCMem, apt1, m);
    DeleteObject(hpen);
    hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(hDCMem, hpen);
    MoveToEx(hDCMem, 0, 0, NULL);
    LineTo(hDCMem, rect.right, 0);
    MoveToEx(hDCMem, rect.right, rect.bottom,NULL);
    LineTo(hDCMem, 0, rect.bottom);
    DeleteObject(hpen);
    GetClientRect(hWnd, &rect);
    BitBlt(hdc, 0, 0, rect.right, rect.bottom, hDCMem, 0, 0, SRCCOPY);
    DeleteDC(hDCMem);
    DeleteObject(hBmpMem);
    return;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //int wmId, wmEvent;    
    PAINTSTRUCT ps;
    HDC hdc;
    HDC hdcStatic;
    static HWND hStatic1;
    static HWND hStatic3;
    bool initSuccess;
    int wmId, wmEvent;

    switch (message)
    {
    case WM_CREATE:
        // Initializes the Ribbon framework.
        initSuccess = InitializeFramework(hWnd);
        if (!initSuccess) 
        {
            return -1;
        }

        hFont = CreateFont(-12, -6, 0, 0, 400 ,
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
            WS_CHILD | WS_VISIBLE | SS_OWNERDRAW ,
            200, 170, 900, 80, hWnd, (HMENU)4, NULL, NULL);

        hStatic5 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE | SS_OWNERDRAW,
            200, 490, 900, 80, hWnd, (HMENU)5, NULL, NULL);

        hStatic6 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE | SS_OWNERDRAW,
            200, 280, 900, 180, hWnd, (HMENU)6, NULL, NULL);

        hStatic7 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE | SS_OWNERDRAW,
            200, 600, 900, 180, hWnd, (HMENU)7, NULL, NULL);

        hComboBox = CreateWindow(TEXT("COMBOBOX"), TEXT(""),
            WS_CHILD | CBS_DROPDOWNLIST,
            20, 510, 100, 80, hWnd, (HMENU)9, NULL, NULL);

        SendMessage(hStatic1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(hStatic2, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(hStatic8, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(hComboBox, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("时域"));
        SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)0, 0);
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("幅值、相位"));
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("实部、虚部"));


    case WM_CTLCOLORSTATIC: 
    {
        hdcStatic = (HDC)wParam;
        SetBkMode(hdcStatic, TRANSPARENT);
    }
    case WM_COMMAND:
    {
        wmId = lParam;
        wmEvent = HIWORD(wParam);
        switch (wmEvent)
        {
        case CBN_SELCHANGE:
            TCHAR  szCombo[20];
            GetDlgItemText(hWnd, 9, szCombo, 20);
            if (wcscmp(szCombo, TEXT("时域")) == 0)
            {
                if (onum == NULL && onum1 == NULL)
                {
                    onum = gnum;
                    onum1 = gnum1;
                }
                else
                {
                    gnum = onum;
                    gnum1 = onum1;
                }
                InvalidateRect(hStatic6, NULL, true);
                UpdateWindow(hStatic6);

            }
            if (wcscmp(szCombo, TEXT("幅值、相位")) == 0)
            {
                if (onum == NULL && onum1 == NULL)
                {
                    onum = gnum;
                    onum1 = gnum1;
                    gnum = anum;
                    gnum1 = pnum;

                }
                else
                {
                    gnum = anum;
                    gnum1 = pnum;
                }
                InvalidateRect(hStatic6, NULL, true);
                UpdateWindow(hStatic6);
            }
            if (wcscmp(szCombo, TEXT("实部、虚部")) == 0)
            {
                if (onum == NULL && onum1 == NULL)
                {
                    onum = gnum;
                    onum1 = gnum1;
                    gnum = rnum;
                    gnum1 = lnum;

                }
                else
                {
                    gnum = rnum;
                    gnum1 = lnum;
                }
                InvalidateRect(hStatic6, NULL, true);
                UpdateWindow(hStatic6);
            }
            break;
        }
        return 0;
    }

    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        //if (ismove == 0)
        //{
        //    switch (CanvasStatus)
        //    {
        //    case 1:
        //    {
        //        DrawLine(hStatic4, gnum, gn);
        //        break;
        //    }
        //    case 2:
        //    {
        //        ismove = 1;
        //        Drawlinemk2(hStatic4, pt, ptt);
        //        Drawlinemk3(hStatic4);
        //        ismove = 0;
        //        break;
        //    }
        //    case 3:
        //    {
        //        DrawLine(hStatic4, gnum, gn);
        //        DrawLine(hStatic5, gnum1, gn1);
        //        break;
        //    }
        //    case 4:
        //    {
        //        ismove = 1;
        //        Drawlinemk2(hStatic4, pt, ptt);
        //        Drawlinemk3(hStatic4);
        //        ismove = 0;
        //        DrawLine(hStatic5, gnum1, gn1);
        //        break;
        //    }
        //    case 5:
        //    {
        //        DrawLine(hStatic4, gnum, gn);
        //        ismove = 2;
        //        Drawlinemk2(hStatic5, pt1, ptt1);
        //        Drawlinemk3(hStatic5);
        //        ismove = 0;
        //        break;
        //    }
        //    case 6:
        //    {
        //        ismove = 1;
        //        Drawlinemk2(hStatic4, pt, ptt);
        //        Drawlinemk3(hStatic4);
        //        ismove = 2;
        //        Drawlinemk2(hStatic5, pt1, ptt1);
        //        Drawlinemk3(hStatic5);
        //        ismove = 0;
        //        break;
        //    }

        //    }
        //}
        EndPaint(hWnd, &ps);
        break;
    }        

    case WM_DRAWITEM:
    {
        if (ismove == 0)
        {
            switch (BtnNum)
            {
            case 0:
                return 0;
            case cmdButton3:
            case cmdButton4:
            case cmdButton5:
            case cmdButton6:
            {
                if (CanvasStatus ==1)
                {
                    DrawLine(hStatic4, gnum, gn);
                    DrawCoordinate(hStatic4, hFont, gnum, 0, gn, gn);
                }
                if (CanvasStatus ==2)
                {
                    ismove = 1;
                    Drawlinemk2(hStatic4, pt, ptt);
                    //Drawlinemk3(hStatic4);
                    DrawLine(hStatic6, pnum1, m1);
                    DrawCoordinate(hStatic4, hFont, gnum, 0, gn, gn);
                    DrawCoordinate(hStatic6, hFont, pnum1, k1, l1, m1);
                    ismove = 0;
                }
                break;
            }
            case cmdButton7:
            case cmdButton9:
            {
                switch (CanvasStatus)
                {
                case 3:
                {
                    DrawLine(hStatic4, gnum, gn);
                    DrawLine(hStatic5, gnum1, gn1);
                    DrawCoordinate(hStatic4, hFont, gnum, 0, gn, gn);
                    DrawCoordinate(hStatic5, hFont, gnum1, 0, gn1, m1);
                    break;
                }
                case 4:
                {
                    ismove = 1;
                    Drawlinemk2(hStatic4, pt, ptt);
                    DrawLine(hStatic6, pnum1, m1);
                    ismove = 0;
                    DrawLine(hStatic5, gnum1, gn1);
                    DrawCoordinate(hStatic4, hFont, gnum, 0, gn, gn);
                    DrawCoordinate(hStatic5, hFont, gnum1, 0, gn1, gn1);
                    DrawCoordinate(hStatic6, hFont, pnum1, k1, l1, m1);
                    break;
                }
                case 5:
                {
                    DrawLine(hStatic4, gnum, gn);
                    ismove = 2;
                    Drawlinemk2(hStatic5, pt1, ptt1);
                    DrawLine(hStatic7, pnum2, m2);
                    ismove = 0;
                    DrawCoordinate(hStatic4, hFont, gnum, 0, gn, gn);
                    DrawCoordinate(hStatic5, hFont, gnum1, 0, gn1, gn1);
                    DrawCoordinate(hStatic7, hFont, pnum2, k2, l2, m2);
                    break;
                }
                case 6:
                {
                    ismove = 1;
                    Drawlinemk2(hStatic4, pt, ptt);
                    DrawLine(hStatic6, pnum1, m1);
                    ismove = 2;
                    Drawlinemk2(hStatic5, pt1, ptt1);
                    DrawLine(hStatic7, pnum2, m2);
                    ismove = 0;
                    DrawCoordinate(hStatic4, hFont, gnum, 0, gn, gn);
                    DrawCoordinate(hStatic5, hFont, gnum1, 0, gn1, gn1);
                    DrawCoordinate(hStatic6, hFont, pnum1, k1, l1, m1);
                    DrawCoordinate(hStatic7, hFont, pnum2, k2, l2, m2);
                    break;
                }
                default:
                    break;
                }
                break;
            }
            case cmdButton8:
            case cmdButton10:
            case cmdButton11:
            case cmdButton12:
            case cmdButton13:
            case cmdButton14:
            case cmdButton15:
            {
                if (CanvasStatus == 3)
                {
                    DrawLine(hStatic4, gnum, gn);
                    DrawLine(hStatic5, gnum1, gn1);
                    DrawCoordinate(hStatic4, hFont, gnum, 0, gn, gn);
                    DrawCoordinate(hStatic5, hFont, gnum1, 0, gn1, gn1);
                    break;
                }
                if (CanvasStatus == 4)
                {
                    ismove = 1;
                    Drawlinemk2(hStatic4, pt, ptt);
                    DrawLine(hStatic6, pnum1, m1);
                    ismove = 0;
                    DrawLine(hStatic5, gnum1, gn1);
                    DrawCoordinate(hStatic4, hFont, gnum, 0, gn, gn);
                    DrawCoordinate(hStatic5, hFont, gnum1, 0, gn1, gn1);
                    DrawCoordinate(hStatic6, hFont, pnum1, k1, l1, m1);
                    break;
                }
            break;
            }
            default:
                break;
            }
        }
        return 0;
    }

    case WM_LBUTTONDOWN: 
    {
        switch (BtnNum)
        {
        case 0 :
            return 0;
        case cmdButton3:
        case cmdButton4:
        case cmdButton5:
        case cmdButton6:
        case cmdButton7:
        case cmdButton9:
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
            if (PtInRect(&rect, pt) && IsWindowVisible(hStatic5))
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
        case cmdButton8:
        case cmdButton10:
        case cmdButton11:
        case cmdButton12:
        case cmdButton13:
        case cmdButton14:
        case cmdButton15:
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
                ismove = 0;
                pt = pt0;
            }
            return 0;
        }
        default:
            break;
        }
        
    }

    case WM_LBUTTONUP:
    {
        if (ismove == 1)
        {
            delete []pnum1;
            pnum1 = new double[m1];
            for (int i = 0; i < m1; i++)
            {
                pnum1[i] = gnum[k1 + i];
            }
            DrawLine(hStatic6, pnum1, m1);
            InvalidateRect(hwnd, NULL, true);
            UpdateWindow(hwnd);
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
            delete[]pnum2;
            pnum2 = new double[m2];
            for (int i = 0; i < m2; i++)
            {
                pnum2[i] = gnum1[k2 + i];
            }
            DrawLine(hStatic7, pnum2, m2);
            InvalidateRect(hwnd, NULL, true);
            UpdateWindow(hwnd);
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
            Drawlinemk2(hStatic4, pt, ptt);
            SetWindowText(hStatic8, szBuffer1);
        }
        if (ismove == 2)
        {
            GetCursorPos(&ptt1);
            ScreenToClient(hStatic5, &ptt1);
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