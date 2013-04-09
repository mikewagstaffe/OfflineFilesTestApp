#include "stdafx.h"
#include "SyncProgressHandler.h"


CSyncProgressHandler::CSyncProgressHandler(void)
{
	m_cRef = 0;
}


CSyncProgressHandler::~CSyncProgressHandler(void)
{
}

HRESULT CSyncProgressHandler::SyncItemBegin(LPCWSTR pszFile, OFFLINEFILES_OP_RESPONSE *pResponse)
{
}

HRESULT CSyncProgressHandler::SyncItemResult(LPCWSTR pszFile, HRESULT hrResult, IOfflineFilesSyncErrorInfo *pErrorInfo, OFFLINEFILES_OP_RESPONSE *pResponse)
{
}


HRESULT CSyncProgressHandler::QueryInterface (REFIID   riid, LPVOID * ppvObj)
{
    // Always set out parameter to NULL, validating it first.
    if (!ppvObj)
	{
        return E_INVALIDARG;
	}
	*ppvObj = NULL;
    if (riid == IID_IUnknown || riid == IID_IOfflineFilesSyncConflictHandler)
    {
        // Increment the reference count and return the pointer.
        *ppvObj = (LPVOID)this;
        AddRef();
        return NOERROR;
    }
    return E_NOINTERFACE;
}

ULONG CSyncProgressHandler::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}
ULONG CSyncProgressHandler::Release()
{
    // Decrement the object's internal counter.
    ULONG ulRefCount = InterlockedDecrement(&m_cRef);
    if (0 == m_cRef)
    {
        delete this;
    }
    return ulRefCount;
}