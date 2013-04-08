#pragma once

#include <cscobj.h>

class COfflineFilesEvents : IOfflineFilesEvents, IOfflineFilesEventsFilter
{
public:
	COfflineFilesEvents(void);
	~COfflineFilesEvents(void);

	// IUnknown Metehods
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppvObject)
    {        
		if (iid == IID_IOfflineFilesEvents)
        {
            m_dwRefCount++;
            *ppvObject = (void *)this;
            return S_OK;
        }

        if (iid == IID_IUnknown)
        {
            m_dwRefCount++;            
            *ppvObject = (void *)this;
            return S_OK;
        }

        return E_NOINTERFACE;
    }

		ULONG STDMETHODCALLTYPE AddRef()
    {
        m_dwRefCount++;
        return m_dwRefCount;
    }
    
	ULONG STDMETHODCALLTYPE Release()
    {
        ULONG l;
        
        l  = m_dwRefCount--;

        if ( 0 == m_dwRefCount)
        {
            delete this;
        }
        
        return l;
    }
	// End IUnknown Metehods

	// IOfflineFileEvents Methods
	virtual HRESULT STDMETHODCALLTYPE CacheMoved( LPCWSTR pszOldPath,  LPCWSTR pszNewPath)
	{
		printf("CacheMoved Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE CacheIsFull(void)
	{
		printf("CacheIsFull Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE CacheIsCorrupted( void)
	{
		printf("CacheIsCorrupted Event\r\n");
		return S_OK;
	}
       
    virtual HRESULT STDMETHODCALLTYPE Enabled( BOOL bEnabled)
	{
		printf("Enabled Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE EncryptionChanged( BOOL bWasEncrypted, BOOL bWasPartial, BOOL bIsEncrypted, BOOL bIsPartial)
	{
		printf("EncryptionChanged Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE SyncBegin(REFGUID rSyncId)
	{
		printf("SyncBegin Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE SyncFileResult(REFGUID rSyncId, LPCWSTR pszFile, HRESULT hrResult)
	{
		printf("SyncFileResult Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE SyncConflictRecAdded( LPCWSTR pszConflictPath,  const FILETIME *pftConflictDateTime, OFFLINEFILES_SYNC_STATE ConflictSyncState)
	{
		printf("SyncConflictRecAdded Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE SyncConflictRecUpdated( LPCWSTR pszConflictPath, const FILETIME *pftConflictDateTime, OFFLINEFILES_SYNC_STATE ConflictSyncState)
	{
		printf("SyncConflictRecUpdated Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE SyncConflictRecRemoved(  LPCWSTR pszConflictPath, const FILETIME *pftConflictDateTime,  OFFLINEFILES_SYNC_STATE ConflictSyncState)
	{
		printf("SyncConflictRecRemoved Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE SyncEnd(  REFGUID rSyncId, HRESULT hrResult)
	{
		printf("SyncEnd Event\r\n");
		return S_OK;
	}

        
    virtual HRESULT STDMETHODCALLTYPE NetTransportArrived( void)
	{
		printf("NetTransportArrived Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE NoNetTransports( void)
	{
		printf("NoNetTransports Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE ItemDisconnected( LPCWSTR pszPath, OFFLINEFILES_ITEM_TYPE ItemType)
	{
		printf("ItemDisconnected Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE ItemReconnected( LPCWSTR pszPath, OFFLINEFILES_ITEM_TYPE ItemType)
	{
		printf("ItemReconnected Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE ItemAvailableOffline( LPCWSTR pszPath, OFFLINEFILES_ITEM_TYPE ItemType)
	{
		printf("ItemAvailableOffline Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE ItemNotAvailableOffline(  LPCWSTR pszPath, OFFLINEFILES_ITEM_TYPE ItemType)
	{
		printf("ItemNotAvailableOffline Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE ItemPinned( LPCWSTR pszPath, OFFLINEFILES_ITEM_TYPE ItemType)
	{
		printf("ItemPinned Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE ItemNotPinned(  LPCWSTR pszPath, OFFLINEFILES_ITEM_TYPE ItemType)
	{
		printf("ItemNotPinned Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE ItemModified(  LPCWSTR pszPath, OFFLINEFILES_ITEM_TYPE ItemType, BOOL bModifiedData, BOOL bModifiedAttributes)
	{
		printf("ItemModified Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE ItemAddedToCache( LPCWSTR pszPath, OFFLINEFILES_ITEM_TYPE ItemType)
	{
		printf("ItemAddedToCache Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE ItemDeletedFromCache( LPCWSTR pszPath, OFFLINEFILES_ITEM_TYPE ItemType)
	{
		printf("ItemDeletedFromCache Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE ItemRenamed( LPCWSTR pszOldPath, LPCWSTR pszNewPath, OFFLINEFILES_ITEM_TYPE ItemType)
	{
		printf("ItemRenamed Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE DataLost( void)
	{
		printf("DataLost Event\r\n");
		return S_OK;
	}
        
    virtual HRESULT STDMETHODCALLTYPE Ping( void)
	{
		printf("Ping Event\r\n");
		return S_OK;
	}

	// End IOfflineFileEvents Methods

	//IOfflineFileEventFilters Methods
	virtual HRESULT STDMETHODCALLTYPE GetPathFilter( LPWSTR *ppszFilter, OFFLINEFILES_PATHFILTER_MATCH *pMatch)
	{
		return E_NOTIMPL;
	}
        
	virtual HRESULT STDMETHODCALLTYPE GetIncludedEvents( ULONG cElements, OFFLINEFILES_EVENTS *prgEvents, ULONG *pcEvents)
	{
		return E_NOTIMPL;
	}
        
	virtual HRESULT STDMETHODCALLTYPE GetExcludedEvents( ULONG cElements, OFFLINEFILES_EVENTS *prgEvents, ULONG *pcEvents)
	{
		return E_NOTIMPL;
	}

	private:
		DWORD m_dwRefCount;
};

