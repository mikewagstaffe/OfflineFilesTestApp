#pragma once
#include "cscobj.h"
class CSyncProgressHandler :
	public IOfflineFilesSyncProgress
{
public:
	CSyncProgressHandler(void);
	~CSyncProgressHandler(void);

	HRESULT SyncItemBegin(LPCWSTR pszFile, OFFLINEFILES_OP_RESPONSE *pResponse);
	HRESULT SyncItemResult(LPCWSTR pszFile, HRESULT hrResult, IOfflineFilesSyncErrorInfo *pErrorInfo, OFFLINEFILES_OP_RESPONSE *pResponse);


	HRESULT QueryInterface (REFIID   riid, LPVOID * ppvObj);
	ULONG AddRef();
	ULONG Release();
private:
	ULONG m_cRef;
};

