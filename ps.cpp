#include <stdio.h>
#include <Windows.h>  
#include <TlHelp32.h>  

int ps_main(int argc, char* argv[])  
{  
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  
	if (hProcessSnap == FALSE )  
	{  
		printf("CreateToolhelp32Snapshot error");  
		return -1;  
	}  
	PROCESSENTRY32 pe32;  
	pe32.dwSize = sizeof(PROCESSENTRY32);  

	BOOL bRet = Process32First(hProcessSnap, &pe32);  
	printf("PID\t\tPPID\t\tNAME\n");
	while (bRet) {  
		printf("%d\t\t%d\t\t%s\t\t\n", pe32.th32ProcessID, pe32.th32ParentProcessID, pe32.szExeFile);  
		bRet = Process32Next(hProcessSnap, &pe32); 
	}
	::CloseHandle(hProcessSnap); 
	return 0;  
}

BOOL KillProcess(DWORD ProcessId)
{
	HANDLE hProcess=OpenProcess(PROCESS_TERMINATE,FALSE,ProcessId);
	if(hProcess==NULL)
		return FALSE;
	if(!TerminateProcess(hProcess,0))
		return FALSE;
	return TRUE;
}

int kill_main(int argc, char* argv[])
{
	if (argc != 2) {
		printf("invalid arguments\n");
		return -1;
	}
	
	KillProcess(atoi(argv[1]));
}

