#include "stdafx.h"
#include "SyncConflictHandler.h"

CSyncConflictHandler::CSyncConflictHandler(void)
{
	m_cRef = 0;
}


CSyncConflictHandler::~CSyncConflictHandler(void)
{
}

HRESULT STDMETHODCALLTYPE CSyncConflictHandler::ResolveConflict(LPCWSTR pszPath, DWORD fStateKnown, OFFLINEFILES_SYNC_STATE state, DWORD fChangeDetails, OFFLINEFILES_SYNC_CONFLICT_RESOLVE *pConflictResolution,LPWSTR *ppszNewName)
{
	//wprintf(L"Conflict Occured Just Log for now: %s\n\r",pszPath);

	*pConflictResolution = OFFLINEFILES_SYNC_CONFLICT_RESOLVE_LOG; //For now just log
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSyncConflictHandler::QueryInterface (REFIID   riid, LPVOID * ppvObj)
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

ULONG STDMETHODCALLTYPE CSyncConflictHandler::AddRef()
{
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}
ULONG STDMETHODCALLTYPE CSyncConflictHandler::Release()
{
    // Decrement the object's internal counter.
    ULONG ulRefCount = InterlockedDecrement(&m_cRef);
    if (0 == m_cRef)
    {
        delete this;
    }
    return ulRefCount;
}