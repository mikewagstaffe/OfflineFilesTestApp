// TestApp2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestApp2.h"
#include "OfflineFilesClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CACHEPATH L"\\\\pdmwe\\AutoCAD\\SyncTest"

// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	COfflineFilesClient	OfflineFilesClient;
	BOOL bInitComplete = FALSE;

	if ( !OfflineFilesClient.Init())
	{
		printf("Falied To Initialise Offline Client\r\n");
	}

	if (OfflineFilesClient.GetInitialiseState() == OFFLINEFILESCLIENT_INIT_STATE_RESTART_PENDING)
	{
		printf("Your PC Must be restarted to start Offline files Service\r\n");
	}
	else if (OfflineFilesClient.GetInitialiseState() == OFFLINEFILESCLIENT_INIT_STATE_STARTED)
	{
		LPCWSTR pszPath = CACHEPATH;
		if(!OfflineFilesClient.InitCache(&pszPath))
		{
			printf("Falied To Initialise Offline Client Cache\r\n");
		}
		else
		{
			bInitComplete = TRUE;
		}
	}
	else
	{
		printf("Offline Files Services are not started\r\n");
	}

	if (bInitComplete)
	{
		printf("init is complete\r\n");
	}
	else
	{
		printf("init failed\r\n");
	}

	//Initialisation is complete
	//Create A thread to sync files every miute
	//Create A thread to listen for keypresses at the command prompt
	HANDLE syncThread = CreateThread(NULL,0,SyncThreadFunc,&OfflineFilesClient,0
	getchar();

	OfflineFilesClient.Cleanup(); //This is called in the destructor but do it anyway
	return 0;
}

DWORD WINAPI SyncThreadFunc( LPVOID lpParam ) 
{
}

DWORD WINAPI ConsoleListenerThreadFunc( LPVOID lpParam)
{
}


