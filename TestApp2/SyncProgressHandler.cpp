#include "stdafx.h"
#include "SyncProgressHandler.h"


CSyncProgressHandler::CSyncProgressHandler(void)
{
	m_cRef = 0;
}


CSyncProgressHandler::~CSyncProgressHandler(void)
{
}

HRESULT STDMETHODCALLTYPE CSyncProgressHandler::SyncItemBegin(LPCWSTR pszFile, OFFLINEFILES_OP_RESPONSE *pResponse)
{
	//wprintf(L"SyncItemBegin: %s\n\r",pszFile);
	 *pResponse = OFFLINEFILES_OP_CONTINUE;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSyncProgressHandler::SyncItemResult(LPCWSTR pszFile, HRESULT hrResult, IOfflineFilesSyncErrorInfo *pErrorInfo, OFFLINEFILES_OP_RESPONSE *pResponse)
{
	//wprintf(L"SyncItemResult Result is: :%d file is%s\n\r",hrResult, pszFile);
	 *pResponse = OFFLINEFILES_OP_CONTINUE;
	return S_OK;
}


HRESULT STDMETHODCALLTYPE CSyncProgressHandler::Begin(BOOL *pbAbort)
{
	//printf("Progress Handler Begin Called\n\r");

	*pbAbort = FALSE;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSyncProgressHandler::End(HRESULT hrResult)
{
	//printf("Progress Handler End Called Result:%d\n\r",hrResult);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSyncProgressHandler::QueryAbort(BOOL *pbAbort)
{
	//printf("Progress Handler QueryAbort Called\n\r");
	*pbAbort = FALSE;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSyncProgressHandler::QueryInterface (REFIID   riid, LPVOID * ppvObj)
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

ULONG STDMETHODCALLTYPE CSyncProgressHandler::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}
ULONG STDMETHODCALLTYPE CSyncProgressHandler::Release()
{
    // Decrement the object's internal counter.
    ULONG ulRefCount = InterlockedDecrement(&m_cRef);
    if (0 == m_cRef)
    {
        delete this;
    }
    return ulRefCount;
}