#pragma once
#include "cscobj.h"
class CSyncConflictHandler :
	public IOfflineFilesSyncConflictHandler
{
public:
	CSyncConflictHandler(void);
	~CSyncConflictHandler(void);

	//IOfflineFilesSyncConflictHandler Methods
	HRESULT STDMETHODCALLTYPE ResolveConflict(LPCWSTR pszPath, DWORD fStateKnown, OFFLINEFILES_SYNC_STATE state, DWORD fChangeDetails, OFFLINEFILES_SYNC_CONFLICT_RESOLVE *pConflictResolution,LPWSTR *ppszNewName);
	
	//IUnknown Methods
	HRESULT STDMETHODCALLTYPE QueryInterface (REFIID   riid, LPVOID * ppvObj);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
private:
	ULONG m_cRef;
};

