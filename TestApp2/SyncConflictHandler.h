#pragma once
#include "cscobj.h"
class CSyncConflictHandler :
	public IOfflineFilesSyncConflictHandler
{
public:
	CSyncConflictHandler(void);
	~CSyncConflictHandler(void);

	HRESULT ResolveConflict(LPCWSTR pszPath, DWORD fStateKnown, OFFLINEFILES_SYNC_STATE state, DWORD fChangeDetails, OFFLINEFILES_SYNC_CONFLICT_RESOLVE *pConflictResolution,LPWSTR *ppszNewName);
	HRESULT QueryInterface (REFIID   riid, LPVOID * ppvObj);
	ULONG AddRef();
	ULONG Release();
private:
	ULONG m_cRef;
};

