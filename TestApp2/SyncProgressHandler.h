#pragma once
#include "cscobj.h"
class CSyncProgressHandler :
	public IOfflineFilesSyncProgress
{
public:
	CSyncProgressHandler(void);
	~CSyncProgressHandler(void);

	//IOfflineFilesSyncProgress Methods
	HRESULT STDMETHODCALLTYPE SyncItemBegin(LPCWSTR pszFile, OFFLINEFILES_OP_RESPONSE *pResponse);
	HRESULT STDMETHODCALLTYPE SyncItemResult(LPCWSTR pszFile, HRESULT hrResult, IOfflineFilesSyncErrorInfo *pErrorInfo, OFFLINEFILES_OP_RESPONSE *pResponse);

	HRESULT STDMETHODCALLTYPE Begin(BOOL *pbAbort);
	HRESULT STDMETHODCALLTYPE End(HRESULT hrResult);
	HRESULT STDMETHODCALLTYPE QueryAbort(BOOL *pbAbort);

	//IUnknown Methods
	HRESULT STDMETHODCALLTYPE QueryInterface (REFIID   riid, LPVOID * ppvObj);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
private:
	ULONG m_cRef;
};

