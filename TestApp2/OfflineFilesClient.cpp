#include "stdafx.h"
#include "OfflineFilesClient.h"

#define SYNC_OPTIONS OFFLINEFILES_SYNC_CONTROL_FLAG_SYNCIN \
					+ OFFLINEFILES_SYNC_CONTROL_FLAG_SYNCOUT \
					+ OFFLINEFILES_SYNC_CONTROL_FLAG_ASYNCPROGRESS \
					+ OFFLINEFILES_SYNC_CONTROL_FLAG_PINNEWFILES \
					+ OFFLINEFILES_SYNC_CONTROL_FLAG_PINFORUSER

COfflineFilesClient::COfflineFilesClient(void)
{
	m_bIsOfflineFilesActive = FALSE;
	m_bIsOfflineFilesEnabled = FALSE;
	m_bStartupReebotRequired = FALSE;
	m_bCacheable=FALSE;
	m_pOfflineFilesCache = NULL;
	m_pOfflineFilesItem = NULL;
	m_pOfflineFilesConnecionInfo = NULL;
	m_ShareCachingMode = OFFLINEFILES_CACHING_MODE_NONE;
	m_pIConnectionPoint = NULL;
	m_pOfflineFilesEvents = NULL;
	m_pSinkUnk = NULL;
	m_dwAdvise = 0;
	m_pszCachePath = NULL;
	m_bOfflineOnlineTranistionRequired = FALSE;
}


COfflineFilesClient::~COfflineFilesClient(void)
{
	Cleanup();
}

BOOL COfflineFilesClient::Init(void)
{
	HRESULT hr = CoInitializeEx(NULL,COINITBASE_MULTITHREADED); 
	if (hr != S_OK)
	{
		printf("Failed To initialise COM \r\n");
	}
	else //Co Initialise Succeded
	{
		printf("COM Is Initialised \r\n");
		printf("Getting status Of Offline Files\r\n");
		if ( OfflineFilesQueryStatus(&m_bIsOfflineFilesActive,&m_bIsOfflineFilesEnabled) != ERROR_SUCCESS)
		{
			printf("Failed To Get Offline Files Status: %d\r\n",GetLastError());
		}
		else //Obtained the current status of Offline Files
		{
			printf("Offline Files Is Active: %s\r\n",(m_bIsOfflineFilesActive ? "TRUE" : "FALSE"));
			printf("Offline Files Is Enabled: %s\r\n",(m_bIsOfflineFilesEnabled ? "TRUE" : "FALSE"));

			if (!m_bIsOfflineFilesEnabled) //If not enabled Enable Offline Files try and enable it
			{
				DWORD dwResult = OfflineFilesEnable(TRUE, &m_bStartupReebotRequired);
				if (dwResult == ERROR_SUCCESS)
				{
					printf("Offline Files Is Now Enabled, Is Reeboot required: %s\r\n",(m_bStartupReebotRequired ? "TRUE" : "FALSE"));
					if (!m_bStartupReebotRequired)
					{
						m_bIsOfflineFilesEnabled = TRUE;
					}
				}
				else //Failed To Enable Offline Files
				{
					printf("Failed To Enable Offline Files: %d\r\n",dwResult);
				}
			}

			//If Offline Files is enabled but not Started start it up as long as we are not pending a reboot
			if (!m_bIsOfflineFilesActive && m_bIsOfflineFilesEnabled && !m_bStartupReebotRequired)
			{
				//Windows 7 Does not support starting of Offline Files
				printf("Failed To Start Offline Files\r\n");
			}
		}
	}

	//Create an instance of the Offline Files Cache
	if( m_bIsOfflineFilesEnabled && m_bIsOfflineFilesActive)
	{
		printf("Creating Offline Files Cache Instance\r\n");
		hr = CoCreateInstance(CLSID_OfflineFilesCache, NULL, CLSCTX_INPROC_SERVER, IID_IOfflineFilesCache, (void**) &m_pOfflineFilesCache); 
		if ( FAILED(hr) )
		{
			printf("Failed To Create offline Cache Object: %d \r\n",hr);
			m_pOfflineFilesCache = NULL;
		}
	}

	if ( m_pOfflineFilesCache != NULL )
	{
		printf("Created Files Cache Instance\r\n");
		return (TRUE);
	}

	printf("Creating Offline Files Progress Handler Instance\r\n");
	
	CoUninitialize();
	return (FALSE);
}

OFFLINEFILESCLIENT_INIT_STATE COfflineFilesClient::GetInitialiseState(void)
{
	if (!m_bIsOfflineFilesEnabled)
	{
		if (m_bStartupReebotRequired)
		{
			return OFFLINEFILESCLIENT_INIT_STATE_RESTART_PENDING;
		}
		return OFFLINEFILESCLIENT_INIT_STATE_NOT_ENABLED;
	}
	else
	{
		if (m_bIsOfflineFilesActive)
		{
			return OFFLINEFILESCLIENT_INIT_STATE_STARTED;
		}
		return OFFLINEFILESCLIENT_INIT_STATE_STARTED;
	}
}

