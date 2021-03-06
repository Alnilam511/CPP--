#include <UIRibbon.h>

#include <fstream>          //提供文件头文件
#include <math.h>

#include <iostream>
#include <iomanip>          //精度
#include <cstdlib>
#include <vectormath.h>
//#include <statistics.h> 
#include <fft.h>            //调用SP++信号库- 傅里叶变换函数
#include <wft.h>            //加窗函数->短时傅里叶
#include <cwt.h>            //小波变换
#include "WaveletPack.h"    //小波包变换
#include "EMD.h"            //经验模态分解

#include "SpectrumHandler.h"
#include "SimpleRibbonUI.h"
#include "RibbonFramework.h"
#include "Resource.h"

#include "GlobalVariable.h"
#include "Functions.h"

using namespace std;
using namespace splab;  //命名空间声明

STDMETHODIMP CSpectrumHandler::Execute(
    UINT nCmdID,
    UI_EXECUTIONVERB verb,
    const PROPERTYKEY* key,
    const PROPVARIANT* ppropvarValue,
    IUISimplePropertySet* pCommandExecutionProperties)
{
    UNREFERENCED_PARAMETER(pCommandExecutionProperties);
    UNREFERENCED_PARAMETER(ppropvarValue);
    UNREFERENCED_PARAMETER(key);
    //UNREFERENCED_PARAMETER(verb);
    //UNREFERENCED_PARAMETER(nCmdID);

    if (nCmdID == cmdButton8 && verb == UI_EXECUTIONVERB_EXECUTE)
    {    
        BtnNum = nCmdID;
        OPENFILENAME ofn;
        TCHAR szFile[MAX_PATH];
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = L"Text\0*.TXT\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn))
        {
            typedef double Type;
            Vector < complex<Type> > Xk;
            Vector <Type> xn;

            ifstream fin;
            fin.open(szFile);
            int n = GetLine(fin);
            fin.close();

            fin.open(szFile);
            double* num = GetNum(fin, n);
                        
            ShowWindow(hStatic5, SW_SHOWDEFAULT);
            ShowWindow(hStatic7, SW_HIDE);
            ShowWindow(hStatic6, SW_HIDE);
            ShowWindow(hComboBox, SW_HIDE);
            DrawLine(hStatic4, num, n);
            gn = n;
            gnum = new double[gn];
            for (int i = 0; i < gn; i++)
            {
                gnum[i] = num[i];
            }
            for (int i = 0; i < n; i++)
            {
                xn.resize(n);
                xn[i] = num[i];
            }

            Xk = fftr2c(xn);

            ofstream OutFile("SPS.txt");
            for (int i = 0; i < n; ++i)
            {
                num[i] = abs(Xk[i])* abs(Xk[i]);
                OutFile << setiosflags(ios::fixed) << setprecision(4);
                OutFile << num[i] << endl;
            }
            OutFile.close();

            LPCTSTR str = L" 数据个数:\n   %i\n";
            swprintf_s(szBuffer, str, n);

            DrawLine(hStatic5, num, n);
            gn1 = n;
            gnum1 = new double[gn1];
            CanvasStatus = 3;
            for (int i = 0; i < gn1; i++)
            {
                gnum1[i] = num[i];
            }
            SetWindowText(hStatic2, szBuffer);
            SetWindowText(hStatic8, NULL);
            ShowWindow(hStatic6, SW_SHOW);
            ShowWindow(hStatic7, SW_SHOW);
            InvalidateRect(hwnd, NULL, true);
            UpdateWindow(hwnd);
        }
    }

    if (nCmdID == cmdButton9 && verb == UI_EXECUTIONVERB_EXECUTE)
    {
        BtnNum = nCmdID;
        OPENFILENAME ofn;			// 公共对话框结构
        TCHAR szFile[MAX_PATH];		// 保存获取文件名称的缓冲区
        TCHAR szPath[MAX_PATH];
        TCHAR szFileName1[MAX_PATH];
        TCHAR szFileName2[MAX_PATH];
        TCHAR* p, * q;
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = L"Text\0*.TXT\0"; //过滤规则
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ALLOWMULTISELECT;

        if (GetOpenFileName(&ofn))
        {
            q = szFile + ofn.nFileOffset;
            int FileNum = 0;
            while (*q)
            {
                FileNum++;
                q = q + lstrlen(q) + 1;
            }
            if (FileNum == 2)
            {
                lstrcpyn(szPath, szFile, ofn.nFileOffset);
                lstrcat(szPath, TEXT("\\"));

                p = szFile + ofn.nFileOffset; //把指针移到第一个文件

                ZeroMemory(szFileName1, sizeof(szFileName1));
                lstrcat(szFileName1, szPath);  //给文件名加上路径  
                lstrcat(szFileName1, p);    //加上文件名

                q = p + lstrlen(p) + 1;

                ZeroMemory(szFileName2, sizeof(szFileName2));
                lstrcat(szFileName2, szPath);  //给文件名加上路径  
                lstrcat(szFileName2, q);    //加上文件名

                typedef double Type;
                Vector < complex<Type> > Xk, Yk, Zk;
                Vector <Type> xn, yn;


                ifstream fin;
                fin.open(szFileName1);
                int n = GetLine(fin);
                fin.close();
                gn = n;
                gn1 = n;
                gnum = new double[gn];
                gnum1 = new double[gn1];
                rnum = new double[gn];
                lnum = new double[gn1];
                anum = new double[gn];
                pnum = new double[gn1];
                fin.open(szFileName1);
                double* num = GetNum(fin, n);

                for (int i = 0; i < n; i++)
                {
                    xn.resize(n);
                    xn[i] = num[i];
                }
                for (int i = 0; i < n; i++)
                {
                    gnum[i] = num[i];
                }

                fin.close();

                fin.open(szFileName2);
                num = GetNum(fin, n);

                for (int i = 0; i < n; i++)
                {
                    yn.resize(n);
                    yn[i] = num[i];
                }
                for (int i = 0; i < n; i++)
                {
                    gnum1[i] = num[i];
                }

                fin.close();

                Xk = fftr2c(xn);
                Yk = fftr2c(yn);

                ofstream OutFile("CPS.txt");
                for (int i = 0; i < n; ++i)
                {
                    double xr = real(Xk[i]);
                    double xi = imag(Xk[i]); 
                    double yr = real(Yk[i]);
                    double yi = imag(Yk[i]);
                    Zk.resize(n);
                    Zk[i] = complex<Type>(xr * yr + xi * yi, - xr * yi + yr * xi);
                    OutFile << setiosflags(ios::fixed) << setprecision(4);
                    OutFile << Zk[i] << endl;
                }
                OutFile.close();
                LPCTSTR str = L" 数据个数:\n   %i\n";
                swprintf_s(szBuffer, str, n);

                ShowWindow(hStatic5, SW_SHOWDEFAULT);
                ShowWindow(hStatic7, SW_HIDE);
                ShowWindow(hStatic6, SW_HIDE);
                ShowWindow(hComboBox, SW_SHOW);
                SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)0, 0);
                CanvasStatus = 3;
                for (int i = 0; i < gn; i++)
                {
                    rnum[i] = real(Zk[i]);
                }
                for (int i = 0; i < gn1; i++)
                {
                    lnum[i] = imag(Zk[i]);
                }
                for (int i = 0; i < gn1; i++)
                {
                    anum[i] = abs(Zk[i]);
                }
                for (int i = 0; i < gn1; i++)
                {
                    pnum[i] = atan(imag(Zk[i])/ real(Zk[i]));
                }
                DrawLine(hStatic4, gnum, gn);
                DrawLine(hStatic5, gnum1, gn1);
                SetWindowText(hStatic2, szBuffer);
                SetWindowText(hStatic8, NULL);
                ShowWindow(hStatic6, SW_SHOW);
                ShowWindow(hStatic7, SW_SHOW);
                InvalidateRect(hwnd, NULL, true);
                UpdateWindow(hwnd);
            }
            else
            {
                MessageBox(NULL, L"请选择两个数据文件", L"提示", MB_OK);
            }
        }
    }

    if (nCmdID == cmdButton10 && verb == UI_EXECUTIONVERB_EXECUTE)
    {
        BtnNum = nCmdID;
        OPENFILENAME ofn;
        TCHAR szFile[MAX_PATH];
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = L"Text\0*.TXT\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn))
        {
            typedef double Type;
            Vector < complex<Type> > Xk, Yk;
            Vector <Type> xn, yn;

            ifstream fin;
            fin.open(szFile);
            int n = GetLine(fin);
            fin.close();

            fin.open(szFile);
            double* num = GetNum(fin, n);

            ShowWindow(hStatic5, SW_SHOWDEFAULT);
            ShowWindow(hStatic7, SW_HIDE);
            ShowWindow(hStatic6, SW_HIDE);
            ShowWindow(hComboBox, SW_HIDE);
            DrawLine(hStatic4, num, n);
            gn = n;
            gnum = new double[gn];
            for (int i = 0; i < gn; i++)
            {
                gnum[i] = num[i];
            }
            ofstream OutFile("CEP.txt");
            for (int i = 0; i < n; i++)
            {
                xn.resize(n);
                xn[i] = num[i];
            }

            Xk = fftr2c(xn);

            for (int i = 0; i < n; ++i)
            {
                num[i] = abs(Xk[i]);
                Yk.resize(n);
                Yk[i] = complex<Type>(log(num[i]), 0);
            }

            yn = ifftc2r(Yk);

            for (int i = 0; i < n; ++i)
            {
                OutFile << setiosflags(ios::fixed) << setprecision(4);
                OutFile << yn[i] << endl;
            }
            OutFile.close();
            LPCTSTR str = L" 数据个数:\n   %i\n";
            swprintf_s(szBuffer, str, n);

            DrawLine(hStatic5, yn, n);
            gn1 = n;
            gnum1 = new double[gn1];
            CanvasStatus = 3;
            for (int i = 0; i < gn1; i++)
            {
                gnum1[i] = yn[i];
            }
            SetWindowText(hStatic2, szBuffer);
            SetWindowText(hStatic8, NULL);
            ShowWindow(hStatic6, SW_SHOW);
            ShowWindow(hStatic7, SW_SHOW);
            InvalidateRect(hwnd, NULL, true);
            UpdateWindow(hwnd);
        }
    }

    if (nCmdID == cmdButton11 && verb == UI_EXECUTIONVERB_EXECUTE)
    {
        BtnNum = nCmdID;
        OPENFILENAME ofn;
        TCHAR szFile[MAX_PATH];
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = L"Text\0*.TXT\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn))
        {
            typedef long double Type;
            const int Lg = 100;
            const int Ls = 1000;
            const int Fs = 1000;

            Type c = 0;
            Type b = Ls - 1.0;
            Vector<Type> t = linspace(c, b, Ls) / Type(Fs);
            Vector<Type> s = sin(Type(400 * PI) * pow(t, Type(2.0)));
            //Vector<Type> s = 1;

            c = 0;
            b = Type(Lg - 1.0);
            Type u = (Lg - 1) / Type(2);
            Type r = Lg / Type(8);
            t = linspace(c, b, Lg);
            Vector<Type> g = gauss(t, u, r);
            g = g / norm(g);

            Matrix<complex<Type>> coefs = wft(s, g);
            ofstream OutFile("wft.txt");
            OutFile << coefs << endl;
            OutFile.close();

            LPCTSTR str = L"短时傅里叶变换已完成，数据文件已输出";
            swprintf_s(szBuffer, str);
            SetWindowText(hStatic2, szBuffer);
            SetWindowText(hStatic8, NULL);
        }        
    }

    if (nCmdID == cmdButton12 && verb == UI_EXECUTIONVERB_EXECUTE)
    {
        BtnNum = nCmdID;
        OPENFILENAME ofn;
        TCHAR szFile[MAX_PATH];
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = L"Text\0*.TXT\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn))
        {
            const int Ls = 1000;
            const double fs = 1000.0;

            Vector<double> t = linspace(0.0, (Ls - 1) / fs, Ls);
            Vector<double> st = sin(200 * PI * pow(t, 2.0));
            st = st - sum(st) / st.dim();
            //st = st - mean(st); //statistics.h

            CWT<double> wavelet("morlet");
            wavelet.setScales(fs, fs / Ls, fs / 2);
            Matrix<complex<double>> coefs = wavelet.cwtC(st);
            ofstream OutFile("Wavelet.txt");
            OutFile << coefs << endl;
            OutFile.close();

            LPCTSTR str = L"小波分析已完成，数据文件已输出";
            swprintf_s(szBuffer, str);
            SetWindowText(hStatic2, szBuffer);
            SetWindowText(hStatic8, NULL);
        }
    }

    if (nCmdID == cmdButton13 && verb == UI_EXECUTIONVERB_EXECUTE)
    {
        BtnNum = nCmdID;
        OPENFILENAME ofn;
        TCHAR szFile[MAX_PATH];
        TCHAR* p;
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = L"Text\0*.TXT\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn))
        {       
            int dbN = 1, stages = 3, numf = 1, j;
            float thrshold = 0.5;
            FILE* fp, * fp1;

            p = szFile + ofn.nFileOffset;
            char nchar[256] = { 0 };
            WideCharToMultiByte(CP_ACP, 0, p, -1, nchar, 256, NULL, NULL);
            const char* constc = nullptr;
            constc = nchar;

            if ((fp = fopen(constc, "r")) == NULL)
            {
                printf("cann't open input file\n");
                exit(-3);
            }
            for (j = 0; j < datalen; j++)
                fscanf_s(fp, "%f", &dataio[j]);

            ShowWindow(hStatic5, SW_SHOWDEFAULT);
            ShowWindow(hStatic7, SW_HIDE);
            ShowWindow(hStatic6, SW_HIDE);
            ShowWindow(hComboBox, SW_HIDE);
            gn = j;                                                         
            gnum = new double[gn];
            for (int i = 0; i < gn; i++)
            {
                gnum[i] = dataio[i];
            }
            DrawLine(hStatic4, gnum, gn);

            // wavelet transformation
            dwpt_filter(dbN, stages, numf, thrshold);

            // out put result
            if ((fp1 = fopen("WaveletPackOutput.txt", "w+")) == NULL)
            {
                printf("cann't open output file\n");
                exit(-4);
            }
            for (j = 0; j < datalen; j++)
                fprintf(fp1, "%10.4f\n", dataio[j]);

            fclose(fp);
            fclose(fp1);

            LPCTSTR str = L"小波包分析已完成，数据文件已输出";
            swprintf_s(szBuffer, str);

            gn1 = j;
            gnum1 = new double[gn1];
            for (int i = 0; i < gn1; i++)
            {
                gnum1[i] = dataio[i];
            }
            DrawLine(hStatic5, gnum1, gn1);

            CanvasStatus = 3;
            SetWindowText(hStatic2, szBuffer);
            SetWindowText(hStatic8, NULL);
            ShowWindow(hStatic6, SW_SHOW);
            ShowWindow(hStatic7, SW_SHOW);
            InvalidateRect(hwnd, NULL, true);
            UpdateWindow(hwnd);
        }
    }

    if (nCmdID == cmdButton14 && verb == UI_EXECUTIONVERB_EXECUTE)
    {
        BtnNum = nCmdID;
        OPENFILENAME ofn;
        TCHAR szFile[MAX_PATH];
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = L"Text\0*.TXT\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn))
        {
            int i, k;
            typedef double Type;
            Vector < complex<Type> > Xk, Yk;
            Vector <Type> xn;

            ifstream fin;
            fin.open(szFile);
            int n = GetLine(fin);
            fin.close();

            fin.open(szFile);
            double* s = GetNum(fin, n);

            ShowWindow(hStatic5, SW_SHOWDEFAULT);
            ShowWindow(hStatic7, SW_HIDE);
            ShowWindow(hStatic6, SW_HIDE);
            ShowWindow(hComboBox, SW_HIDE);

            DrawLine(hStatic4, s, n);
            gn = n;
            gnum = new double[gn];
            for (i = 0; i < gn; i++)
            {
                gnum[i] = s[i];
            }

            for (i = 0; i < n; i++)
            {
                xn.resize(n);
                xn[i] = s[i];
                s[i] = 0;
            }
            fin.close();

            Xk = fftr2c(xn);

            if ((n > 0) && ((2 * floor(n / 2)) == (n))) // n is even
            {
                s[0] = 1;
                s[n / 2] = 1;

                for (k = 1; k < n / 2; k++)
                {
                    s[k] = 2;
                }
            }
            else //n is odd
            {
                if (n > 0)
                {
                    s[0] = 1;

                    for (k = 1; k < (n + 1) / 2; k++)
                    {
                        s[k] = 2;
                    }
                }
            }

            for (i = 0; i < n; i++)
            {
                double xr = real(Xk[i]);
                double xi = imag(Xk[i]);
                xr = xr * s[i];
                xi = xi * s[i];
                Xk.resize(n);
                Xk[i] = complex<Type>(xr, xi);
            }

            Yk = ifftc2c(Xk);

            ofstream OutFile("Hilbert.txt");
            for (i = 0; i < n; i++)
            {
                double yi = imag(Yk[i]);
                OutFile << setiosflags(ios::fixed) << setprecision(4);
                OutFile << yi << endl;
            }
            OutFile.close();

            gn1 = n;
            gnum1 = new double[gn1];
            CanvasStatus = 3;
            for (i = 0; i < gn1; i++)
            {
                gnum1[i] = imag(Yk[i]);
            }
            DrawLine(hStatic5, gnum1, n);


            LPCTSTR str = L"希尔伯特变换已完成，数据文件已输出";
            swprintf_s(szBuffer, str);

            SetWindowText(hStatic2, szBuffer);
            SetWindowText(hStatic8, NULL);
            ShowWindow(hStatic6, SW_SHOW);
            ShowWindow(hStatic7, SW_SHOW);
            InvalidateRect(hwnd, NULL, true);
            UpdateWindow(hwnd);
        }
    }

    if (nCmdID == cmdButton15 && verb == UI_EXECUTIONVERB_EXECUTE)
    {
        BtnNum = nCmdID;
        OPENFILENAME ofn;
        TCHAR szFile[MAX_PATH];
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = L"Text\0*.TXT\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn))
        {           
            ifstream fin;
            fin.open(szFile);
            int n = GetLine(fin);
            fin.close();

            fin.open(szFile);
            double* s = GetNum(fin, n);

            ShowWindow(hStatic5, SW_SHOWDEFAULT);
            ShowWindow(hStatic7, SW_HIDE);
            ShowWindow(hStatic6, SW_HIDE);
            ShowWindow(hComboBox, SW_HIDE);

            DrawLine(hStatic4, s, n);
            gn = n;
            gnum = new double[gn];
            for (int i = 0; i < gn; i++)
            {
                gnum[i] = s[i];
            }

            double t[10240] = { 0 };
            double yp1, ypn;   //一阶导数值

            for (int i = 0; i < n; i++)
            {
                t[i] = 1.0*i / n;
            }

            yp1 = s[0] / t[0];
            ypn = s[n] / t[n];

            double* y2 = spline1(t, s, n, yp1, ypn);
            double yn[10240];

            ofstream OutFile("EMD.txt");
            for (int i = 0; i < n; i++)
            {
                double x = t[i];
                double y = spline2(t, s, y2, n, x);
                yn[i] = y;
                OutFile << setiosflags(ios::fixed) << setprecision(4);
                OutFile << yn[i] << endl;
            }          

            LPCTSTR str = L"经验模态已完成，数据文件已输出";
            swprintf_s(szBuffer, str);

            gn1 = n;
            gnum1 = new double[gn1];
            CanvasStatus = 3;
            for (int i = 0; i < gn1; i++)
            {
                gnum1[i] = yn[i];
            }
            DrawLine(hStatic5, gnum1, n);
            SetWindowText(hStatic2, szBuffer);
            SetWindowText(hStatic8, NULL);
            ShowWindow(hStatic6, SW_SHOW);
            ShowWindow(hStatic7, SW_SHOW);
            InvalidateRect(hwnd, NULL, true);
            UpdateWindow(hwnd);
        }
    }

    return S_OK;
}

