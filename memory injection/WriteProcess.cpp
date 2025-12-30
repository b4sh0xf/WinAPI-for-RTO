#include <iostream>
#include <Windows.h>
#include <processthreadsapi.h>
#include <memoryapi.h>

/*

DWORD GetCurrentProcessId();

HANDLE OpenProcess(
  [in] DWORD dwDesiredAccess, => access rights (https://learn.microsoft.com/pt-br/windows/win32/procthread/process-security-and-access-rights)
  [in] BOOL  bInheritHandle,  => herança
  [in] DWORD dwProcessId      => PID
);

BOOL WriteProcessMemory(
  [in]  HANDLE  hProcess,               => identificador pra região de memoria do processo
  [in]  LPVOID  lpBaseAddress,			=> ponteiro pro base address do buffer a ser escrito
  [in]  LPCVOID lpBuffer,				=> ponteiro pro buffer
  [in]  SIZE_T  nSize,                  => tamanho do buffer, quantos bytes vc quer escreever no processo
  [out] SIZE_T  *lpNumberOfBytesWritten => ponteiro pra variável que recebe o numero de bytes escritos
);

BOOL ReadProcessMemory(
  [in]  HANDLE  hProcess,			 => identificador pra região de memoria do processo
  [in]  LPCVOID lpBaseAddress,		 => ponteiro pro base address do processo
  [out] LPVOID  lpBuffer,			 => buffer que recebe os bytes da região de memoria do processo
  [in]  SIZE_T  nSize,				 => tamanho do buffer
  [out] SIZE_T  *lpNumberOfBytesRead => ponteiro pra variável que recebe o numero de bytes lidos
  * se a função falhar, chame GetLastError() pra pegar um stack trace detalhado
);

*/

using namespace std;

int main(void) {

	int value = 1337;
	DWORD pid = GetCurrentProcessId(); // estamos fazendo isso para exemplo. em um cenario real, enumerariamos os processos e pegariamos o seu PID (gps | Select-String -Pattern <PID>)

	cout << "[+] original value: " << value << endl;
	cout << "[+] current PID: " << pid << endl;

	HANDLE hProcess = OpenProcess(
		(DWORD)PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,
		false,
		pid
	);

	if (!hProcess) {
		cerr << "[!] error to find process with PID " << pid << ". error " << GetLastError() << endl;
		cerr << "[!] check https://learn.microsoft.com/pt-br/windows/win32/debug/system-error-codes--0-499- for more information." << endl;
		return 1;
	}

	int lpReadBuffer		   = 0;
	SIZE_T lpNumberOfBytesRead = 0;

	if (ReadProcessMemory(hProcess, &value, &lpReadBuffer, sizeof(lpReadBuffer), &lpNumberOfBytesRead)) {
		cout << "[*] ReadProcessMemory: " << lpReadBuffer << ". " << sizeof(lpReadBuffer) << " bytes read." << endl;
	}
	else {
		cerr << "[!] error to read process memory. error " << GetLastError() << endl;
		cerr << "[!] check https://learn.microsoft.com/pt-br/windows/win32/debug/system-error-codes--0-499- for more information." << endl;
	}

	int newValue			          = 777;
	SIZE_T lpNumberOfBytesWrite = 0;

	if (WriteProcessMemory(hProcess, &value, &newValue, sizeof(newValue), &lpNumberOfBytesWrite)) {
		cout << "[*] WriteProcessMemory: wrote " << lpNumberOfBytesWrite << " bytes." << endl;
		cout << "[-] tampered value: " << value << endl;
	}
	else {
		cerr << "[!] error to write into process memory. error " << GetLastError() << endl;
		cerr << "[!] check https://learn.microsoft.com/pt-br/windows/win32/debug/system-error-codes--0-499- for more information." << endl;
	}

}
