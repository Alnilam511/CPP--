#define WIN32_LEAN_AND_MEAN           

#include <windows.h>

#include "SimpleRibbon.h"
#include "RibbonFramework.h"

#include "GlobalVariable.h"
#include "Functions.h"
#include <fft.h>            //����SP++�źſ�- ����Ҷ�任����
#include <wft.h>            //�Ӵ�����->��ʱ����Ҷ
#include <cwt.h>            //С���任
#include "WaveletPack.h"    //С�����任
#include "EMD.h"            //����ģ̬�ֽ�
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
int ismove = 0;                                   // ���λ���жϣ�0�����⣬1��hstatic4��2��hstatic5

int CanvasStatus = 0;                  //����״̬�ж� |  1  |  2  |  3  |  4  |  5  |  6  |    ��Ϊint���֣���Ϊ��Ӧ״̬
                                       //             |  4  |  4' | 4,5 | 4',5| 4,5'|4',5'|    4������4��5������5���� ' ���ֲ��Ŵ�״̬
int m;

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
    HDC hDCMem;                                             //˫�ػ���
    HBITMAP hBmpMem;
    hDCMem = CreateCompatibleDC(hdc);
    hBmpMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
    SelectObject(hDCMem, hBmpMem);

    if (hWnd == hStatic4)                                                        //////////////Drawline1��ʼ//////////////////
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
    int i1 = rect.bottom / 5;  int j1 = rect.right / 5;
    while (i1 < rect.bottom)
    {
        MoveToEx(hDCMem, 0, i1, NULL);
        LineTo(hDCMem, rect.right, i1);
        i1 = i1 + rect.bottom / 5;
    }
    while (j1 < rect.right)
    {
        MoveToEx(hDCMem, j1, 0, NULL);
        LineTo(hDCMem, j1, rect.bottom);
        j1 = j1 + rect.right / 5;
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
        int k1 = 0;
        double min = input[0], max = input[0];
        for (int l1 = 1; l1 < n2; l1++)
        {
            max = Maximum(max, input[l1]);
            min = Minimum(min, input[l1]);
        }
        max = Maximum(fabs(max), fabs(min));
        if (hWnd == hStatic4)                            //hstatic4 aptt
        {
            delete[]aptt;
            aptt = new POINT[n2];
            while (k1 < n2)
            {
                aptt[k1].x = k1 * rect.right / (n2 - 1);
                aptt[k1].y = (long)((-input[k1]) * rect.bottom / (max * 2)) + rect.bottom / 2;
                k1++;
            }
            Polyline(hDCMem, aptt, n2);
        }
        if (hWnd == hStatic5)                          //hstatic5 apt
        {
            delete apt;
            apt = new POINT[n2];
            while (k1 < n2)
            {
                apt[k1].x = k1 * rect.right / (n2 - 1);
                apt[k1].y = (long)((-input[k1]) * rect.bottom / (max * 2)) + rect.bottom / 2;
                k1++;
            }
            Polyline(hDCMem, apt, n2);

        }
        if (hWnd == hStatic7)
        {
            delete apts;
            apts = new POINT[n2];
            while (k1 < n2)
            {
                apts[k1].x = k1 * rect.right / (n2 - 1);
                apts[k1].y = (long)((-input[k1]) * rect.bottom / (max * 2)) + rect.bottom / 2;
                k1++;
            }
            Polyline(hDCMem, apts, n2);

        }
        DeleteObject(hpen);

    }                                                                              ///////////////Drawline1����//////////////////


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
    HBRUSH hbrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hDCMem, &rect, hbrush);
    DeleteObject(hbrush);
    POINT* apt0;
    apt0 = new POINT;
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
    if (m == 0)
    {
        return;
    }

    double* part = GetPart(input, l, k);
    LPCTSTR str;
    memset(szBuffer1, '\0', sizeof(szBuffer1));
                                                              /////////////////////�ֲ�����/////////////////////////
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
        str = L" �ֲ����ݸ���Ϊ��\n   %i\n �ֲ���ֵΪ:\n   %e\n   %e\n";
        swprintf_s(szBuffer1, str, m + 1, max, min);
        break;
    }        
    case cmdButton4:
    {
        double average = Average(part, m + 1);
        double variance = Variance(part, m + 1);
        str = L" �ֲ����ݸ���:\n   %i\n �ֲ���ֵ:\n   %e\n �ֲ�����:\n   %e\n";
        swprintf_s(szBuffer1, str, m + 1, average, variance);
        break;
    }
    case cmdButton5:
    {
        double kurtosis = Kurtosis(part, m + 1);
        str = L" �ֲ����ݸ���:\n   %i\n �ֲ��Ͷ�:\n   %e\n";
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
        str = L" k:\n   %i\n l:\n   %i\n";
        DrawLine(hStatic7, output, m + 1);
        swprintf_s(szBuffer1, str, k, l);
        break;
    }
    case cmdButton9:
    {
        //����
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
        break;
    }
    case cmdButton11:
    {
        //��ʱ����Ҷ
        break;
    }
    case cmdButton12:
    {
        //С��
        break;
    }
    case cmdButton13:
    {
        //С��������
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
        fclose(fp);
        fclose(fp1);
        break;
    }
    case cmdButton14:
    {
        //ϣ�����ر任
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
        break;
    }
    case cmdButton15:
    {
        //����ģ̬
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



void Drawlinemk3(HWND hWnd)
{
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
    rect.right = rect.right - 1;
    rect.bottom = rect.bottom - 1;
    hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(hdc, hpen);
    MoveToEx(hdc, 0, 0, NULL);
    LineTo(hdc, rect.right, 0);
    LineTo(hdc, rect.right, rect.bottom);
    LineTo(hdc, 0, rect.bottom);
    LineTo(hdc, 0, 0);
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
    return;
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
            FALSE/*б��?*/, FALSE/*�»���?*/, FALSE/*ɾ����?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("΢���ź�"));

        hStatic3 = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            15, 198, 153, 294, hWnd, (HMENU)3, NULL, NULL);

        hStatic1 = CreateWindow(TEXT("STATIC"), TEXT("���ݽ��"),
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
                ismove = 2;
                Drawlinemk2(hStatic5, pt1, ptt1);
                Drawlinemk3(hStatic5);
                ismove = 0;
                break;
            }
            case 6:
            {
                ismove = 1;
                Drawlinemk2(hStatic4, pt, ptt);
                Drawlinemk3(hStatic4);
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
        switch (BtnNum)
        {
        case 0 :
            return 0;
        case cmdButton3:
        case cmdButton4:
        case cmdButton5:
        case cmdButton6:
        case cmdButton7:
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
        case cmdButton9:
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
            Drawlinemk3(hStatic4);
            //if (isSpectrum == true)
            //{

            //}
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
            //DrawLine(hStatic4, gnum, gn);
            Drawlinemk2(hStatic4, pt, ptt);
            SetWindowText(hStatic8, szBuffer1);
        }
        if (ismove == 2)
        {
            GetCursorPos(&ptt1);
            ScreenToClient(hStatic5, &ptt1);
            //DrawLine(hStatic5, gnum1, gn1);
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