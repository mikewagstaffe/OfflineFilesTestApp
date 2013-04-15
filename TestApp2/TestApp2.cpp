// TestApp2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestApp2.h"
#include "OfflineFilesClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CACHEPATH L"\\\\pdmwe.ishida.com\\AutoCAD\\SyncTest"//L"\\\\pdmwe\\AutoCAD\\SyncTest""\\\\ASSET-1256\\SyncShare\\New Text Document.txt"//
#define SYNC_RATE 30000
// The one and only application object

CWinApp theApp;

using namespace std;

COfflineFilesClient	OfflineFilesClient;
HANDLE  hSyncThread = NULL; 
HANDLE	ghTerminateEvent = NULL; 

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	if (! InitSyncClient() )
	{
		printf("Failed Initialising Sync Client\n\r");
		OfflineFilesClient.Cleanup(); //This is called in the destructor but do it anyway
		getchar();
		return (1);
	}
	
	// The Sync client is initiated now create a thread to run it in
	if( ! SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE ) )
	{
		printf("Failed Adding Control Handler\n\r");
		OfflineFilesClient.Cleanup(); //This is called in the destructor but do it anyway
		getchar();
		return (1);
	}

	hSyncThread = CreateThread(NULL,0,SyncThreadFunc,NULL,0,NULL); //Create the synchronisation thread
	if (hSyncThread == NULL)
	{
		printf("Failed Creating The Sync thread\n\r");
		SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, FALSE ); //Clear the control handler
		OfflineFilesClient.Cleanup(); //This is called in the destructor but do it anyway
		getchar();
		return (1);
	}

	printf("Waiting For Sync Client To Shut Down\n\r");
	WaitForSingleObject(hSyncThread,INFINITE); // The program will wait here forever until the sync thread shuts down
	printf("Sync Client is shutting Down\n\r");

	hSyncThread = NULL;
	SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, FALSE ); //Clear the control handler
	OfflineFilesClient.Cleanup(); //This is called in the destructor but do it anyway
	
	getchar();
	return (0);
}


BOOL InitSyncClient()
{
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
			//OfflineFilesClient.RegisterSyncEvents();
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

	return bInitComplete;
}
DWORD WINAPI SyncThreadFunc( LPVOID lpParam ) 
{

	ghTerminateEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("TerminateEvent") ); 
	if (ghTerminateEvent == NULL) 
    { 
        printf("CreateEvent For Terminate Event failed (%d)\n", GetLastError());
        return 0;
    }

	while (TRUE)
	{
		DWORD dwResult = WaitForSingleObject(ghTerminateEvent,SYNC_RATE);
		if (dwResult == WAIT_OBJECT_0)
		{
			//is a terminate event
			printf("Thread Terminate Received\n\r");
			break;
		}
		OfflineFilesClient.Synchronise();

	}

	CloseHandle(ghTerminateEvent);
	CoUninitialize();
	return 0;
}

//Used to signal the sync thread to shutdown
BOOL CtrlHandler( DWORD fdwCtrlType ) 
{ 
	if ( fdwCtrlType == CTRL_C_EVENT || fdwCtrlType == CTRL_BREAK_EVENT)
	{
		if ( !SetEvent(ghTerminateEvent) ) 
		{
			//Failed to set the event so need to forcibly terminate the thread
			TerminateThread(hSyncThread,0);
			hSyncThread=NULL;
		}
		return true;
	}
	else
	{
		return false;
	}
} 