BOOL COfflineFilesClient::InitCache(LPCWSTR *ppszCachePath)
{
	if (m_pOfflineFilesCache == NULL)
	{
		// The Cache Has Not been initialised
		return (FALSE);
	}

	//Save the path as a member variable
	size_t PathSize =  (wcslen (*ppszCachePath) + 1) * sizeof(wchar_t);
	m_pszCachePath = (LPCWSTR) malloc(PathSize);
	if (m_pszCachePath == NULL) 
	{
		printf("Failed To allocate Memory For Path Member");
		return (FALSE);
	}
	
	memcpy((void *)m_pszCachePath,*ppszCachePath,PathSize);
	
	//Print out data about Offline Files
	PrintLocation();
	PrintEncryptionStatus();
	PrintDiskSpaceInfo();
	PrintSettings();

	//Get The status of the Cache
	if ( !CacheStatus())
	{
		printf("The specified folder is not cachable");
		return (FALSE);
	}

	//Get The Current State Of The Cache
	m_pOfflineFilesCache->FindItem(m_pszCachePath,0,(IOfflineFilesItem**)&m_pOfflineFilesItem);
	if(m_pOfflineFilesItem != NULL)
	{
		m_pOfflineFilesItem->QueryInterface(IID_IOfflineFilesConnectionInfo,(void**)&m_pOfflineFilesConnecionInfo);
		if (m_pOfflineFilesConnecionInfo == NULL)
		{
			printf("Cannot Determine The State of path. ");
			return (FALSE);
		}
		m_pOfflineFilesConnecionInfo->GetConnectState(&m_CurentConnectState,&m_OfflineReason);
	}
	else
	{
		printf("Failed to get Interface To Sync Item");
		return(FALSE);
	}
	return (TRUE);
}

void COfflineFilesClient::PrintLocation(void)
{
	if (m_pOfflineFilesCache == NULL)
	{
		// The Cache Has Not been initialised
		return;
	}

	printf("Getting Cache Location\n\r");
	LPWSTR szCacheLocation;
	if ( m_pOfflineFilesCache->GetLocation(&szCacheLocation) == S_OK)
	{
		wprintf(L"Location Is: %s\r\n",szCacheLocation);
	}
}

void COfflineFilesClient::PrintEncryptionStatus(void)
{
	if (m_pOfflineFilesCache == NULL)
	{
		// The Cache Has Not been initialised
		return;
	}

	//Log The Encryption Status
	printf("Getting Cache Encryption Status\n\r");
	BOOL bEncryptionStatus = FALSE;
	BOOL bPartialEncryption = FALSE;
	if ( m_pOfflineFilesCache ->GetEncryptionStatus(&bEncryptionStatus,&bPartialEncryption) == S_OK)
	{
		printf("Offline Files Encryption status: %s\r\n",(bEncryptionStatus ? "Encrypted" : "UnEncrypted"));
		printf("Offline Files is Partialy Encrypted: %s\r\n",(bPartialEncryption ? "True" : "False"));
	}
}

void COfflineFilesClient::PrintDiskSpaceInfo(void)
{
	if (m_pOfflineFilesCache == NULL)
	{
		// The Cache Has Not been initialised
		return;
	}

	printf("Getting Cache Disckpace Information\n\r");
	ULONGLONG cbVolumeTotal =0;
	ULONGLONG cbLimit =0;
	ULONGLONG cbUsed =0;
	ULONGLONG cbUnpinnedLimit=0;
	ULONGLONG cbUnpinnedUsed=0;
		
	if ( m_pOfflineFilesCache->GetDiskSpaceInformation(&cbVolumeTotal, &cbLimit, &cbUsed, &cbUnpinnedLimit, &cbUnpinnedUsed)  == S_OK)
	{
		printf("Volume Size is: %ull bytes\r\n",cbVolumeTotal);
		printf("Maximum Cache Size is: %ull bytes\r\n",cbLimit);
		printf("Current Cache Size is: %ull bytes\r\n",cbUsed);
		printf("Unpinned Cache Limit is: %ull bytes \r\n",cbUnpinnedLimit);
		printf("Current Unpinned Cache Size is: %ull bytes\r\n",cbUnpinnedUsed);
	}
}

