/*
 * PROJECT:     ReactOS Services
 * LICENSE:     GPL - See COPYING in the top level directory
 * FILE:        base/system/sc/query.c
 * PURPOSE:     queries service info
 * COPYRIGHT:   Copyright 2005 - 2006 Ged Murphy <gedmurphy@gmail.com>
 *
 */
/*
 * TODO:
 * Allow calling of 2 options e.g.:
 *    type= driver state= inactive
 */

#include <stdarg.h>
#include <windef.h>
#include <winbase.h>
#include <winsvc.h>
#include <tchar.h>

LPTSTR QueryOpts[] = {
    _T("type="),
    _T("state="),
    _T("bufsize="),
    _T("ri="),
    _T("group="),
};

VOID ReportLastError(VOID)
{
	LPVOID lpMsgBuf;
	DWORD RetVal;

	DWORD ErrorCode = GetLastError();
	if (ErrorCode != ERROR_SUCCESS)
	{
		RetVal = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				ErrorCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), /* Default language */
				(LPTSTR) &lpMsgBuf,
				0,
				NULL );

		if (RetVal != 0)
		{
			_tprintf(_T("%s"), (LPTSTR)lpMsgBuf);
			LocalFree(lpMsgBuf);
		}
	}
}

VOID PrintService(LPCTSTR lpServiceName,
		LPSERVICE_STATUS_PROCESS pStatus,
		BOOL bExtended)
{
	_tprintf(_T("SERVICE_NAME: %s\n"), lpServiceName);

	_tprintf(_T("\tTYPE               : %x  "),
			(unsigned int)pStatus->dwServiceType);
	switch (pStatus->dwServiceType)
	{
		case SERVICE_KERNEL_DRIVER:
			_tprintf(_T("KERNEL_DRIVER\n"));
			break;

		case SERVICE_FILE_SYSTEM_DRIVER:
			_tprintf(_T("FILE_SYSTEM_DRIVER\n"));
			break;

		case SERVICE_WIN32_OWN_PROCESS:
			_tprintf(_T("WIN32_OWN_PROCESS\n"));
			break;

		case SERVICE_WIN32_SHARE_PROCESS:
			_tprintf(_T("WIN32_SHARE_PROCESS\n"));
			break;

		case SERVICE_WIN32_OWN_PROCESS + SERVICE_INTERACTIVE_PROCESS:
			_tprintf(_T("WIN32_OWN_PROCESS (interactive)\n"));
			break;

		case SERVICE_WIN32_SHARE_PROCESS + SERVICE_INTERACTIVE_PROCESS:
			_tprintf(_T("WIN32_SHARE_PROCESS (interactive)\n"));
			break;

		default : _tprintf(_T("\n")); break;
	}

	_tprintf(_T("\tSTATE              : %x  "),
			(unsigned int)pStatus->dwCurrentState);

	switch (pStatus->dwCurrentState)
	{
		case 1 : _tprintf(_T("STOPPED\n")); break;
		case 2 : _tprintf(_T("START_PENDING\n")); break;
		case 3 : _tprintf(_T("STOP_PENDING\n")); break;
		case 4 : _tprintf(_T("RUNNING\n")); break;
		case 5 : _tprintf(_T("CONTINUE_PENDING\n")); break;
		case 6 : _tprintf(_T("PAUSE_PENDING\n")); break;
		case 7 : _tprintf(_T("PAUSED\n")); break;
		default : _tprintf(_T("\n")); break;
	}

	_tprintf(_T("\t\t\t\t("));

	if (pStatus->dwControlsAccepted & SERVICE_ACCEPT_STOP)
		_tprintf(_T("STOPPABLE,"));
	else
		_tprintf(_T("NOT_STOPPABLE,"));

	if (pStatus->dwControlsAccepted & SERVICE_ACCEPT_PAUSE_CONTINUE)
		_tprintf(_T("PAUSABLE,"));
	else
		_tprintf(_T("NOT_PAUSABLE,"));

	if (pStatus->dwControlsAccepted & SERVICE_ACCEPT_SHUTDOWN)
		_tprintf(_T("ACCEPTS_SHUTDOWN"));
	else
		_tprintf(_T("IGNORES_SHUTDOWN"));

	_tprintf(_T(")\n"));

	_tprintf(_T("\tWIN32_EXIT_CODE    : %u  (0x%x)\n"),
			(unsigned int)pStatus->dwWin32ExitCode,
			(unsigned int)pStatus->dwWin32ExitCode);
	_tprintf(_T("\tSERVICE_EXIT_CODE  : %u  (0x%x)\n"),
			(unsigned int)pStatus->dwServiceSpecificExitCode,
			(unsigned int)pStatus->dwServiceSpecificExitCode);
	_tprintf(_T("\tCHECKPOINT         : 0x%x\n"),
			(unsigned int)pStatus->dwCheckPoint);
	_tprintf(_T("\tWAIT_HINT          : 0x%x\n"),
			(unsigned int)pStatus->dwWaitHint);

	if (bExtended)
	{
		_tprintf(_T("\tPID                : %lu\n"),
				pStatus->dwProcessId);
		_tprintf(_T("\tFLAGS              : %lu\n"),
				pStatus->dwServiceFlags);
	}

	_tprintf(_T("\n"));
}

