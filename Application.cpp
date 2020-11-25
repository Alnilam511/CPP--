
#include <UIRibbon.h>
#include <UIRibbonPropertyHelpers.h>

#include "Application.h"
#include "SimpleRibbonUI.h"
#include "CommandHandler.h"
#include "RelativityHandler.h"
#include "SpectrumHandler.h"


STDMETHODIMP CApplication::OnViewChanged(
    UINT32 nviewID,
    UI_VIEWTYPE typeID,
    IUnknown* pView,
    UI_VIEWVERB verb,
    INT32 uReasonCode)
{
    UNREFERENCED_PARAMETER(uReasonCode);
    UNREFERENCED_PARAMETER(nviewID);

    HRESULT hr = E_NOTIMPL;

    if (UI_VIEWTYPE_RIBBON == typeID)
    {
        switch (verb)
        {
            // The view was newly created.
        case UI_VIEWVERB_CREATE:
            hr = S_OK;
            break;

        case UI_VIEWVERB_SIZE:
        {
            IUIRibbon* pRibbon = NULL;
            UINT uRibbonHeight;

            hr = pView->QueryInterface(IID_PPV_ARGS(&pRibbon));
            if (SUCCEEDED(hr))
            {
                hr = pRibbon->GetHeight(&uRibbonHeight);
                pRibbon->Release();
            }
        }
        break;
        case UI_VIEWVERB_DESTROY:
            hr = S_OK;
            break;
        }
    }

    return hr;
}

STDMETHODIMP CApplication::OnCreateUICommand(
    UINT32 nCmdID,
    UI_COMMANDTYPE typeID,
    IUICommandHandler** ppCommandHandler)
{
    UNREFERENCED_PARAMETER(typeID);

    HRESULT hr = E_FAIL;
    switch (nCmdID)
    {
    case cmdButton3:
    case cmdButton4:
    case cmdButton5:
    {
        CCommandHandler* pCommandHandler = NULL;
        hr = CCommandHandler::CreateInstance(&pCommandHandler);
        if (SUCCEEDED(hr))
        {
            hr = pCommandHandler->QueryInterface(IID_PPV_ARGS(ppCommandHandler));
            pCommandHandler->Release();
        }
        break;
    }
    case cmdButton6:
    case cmdButton7:
    {
        CRelativityHandler* pRelativityHandler = NULL;
        hr = CRelativityHandler::CreateInstance(&pRelativityHandler);
        if (SUCCEEDED(hr))
        {
            hr = pRelativityHandler->QueryInterface(IID_PPV_ARGS(ppCommandHandler));
            pRelativityHandler->Release();
        }
        break;
    }
    case cmdButton8:
    case cmdButton9:
    case cmdButton10:
    case cmdButton11:
    case cmdButton12:
    case cmdButton13:
    case cmdButton14:
    case cmdButton15:
    case cmdButton16:
    {
        CSpectrumHandler* pSpectrumHandler = NULL;
        hr = CSpectrumHandler::CreateInstance(&pSpectrumHandler);
        if (SUCCEEDED(hr))
        {
            hr = pSpectrumHandler->QueryInterface(IID_PPV_ARGS(ppCommandHandler));
            pSpectrumHandler->Release();
        }
        break;
    }

    }

    return hr;
}

STDMETHODIMP CApplication::OnDestroyUICommand(
    UINT32 nCmdID,
    UI_COMMANDTYPE typeID,
    IUICommandHandler* pCommandHandler)
{
    UNREFERENCED_PARAMETER(pCommandHandler);
    UNREFERENCED_PARAMETER(typeID);
    UNREFERENCED_PARAMETER(nCmdID);

    return E_NOTIMPL;
}

HRESULT CApplication::CreateInstance(CApplication **ppApplication, HWND hwnd)
{
    if (!ppApplication)
    {
        return E_POINTER;
    }
    if (!hwnd)
    {
        return E_INVALIDARG;
    }

    *ppApplication = NULL;

    HRESULT hr = S_OK;
   
    CApplication* pApplication = new CApplication();

    if (pApplication != NULL)
    {
        pApplication->m_hwnd = hwnd;
        *ppApplication = pApplication;
    }
    else
    {
        hr = E_OUTOFMEMORY;
    }

    return hr;
}

STDMETHODIMP_(ULONG) CApplication::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CApplication::Release()
{
    LONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}

STDMETHODIMP CApplication::QueryInterface(REFIID iid, void** ppv)
{
    if (!ppv)
    {
        return E_POINTER;
    }

    if (iid == __uuidof(IUnknown))
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == __uuidof(IUIApplication))
    {
        *ppv = static_cast<IUIApplication*>(this);
    }
    else 
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}
