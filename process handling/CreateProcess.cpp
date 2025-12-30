#include <iostream>
#include <Windows.h>
#include <processthreadsapi.h>

using namespace std;

int main(void) {

/*
BOOL CreateProcessW(
  [in, optional]      LPCWSTR               lpApplicationName,
  [in, out, optional] LPWSTR                lpCommandLine,
  [in, optional]      LPSECURITY_ATTRIBUTES lpProcessAttributes,
  [in, optional]      LPSECURITY_ATTRIBUTES lpThreadAttributes,
  [in]                BOOL                  bInheritHandles,
  [in]                DWORD                 dwCreationFlags,
  [in, optional]      LPVOID                lpEnvironment,
  [in, optional]      LPCWSTR               lpCurrentDirectory,
  [in]                LPSTARTUPINFOW        lpStartupInfo,
  [out]               LPPROCESS_INFORMATION lpProcessInformation
);
*/

	LPCWSTR path           = L"C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";
	STARTUPINFOW si        = { 0 };      // struct de inicialização
	PROCESS_INFORMATION pi = { 0 };      // struct pra informações do processo
	si.cb				   = sizeof(si); // tamanho da struct  

	BOOL exec = CreateProcessW(
		path,
		NULL,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si, // aponta pro endereço da struct de inicialização
		&pi  // aponta pro endereço da struct pra informações do processo
	);

	if (exec) {
		wcout << L"[+] process sucessfully created!" << endl;
		wcout << L"[*] PID: " << pi.dwProcessId << endl;
	}
	else {
		wcout << L"[!] failed to create process";
	}

}