LPSERVICE_STATUS_PROCESS
QueryService(LPCTSTR ServiceName)
{
    SC_HANDLE hSCManager = NULL;
    LPSERVICE_STATUS_PROCESS pServiceInfo = NULL;
    SC_HANDLE hSc = NULL;
    DWORD BufSiz = 0;
    DWORD BytesNeeded = 0;
    DWORD Ret;

    hSCManager = OpenSCManager(NULL,
                               NULL,
                               SC_MANAGER_CONNECT);
    if (hSCManager == NULL)
    {
        ReportLastError();
        return NULL;
    }

    hSc = OpenService(hSCManager,
                      ServiceName,
                      SERVICE_QUERY_STATUS);
    if (hSc == NULL)
        goto fail;

    Ret = QueryServiceStatusEx(hSc,
                               SC_STATUS_PROCESS_INFO,
                               NULL,
                               BufSiz,
                               &BytesNeeded);
    if ((Ret != 0) || (GetLastError() != ERROR_INSUFFICIENT_BUFFER))
        goto fail;

    pServiceInfo = (LPSERVICE_STATUS_PROCESS)HeapAlloc(GetProcessHeap(),
                                                       0,
                                                       BytesNeeded);
    if (pServiceInfo == NULL)
        goto fail;

    if (!QueryServiceStatusEx(hSc,
                              SC_STATUS_PROCESS_INFO,
                              (LPBYTE)pServiceInfo,
                              BytesNeeded,
                              &BytesNeeded))
    {
        goto fail;
    }

    CloseServiceHandle(hSc);
    CloseServiceHandle(hSCManager);
    return pServiceInfo;

fail:
    ReportLastError();
    if (pServiceInfo) HeapFree(GetProcessHeap(), 0, pServiceInfo);
    if (hSc) CloseServiceHandle(hSc);
    if (hSCManager) CloseServiceHandle(hSCManager);
    return NULL;
}


static BOOL
EnumServices(ENUM_SERVICE_STATUS_PROCESS **pServiceStatus,
             DWORD ServiceType,
             DWORD ServiceState)
{
    SC_HANDLE hSCManager;
    DWORD BufSize = 0;
    DWORD BytesNeeded = 0;
    DWORD ResumeHandle = 0;
    DWORD NumServices = 0;
    DWORD Ret;

    hSCManager = OpenSCManager(NULL,
                               NULL,
                               SC_MANAGER_ENUMERATE_SERVICE);
    if (hSCManager == NULL)
    {
        ReportLastError();
        return FALSE;
    }

    Ret = EnumServicesStatusEx(hSCManager,
                               SC_ENUM_PROCESS_INFO,
                               ServiceType,
                               ServiceState,
                               (LPBYTE)*pServiceStatus,
                               BufSize,
                               &BytesNeeded,
                               &NumServices,
                               &ResumeHandle,
                               0);

    if ((Ret == 0) && (GetLastError() == ERROR_MORE_DATA))
    {
        *pServiceStatus = (ENUM_SERVICE_STATUS_PROCESS *)
                          HeapAlloc(GetProcessHeap(),
                                    0,
                                    BytesNeeded);
        if (*pServiceStatus != NULL)
        {
            if (EnumServicesStatusEx(hSCManager,
                                     SC_ENUM_PROCESS_INFO,
                                     ServiceType,
                                     ServiceState,
                                     (LPBYTE)*pServiceStatus,
                                     BytesNeeded,
                                     &BytesNeeded,
                                     &NumServices,
                                     &ResumeHandle,
                                     0))
            {
                CloseServiceHandle(hSCManager);
                return NumServices;
            }
        }
    }

    ReportLastError();
    if (*pServiceStatus)
        HeapFree(GetProcessHeap(), 0, *pServiceStatus);

    CloseServiceHandle(hSCManager);

    return NumServices;
}


BOOL
Query(LPCTSTR *ServiceArgs,
      DWORD ArgCount,
      BOOL bExtended)
{
    LPENUM_SERVICE_STATUS_PROCESS pServiceStatus = NULL;
    DWORD NumServices = 0;
    //DWORD ServiceType;
    //DWORD ServiceState;
    BOOL bServiceName = TRUE;
    DWORD OptSize, i;

    LPCTSTR *TmpArgs;
    INT TmpCnt;

#ifdef SCDBG
    TmpArgs = ServiceArgs;
    TmpCnt = ArgCount;
    _tprintf(_T("Arguments:\n"));
    while (TmpCnt)
    {
        _tprintf(_T("  %s\n"), *TmpArgs);
        TmpArgs++;
        TmpCnt--;
    }
    _tprintf(_T("\n"));
#endif /* SCDBG */

    /* display all running services and drivers */
    if (ArgCount == 0)
    {
        NumServices = EnumServices(&pServiceStatus,
                                   SERVICE_WIN32,
                                   SERVICE_ACTIVE);

        if (NumServices != 0)
        {
            for (i=0; i < NumServices; i++)
            {
                PrintService(pServiceStatus[i].lpServiceName,
                             &pServiceStatus[i].ServiceStatusProcess,
                             bExtended);
            }

            _tprintf(_T("number : %lu\n"), NumServices);

            if (pServiceStatus)
                HeapFree(GetProcessHeap(), 0, pServiceStatus);

            return TRUE;
        }

        return FALSE;
    }

    TmpArgs = ServiceArgs;
    TmpCnt = ArgCount;
    OptSize = sizeof(QueryOpts) / sizeof(QueryOpts[0]);
    while (TmpCnt--)
    {
        for (i=0; i < OptSize; i++)
        {
            if (!lstrcmpi(*TmpArgs, QueryOpts[i]))
            {
                bServiceName = FALSE;
            }
        }
        TmpArgs++;
    }


    /* FIXME: parse options */


    /* print only the service requested */
    if (bServiceName)
    {
        LPSERVICE_STATUS_PROCESS pStatus;
        LPCTSTR ServiceName = *ServiceArgs;

        pStatus = QueryService(ServiceName);
        if (pStatus)
        {
            PrintService(ServiceName,
                         pStatus,
                         bExtended);
        }
    }

    return TRUE;
}

int scquery_main(int argc, char* argv[])
{
	return !Query(&argv[1], argc - 1, TRUE);
}