BOOL COfflineFilesClient::CacheStatus(void)
{
	if (m_pOfflineFilesCache == NULL)
	{
		// The Cache Has Not been initialised
		return(FALSE);
	}

	printf("Getting Cache Path Status\n\r");
	
	if ( m_pOfflineFilesCache->IsPathCacheable(m_pszCachePath, &m_bCacheable,&m_ShareCachingMode) == S_OK)
	{
		printf("Path Is Cachable: %s\r\n",(m_bCacheable ? "True" : "False"));
		printf("Path Caching Mode is: %d\r\n",m_ShareCachingMode);
	}
	return m_bCacheable;
}

void COfflineFilesClient::PrintSettings(void)
{
	if (m_pOfflineFilesCache == NULL)
	{
		// The Cache Has Not been initialised
		return;
	}

	//Get Setting Information
	printf("Getting Settings For Offline Files\n\r");
	IEnumOfflineFilesSettings *pEnum = NULL;
	if (m_pOfflineFilesCache->EnumSettingObjects(&pEnum) == S_OK)
	{
		if ( pEnum !=NULL)
		{
			IOfflineFilesSetting *rgelt = NULL;
			ULONG celtFetched = 0;
			VARIANT varValue;
			BOOL bSetByPolicy;
			VariantInit(&varValue);

			while(pEnum->Next(1,&rgelt,&celtFetched)!=S_FALSE)
			{
				LPWSTR pszName = NULL;
				OFFLINEFILES_SETTING_VALUE_TYPE Type = OFFLINEFILES_SETTING_VALUE_UI4;
				rgelt->GetValueType(&Type);
				rgelt->GetName(&pszName);
				rgelt->GetValue(&varValue,&bSetByPolicy);
				if(Type == OFFLINEFILES_SETTING_VALUE_UI4)
				{
					wprintf(L"Setting Name: %s Value:%d Set By Policy:%s\n\r",pszName,varValue.intVal,(bSetByPolicy ? L"True" : L"False"));
				}
				else
				{
					wprintf(L"Setting Name: %s Value:NA Set By Policy:%s\n\r",pszName,(bSetByPolicy?  L"True" : L"False"));
				}
			}
		}
	}
	else
	{
		printf("Failed getting settings\r\n");
	}
	pEnum->Release();
	pEnum = NULL;
}

BOOL COfflineFilesClient::RegisterSyncEvents() //Might not need to look at events as Sync provides method to handle a sync conflict
{
	if (m_pOfflineFilesCache == NULL)
	{
		// The Cache Has Not been initialised
		return (FALSE);
	}

	IConnectionPointContainer * pIConnectionPointContainerTemp = NULL;
	m_pOfflineFilesCache -> QueryInterface (IID_IConnectionPointContainer,  (void**)&pIConnectionPointContainerTemp);
	if (pIConnectionPointContainerTemp)
	{
		printf("Interface supports connection points\r\n");
		HRESULT hr = pIConnectionPointContainerTemp -> FindConnectionPoint(IID_IOfflineFilesEvents, &m_pIConnectionPoint);
		if(FAILED(hr))
		{
			printf("failedTo Find connection point\r\n");
			m_pIConnectionPoint = NULL;
		}
		else
		{
			printf("found connection point\r\n");
		}
		pIConnectionPointContainerTemp -> Release();
		pIConnectionPointContainerTemp = NULL;
	}

	if ( m_pIConnectionPoint == NULL )
	{
		return (FALSE);
	}

	m_pOfflineFilesEvents = new COfflineFilesEvents;
		
	if (m_pOfflineFilesEvents == NULL)
	{
		printf("failed to create connection point\r\n");
	}

	BOOL bEventsRegistered = FALSE;
	if ( m_pIConnectionPoint != NULL && m_pOfflineFilesEvents != NULL)
	{
		HRESULT hr = m_pOfflineFilesEvents->QueryInterface (IID_IUnknown,(void **)&m_pSinkUnk);
		if ( hr == S_OK)
		{
			hr = m_pIConnectionPoint->Advise(m_pSinkUnk,&m_dwAdvise);
			if (hr == S_OK)
			{
				bEventsRegistered = TRUE;
			}
		}
		else
		{
			printf("failed Get Unknown Interface ID\r\n");
		}
		if (!bEventsRegistered && m_pSinkUnk!=NULL)
		{
			m_pSinkUnk->Release();
			m_pSinkUnk = NULL;
		}

		if (!bEventsRegistered && m_pIConnectionPoint!=NULL)
		{
			m_pIConnectionPoint->Release();
			m_pIConnectionPoint = NULL;
		}
		if (!bEventsRegistered && m_pOfflineFilesEvents!=NULL)
		{
			m_pOfflineFilesEvents->Release();
			m_pOfflineFilesEvents=NULL;
		}

		if(bEventsRegistered)
		{
			return (TRUE);
		}
	}
	return (FALSE);
}

