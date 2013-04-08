#pragma once

#include "resource.h"

BOOL InitSyncClient();
DWORD WINAPI SyncThreadFunc( LPVOID lpParam ) ;
DWORD WINAPI ConsoleListenerThreadFunc( LPVOID lpParam);
BOOL CtrlHandler( DWORD fdwCtrlType ) ;