STDMETHODIMP CSpectrumHandler::UpdateProperty(
    UINT nCmdID,
    REFPROPERTYKEY key,
    const PROPVARIANT* ppropvarCurrentValue,
    PROPVARIANT* ppropvarNewValue)
{
    UNREFERENCED_PARAMETER(nCmdID);
    UNREFERENCED_PARAMETER(key);
    UNREFERENCED_PARAMETER(ppropvarCurrentValue);
    UNREFERENCED_PARAMETER(ppropvarNewValue);

    return E_NOTIMPL;

}


HRESULT CSpectrumHandler::CreateInstance(CSpectrumHandler** ppCommandHandler)
{
    if (!ppCommandHandler)
    {
        return E_POINTER;
    }

    *ppCommandHandler = NULL;

    HRESULT hr = S_OK;

    CSpectrumHandler* pCommandHandler = new CSpectrumHandler();

    if (pCommandHandler != NULL)
    {
        *ppCommandHandler = pCommandHandler;

    }
    else
    {
        hr = E_OUTOFMEMORY;
    }

    return hr;
}

STDMETHODIMP_(ULONG) CSpectrumHandler::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CSpectrumHandler::Release()
{
    LONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}

STDMETHODIMP CSpectrumHandler::QueryInterface(REFIID iid, void** ppv)
{
    if (!ppv)
    {
        return E_POINTER;
    }

    if (iid == __uuidof(IUnknown))
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == __uuidof(IUICommandHandler))
    {
        *ppv = static_cast<IUICommandHandler*>(this);
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}
