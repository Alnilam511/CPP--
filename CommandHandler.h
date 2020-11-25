#pragma once

class CCommandHandler
    : public IUICommandHandler 
{
public:
    STDMETHOD(Execute)(UINT nCmdID,
        UI_EXECUTIONVERB verb,
        const PROPERTYKEY* key,
        const PROPVARIANT* ppropvarValue,
        IUISimplePropertySet* pCommandExecutionProperties);

    STDMETHOD(UpdateProperty)(UINT nCmdID,
        REFPROPERTYKEY key,
        const PROPVARIANT* ppropvarCurrentValue,
        PROPVARIANT* ppropvarNewValue);

    static HRESULT CreateInstance(CCommandHandler **ppCommandHandler);

    IFACEMETHODIMP QueryInterface(REFIID iid, void** ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

private:
    CCommandHandler()
        : m_cRef(1) 
    {
    }

    LONG m_cRef;
};
