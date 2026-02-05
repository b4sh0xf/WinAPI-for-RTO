#include <iostream>
#include <windows.h>

/*
int MessageBoxW(
  [in, optional] HWND    hWnd,
  [in, optional] LPCWSTR lpText,
  [in, optional] LPCWSTR lpCaption,
  [in]           UINT    uType
);

FARPROC GetProcAddress(
  [in] HMODULE hModule,
  [in] LPCSTR  lpProcName -> procedure address
);

HMODULE GetModuleHandleA(
  [in, optional] LPCSTR lpModuleName
);
*/

// creating a custom version of MessageBox function
// EDR/AV evasion
typedef int (WINAPI *CustomMBox) (
	HWND    hWnd,
	LPCWSTR lpText,
	LPCWSTR lpCaption,
	UINT    uType
);

using namespace std;

int main() {
	CustomMBox customMBox;

	// we use LoadLibraryA instead of GetModuleHandleA 'cause none of these functions come from user32.dll
	if (LoadLibraryA("user32.dll") == 0) {
		cerr << "[!] error to load user32.dll" << endl;
		cerr << "[!] code: " << GetLastError() << endl;
		exit(GetLastError());
	}

	customMBox = (CustomMBox) GetProcAddress(
		LoadLibraryA("user32.dll"),
		"MessageBoxW"
		// solving dynamically the address of MessageBoxA from user32.dll
	);
	customMBox(0, L"pwn", L"oi", 0);

	return (0);
}
