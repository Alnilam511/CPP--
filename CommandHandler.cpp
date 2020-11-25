
#include <UIRibbon.h>

#include <fstream>   //提供文件头文件
#include "GlobalVariable.h"
#include "Functions.h"

#include "CommandHandler.h"
#include "SimpleRibbonUI.h"
#include "RibbonFramework.h"

using namespace std;

 HWND hStatic4;
 HWND hStatic5;
 HWND hStatic6;
 HWND hStatic7;
 RECT rect;
 HPEN hpen;
 double* gnum;
 double* gnum1;
 double* gnum2;
 int gn;
 int gn1;
 int gn2;
 POINT* aptt;



void DrawLine(HWND hWnd, double* num, int n)                          //固定波形绘制，num波形数组，n数组长度
{
    HDC hdc = GetDC(hWnd);
    GetClientRect(hWnd, &rect);
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    SetBkColor(hdc, RGB(0, 0, 0));
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
    hpen = CreatePen(PS_DASHDOT, 1, RGB(50, 50, 50));
    SelectObject(hdc, hpen);
    int i = rect.bottom / 10;  int j = rect.right / 10;
    while (i < rect.bottom)
    {
        MoveToEx(hdc, 0, i, NULL);
        LineTo(hdc, rect.right, i);
        i = i + rect.bottom / 10;
    }
    while (j < rect.right)
    {
        MoveToEx(hdc, j, 0, NULL);
        LineTo(hdc, j, rect.bottom);
        j = j + rect.right / 10;
    }
    DeleteObject(hpen);
    if (num != NULL)
    {
        hpen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
        SelectObject(hdc, hpen);
        int k = 0;
        aptt = new POINT[n];
        double min = num[0], max = num[0];
        for (int l = 1; l < n; l++)
        {
            max = Maximum(max, num[l]);
            min = Minimum(min, num[l]);
        }
        max = Maximum(fabs(max), fabs(min));
        while (k <= n)
        {
            aptt[k].x = k * rect.right / n;
            aptt[k].y = (long)((num[k - 1]) * rect.bottom / (max * 2)) + rect.bottom / 2;
            k++;
        }
        Polyline(hdc, aptt, n);
        DeleteObject(hpen);
    }

}

STDMETHODIMP CCommandHandler::Execute(
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

    if (nCmdID == cmdButton3 && verb == UI_EXECUTIONVERB_EXECUTE)
    {
        a = nCmdID;
        OPENFILENAME ofn;			// 公共对话框结构
        TCHAR szFile[MAX_PATH];		// 保存获取文件名称的缓冲区
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
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn))
        {
            ifstream fin;
            fin.open(szFile);
            int n = GetLine(fin);
            fin.close();

            fin.open(szFile);
            double* num = GetNum(fin, n);

            double min = num[0], max = num[0];
            for (int i = 1; i < n; i++)
            {
                max = Maximum(max, num[i]);
                min = Minimum(min, num[i]);
            }
            
            LPCTSTR str = L" 数据个数:\n   %i\n 峰值为:\n   %e\n   %e";
            swprintf_s(szBuffer, str, n, max, min);

            ShowWindow(hStatic5,SW_HIDE);
            DrawLine(hStatic4, num, n);                                //固定波形绘制
            gnum = num;
            gn = n;
        }

    }

    if (nCmdID == cmdButton4 && verb == UI_EXECUTIONVERB_EXECUTE)
    {
        a = nCmdID;
        OPENFILENAME ofn;			// 公共对话框结构
        TCHAR szFile[MAX_PATH];		// 保存获取文件名称的缓冲区
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
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn))
        {
            ifstream fin;
            fin.open(szFile);
            int n = GetLine(fin);
            fin.close();

            fin.open(szFile);
            double* num = GetNum(fin, n);

            double average = Average(num, n);
            double variance = Variance(num, n);

            LPCTSTR str = L" 数据个数:\n   %i\n 均值:\n   %e\n 方差:\n   %e";
            swprintf_s(szBuffer, str, n, average, variance);
            DrawLine(hStatic4, num, n);
        }

    }

    if (nCmdID == cmdButton5 && verb == UI_EXECUTIONVERB_EXECUTE)
    {
        a = nCmdID;
        OPENFILENAME ofn;			// 公共对话框结构
        TCHAR szFile[MAX_PATH];		// 保存获取文件名称的缓冲区
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
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn))
        {
            ifstream fin;
            fin.open(szFile);
            int n = GetLine(fin);
            fin.close();

            fin.open(szFile);
            double* num = GetNum(fin, n);

            double kurtosis = Kurtosis(num, n);

            LPCTSTR str = L" 数据个数:\n   %i\n 峭度:\n   %e";
            swprintf_s(szBuffer, str, n, kurtosis);
            DrawLine(hStatic4, num, n);
        }

    }
    return S_OK;
}

STDMETHODIMP CCommandHandler::UpdateProperty(
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

HRESULT CCommandHandler::CreateInstance(CCommandHandler **ppCommandHandler)
{
    if (!ppCommandHandler)
    {
        return E_POINTER;
    }

    *ppCommandHandler = NULL;

    HRESULT hr = S_OK;
   
    CCommandHandler* pCommandHandler = new CCommandHandler();

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

STDMETHODIMP_(ULONG) CCommandHandler::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CCommandHandler::Release()
{
    LONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}

STDMETHODIMP CCommandHandler::QueryInterface(REFIID iid, void** ppv)
{
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
