
#pragma once

class CApplication
    : public IUIApplication 
{
public:
    STDMETHOD(OnViewChanged)(UINT32 nviewID,
        UI_VIEWTYPE typeID,
        IUnknown* pView,
        UI_VIEWVERB verb,
        INT32 uReasonCode);

    STDMETHOD(OnCreateUICommand)(UINT32 nCmdID,
        UI_COMMANDTYPE typeID,
        IUICommandHandler** ppCommandHandler);

    STDMETHOD(OnDestroyUICommand)(UINT32 commandId,
        UI_COMMANDTYPE typeID,
        IUICommandHandler* commandHandler);

    static HRESULT CreateInstance(CApplication **ppApplication, HWND hwnd);

    IFACEMETHODIMP QueryInterface(REFIID iid, void** ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

private:
    CApplication() 
        : m_cRef(1)
        , m_hwnd(NULL)
    {
    }

    LONG m_cRef;                            
    HWND m_hwnd;
};
