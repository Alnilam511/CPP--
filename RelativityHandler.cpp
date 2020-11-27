#include <UIRibbon.h>

#include <fstream>   //提供文件头文件
#include "GlobalVariable.h"
#include "Functions.h"

#include "RelativityHandler.h"
#include "SimpleRibbonUI.h"
#include "RibbonFramework.h"

using namespace std;

STDMETHODIMP CRelativityHandler::Execute(
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
    
    if (nCmdID == cmdButton6 && verb == UI_EXECUTIONVERB_EXECUTE)
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
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

        if (GetOpenFileName(&ofn))
        {            
            ifstream fin;
            fin.open(szFile);
            int n = GetLine(fin);
            fin.close();

            fin.open(szFile);
            double* num = GetNum(fin, n);

            double R = AutoCorrelation(num, n);

            LPCTSTR str = L" 数据个数:\n   %i\n 自相关系数为:\n   %.2f";
            swprintf_s(szBuffer, str, n, R);

            ShowWindow(hStatic5, SW_HIDE);
            DrawLine(hStatic4, num, n);                                //固定波形绘制
            gnum = num;
            gn = n;
        }

    }

    if (nCmdID == cmdButton7 && verb == UI_EXECUTIONVERB_EXECUTE)
    {
        a = nCmdID;
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

                p = p + lstrlen(p) + 1;

                ZeroMemory(szFileName2, sizeof(szFileName2));
                lstrcat(szFileName2, szPath);  //给文件名加上路径  
                lstrcat(szFileName2, p);    //加上文件名

                ifstream fin1, fin2;
                //获取文件数据个数
                fin1.open(szFileName1);
                int n1 = GetLine(fin1);
                fin1.close();

                fin2.open(szFileName2);
                int n2 = GetLine(fin2);
                fin2.close();

                double num1[10240] = { 0 }, num2[10240] = { 0 };
                //对数组赋值
                fin1.open(szFileName1);
                for (int i = 0; i < n1; i++)
                {
                    fin1 >> num1[i];
                }
                fin1.close();

                fin2.open(szFileName2);
                for (int i = 0; i < n2; i++)
                {
                    fin2 >> num2[i];
                }
                fin2.close();

                double R = CrossCorrelation(num1, num2, n1, n2);

                LPCTSTR str = L" 数据个数:\n   %i\n 互相关系数:\n   %.2f";
                swprintf_s(szBuffer, str, n1, R);
                ShowWindow(hStatic5, SW_SHOWDEFAULT);
                DrawLine(hStatic4, num1, n1);
                DrawLine(hStatic5, num2, n2);

            }
            else
            {
                MessageBox(NULL, L"请选择两个数据文件", L"提示", MB_OK);
            }            
        }
    }
    return S_OK;
}

STDMETHODIMP CRelativityHandler::UpdateProperty(
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


HRESULT CRelativityHandler::CreateInstance(CRelativityHandler** ppCommandHandler)
{
    if (!ppCommandHandler)
    {
        return E_POINTER;
    }

    *ppCommandHandler = NULL;

    HRESULT hr = S_OK;

    CRelativityHandler* pCommandHandler = new CRelativityHandler();

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

STDMETHODIMP_(ULONG) CRelativityHandler::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CRelativityHandler::Release()
{
    LONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}

STDMETHODIMP CRelativityHandler::QueryInterface(REFIID iid, void** ppv)
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



