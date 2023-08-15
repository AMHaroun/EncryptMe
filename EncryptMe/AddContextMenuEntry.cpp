#include <Windows.h>


void AddContextMenuEntry()
{
	const wchar_t* CONTEXT_MENU_KEY = L"*\\shell\\EncryptMe";
	const wchar_t* COMMAND_KEY = L"*\\shell\\EncryptMe\\command";

	wchar_t programPath[MAX_PATH];
	GetModuleFileName(NULL, programPath, MAX_PATH);

	HKEY hContextMenuKey;
	RegCreateKey(HKEY_CLASSES_ROOT, CONTEXT_MENU_KEY, &hContextMenuKey);

	// Set the display name for the context menu entry
	RegSetValueEx(hContextMenuKey, NULL, 0, REG_SZ, (BYTE*)L"Encrypt with EncryptMe", sizeof(L"Encrypt with EncryptMe"));

	// Create the command key
	HKEY hCommandKey;
	RegCreateKey(hContextMenuKey, L"command", &hCommandKey);

	RegSetValueEx(hCommandKey, NULL, 0, REG_SZ, (BYTE*)programPath, (wcslen(programPath) + 1) * sizeof(wchar_t));

	RegCloseKey(hCommandKey);
	RegCloseKey(hContextMenuKey);

	return;
}