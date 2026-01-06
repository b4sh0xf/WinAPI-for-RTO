#include <iostream>
#include <Windows.h>

using namespace std;

/*
LSTATUS RegOpenKeyW(
  [in]           HKEY    hKey,
  [in, optional] LPCWSTR lpSubKey,
  [out]          PHKEY   phkResult
);

LSTATUS RegQueryValueExW(
  [in]                HKEY    hKey,
  [in, optional]      LPCWSTR lpValueName,
                      LPDWORD lpReserved,
  [out, optional]     LPDWORD lpType,
  [out, optional]     LPBYTE  lpData,
  [in, out, optional] LPDWORD lpcbData
);
*/

int main(void) {

    HKEY hKey        = HKEY_LOCAL_MACHINE;
    LPCWSTR lpSubKey = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";
    
    if (RegOpenKeyExW(hKey, lpSubKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        wchar_t value[256]; // long pointer
        DWORD value_length = sizeof(value);
        DWORD type         = 0;

        if (RegQueryValueExW(hKey, L"ProductName", NULL, &type, (LPBYTE)value, &value_length) == ERROR_SUCCESS) {
            wcout << L"[+] your windows version is: " << value << endl;
        }
        else {
            cout << "[!] error" << endl;
          return 1;
        }
    }

    return 0;

}
