#include <iostream>
#include <Windows.h>
#include <winuser.h>
#include <cstdlib>

using namespace std;

/*
int MessageBoxW(
  [in, optional] HWND    hWnd,
  [in, optional] LPCWSTR lpText,
  [in, optional] LPCWSTR lpCaption,
  [in]           UINT    uType
);
*/

int main(void) {

	int option = MessageBoxW(
		NULL,
		(LPCWSTR)L"pwn3d!",
		(LPCWSTR)L"WholeLottaR3d0xf",
		(UINT)MB_OK
	);

	switch (option) {
	case IDOK:
		system("calc");
		break;
	default:
		cout << "playboi carti";
	}

  return 0;

}

// apenas para demonstração