void COfflineFilesClient::Cleanup(void)
{
	if ( m_pszCachePath != NULL)
	{
		free((void *)m_pszCachePath);
		m_pszCachePath = NULL;
	}
	
	if (m_pIConnectionPoint != NULL)
	{
		m_pIConnectionPoint->Unadvise(m_dwAdvise);
		m_pIConnectionPoint->Release();
		m_pIConnectionPoint = NULL;
	}

	if (m_pOfflineFilesEvents  != NULL)
	{
		m_pOfflineFilesEvents->Release();
		m_pOfflineFilesEvents=NULL;
	}

	if (m_pSinkUnk!=NULL)
	{
		m_pSinkUnk->Release();
		m_pSinkUnk=NULL;
	}

	if (m_pOfflineFilesCache != NULL)
	{
		m_pOfflineFilesCache->Release();
		m_pOfflineFilesCache = NULL;
	}
}

BOOL COfflineFilesClient::Synchronise(void)
{
	printf("sync called\n\r");
	if (m_pOfflineFilesCache == NULL)
	{
		// The Cache Has Not been initialised
		return (FALSE);
	}
	CSyncConflictHandler *pOfflineFilesConflictHandler = new CSyncConflictHandler;
	CSyncProgressHandler *pOfflineFilesProgress = new CSyncProgressHandler;
	pOfflineFilesProgress->m_pCurentConnectState = &m_CurentConnectState;
	pOfflineFilesProgress->m_pbOfflineOnlineTranistionReq = &m_bOfflineOnlineTranistionRequired;

	//Get the current Logged in user - used for renaming files on conflict
	DWORD  bufCharCount = UNLEN;
	if(0 == GetUserName(pOfflineFilesConflictHandler->m_szUsername, &bufCharCount))
	{
		//If we did not get the user name then set it to user
		_tcscpy_s(pOfflineFilesConflictHandler->m_szUsername,TEXT("USER"));
	}
	
	HRESULT hrResult = m_pOfflineFilesCache->Synchronize( NULL,		//No Window Handle
								&m_pszCachePath,					//The path of our folder
								1,									//1 folder in the list
								FALSE,								//Run Synchronously
								SYNC_OPTIONS,						//Sync Options
								pOfflineFilesConflictHandler,		//Conflict Handler
								pOfflineFilesProgress,				//Progress Handler
								NULL);								//Sync ID
								
								
	printf("sync returned(%d): %X\n\r",S_OK,hrResult);

	if (m_bOfflineOnlineTranistionRequired)
	{
		m_bOfflineOnlineTranistionRequired = FALSE;
		printf("Need to run an Offline to online tranisition");

		//The synchronise function indicates we need to tranistion to online
		BOOL bTranistioncompleted = FALSE;
		if (m_CurentConnectState == OFFLINEFILES_CONNECT_STATE_OFFLINE && 
			(m_OfflineReason==OFFLINEFILES_OFFLINE_REASON_CONNECTION_FORCED || m_OfflineReason==OFFLINEFILES_OFFLINE_REASON_CONNECTION_SLOW )) //Check that we are offline and will be able to tranisition online
		{
			if(S_OK != m_pOfflineFilesConnecionInfo->TransitionOnline(NULL,0x0))
			{
				printf("Failed To Go Online\n\r");
			}
			
			//update the connection state
			m_pOfflineFilesConnecionInfo->GetConnectState(&m_CurentConnectState,&m_OfflineReason);
			
			if (m_CurentConnectState==OFFLINEFILES_CONNECT_STATE_ONLINE) //online so wait and then return to offline mode
			{
				Sleep(60000); //Wait 60 seconds to fetch all changes
				
			}
			BOOL bOpenFilesPreventedTransition = FALSE;
			if(S_OK != m_pOfflineFilesConnecionInfo->TransitionOffline(NULL,0x0,FALSE,&bOpenFilesPreventedTransition))
			{
				printf("Failed To Go Offline\n\r");
				if (bOpenFilesPreventedTransition)
				{
					printf("Failed because a file is open\n\r");
					Sleep(60000); //Wait another 60 seconds to fetch all changes
					if(S_OK != m_pOfflineFilesConnecionInfo->TransitionOffline(NULL,0x0,TRUE,&bOpenFilesPreventedTransition)) //This time forcibly close any connections
					{
						printf("Failed To Go Offline\n\r");
					}
				}
			}
			//Refresh the current connection state
			m_pOfflineFilesConnecionInfo->GetConnectState(&m_CurentConnectState,&m_OfflineReason);
			if(bTranistioncompleted && m_CurentConnectState!=OFFLINEFILES_CONNECT_STATE_OFFLINE)
			{
				//somthing went wrong we completed a tranistion but are not offline
				printf("Unknown Transition Error: Not Offline\n\r");
			}
		}		
	}
	return true;
}
	
