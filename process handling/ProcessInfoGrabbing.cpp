#include <iostream>
#include <Windows.h>
#include <winternl.h>
#include <processthreadsapi.h>

using namespace std;

/*
#define NtCurrentLogonId() (NtCurrentPeb()->LogonId)

 * The NtQueryInformationProcess routine retrieves information about the specified process.
 *
 * \param ProcessHandle A handle to the process.
 * \param ProcessInformationClass The type of process information to be retrieved.
 * \param ProcessInformation -> (*) A pointer to a buffer that receives the process information.
 * \param ProcessInformationLength The size of the buffer pointed to by the ProcessInformation parameter.
 * \param ReturnLength An optional pointer to a variable that receives the size of the data returned.
 * \return NTSTATUS Successful or errant status.

NTSYSCALLAPI
NTSTATUS
NTAPI
NtQueryInformationProcess(
    _In_ HANDLE ProcessHandle,
    _In_ PROCESSINFOCLASS ProcessInformationClass,
    _Out_writes_bytes_(ProcessInformationLength) PVOID ProcessInformation,
    _In_ ULONG ProcessInformationLength,
    _Out_opt_ PULONG ReturnLength
);

(*) typedef struct _PROCESS_BASIC_INFORMATION {
    NTSTATUS ExitStatus;
    PPEB PebBaseAddress;
    ULONG_PTR AffinityMask;
    KPRIORITY BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;

*/

// struct custom pra fazer o retrieve de informações do processo e não conflitar com a _PROCESS_BASIC_INFORMATION
typedef struct _MY_PROCESS_INFORMATION {
    PVOID Reserved1;
    PPEB PebBaseAddress;
    PVOID Reserved2[2];
    ULONG_PTR UniqueProcessId;
    ULONG_PTR InheritedFromUniqueProcessId;
} MY_PROCESS_INFORMATION;

// ponteiro de função pra NtQueryInformationProcess. ela é uma função da NTAPI e retorna NTSTATUS
typedef NTSTATUS(NTAPI* NtQueryInformationProcess_t)(
    HANDLE,
    PROCESSINFOCLASS,
    PVOID,
    ULONG,
    PULONG
    );

int main(void) {

    DWORD pid       = GetCurrentProcessId();
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, pid);

    if (!hProcess) {
        cerr << "[!] error to find process with PID " << pid << ". error " << GetLastError() << endl;
        cerr << "[!] check https://learn.microsoft.com/pt-br/windows/win32/debug/system-error-codes--0-499- for more information." << endl;
        return 1;
    }

    // carregar a NtQueryInformationProcess da ntdll.dll
    HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
    NtQueryInformationProcess_t NtQueryInformationProcess = 
        (NtQueryInformationProcess_t)GetProcAddress(hNtdll, "NtQueryInformationProcess");

    if (!hNtdll) {
        cerr << "[!] error to load NtQueryInformationProcess from ntdll.dll" << endl;
        CloseHandle(hProcess);
        return 1;
    }

    MY_PROCESS_INFORMATION mpi = {};
    ULONG returnLength         = 0;

    NTSTATUS status = NtQueryInformationProcess(
        hProcess,
        ProcessBasicInformation,
        &mpi,
        sizeof(mpi),
        &returnLength
    );

    if (status == 0) {
        cout << "[*] PEB address: " << mpi.PebBaseAddress << endl;
        cout << "[*] parent PID: " << mpi.InheritedFromUniqueProcessId << endl;
        cout << "[*] child PID: " << mpi.UniqueProcessId << endl;
    }
    else {
        cerr << "[!] NtQueryInformationProcess failed to retrieve informations from process with PID " << pid << endl;
     }

}
