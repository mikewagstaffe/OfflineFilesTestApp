#pragma once

enum OFFLINEFILESCLIENT_INIT_STATE
{
	OFFLINEFILESCLIENT_INIT_STATE_NOT_ENABLED,
	OFFLINEFILESCLIENT_INIT_STATE_RESTART_PENDING,
	OFFLINEFILESCLIENT_INIT_STATE_START_REQUIRED,
	OFFLINEFILESCLIENT_INIT_STATE_STARTED
};

#include <cscapi.h>
#include <cscobj.h>

#include "OfflineFilesEvents.h"

class COfflineFilesClient
{
public:
	COfflineFilesClient(void);
	~COfflineFilesClient(void);
	BOOL Init(void);
	OFFLINEFILESCLIENT_INIT_STATE GetInitialiseState(void);
	BOOL InitCache(LPCWSTR *ppszCachePath);
	BOOL RegisterSyncEvents();
	BOOL Synchronise();
	void Cleanup(void);

private:
	void PrintLocation(void);
	void PrintEncryptionStatus(void);
	void PrintDiskSpaceInfo(void);
	void PrintSettings(void);
	BOOL CacheStatus(void);
private:
	LPCWSTR m_pszCachePath;

	//Initialisation Flags, current state of Offline files
	BOOL m_bIsOfflineFilesActive;
	BOOL m_bIsOfflineFilesEnabled;
	BOOL m_bStartupReebotRequired;
	
	//Offline File Cache Members
	IOfflineFilesCache *m_pOfflineFilesCache;
	BOOL m_bCacheable;
	OFFLINEFILES_CACHING_MODE m_ShareCachingMode;

	//Events Members
	IConnectionPoint  *m_pIConnectionPoint;
	IUnknown *m_pSinkUnk;
	DWORD m_dwAdvise;
	COfflineFilesEvents *m_pOfflineFilesEvents;

};

