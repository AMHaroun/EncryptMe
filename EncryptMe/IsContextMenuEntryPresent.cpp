#include <Windows.h>


bool IsContextMenuEntryPresent()
{
	const wchar_t* CONTEXT_MENU_KEY = L"*\\shell\\EncryptMe";
	const wchar_t* COMMAND_KEY = L"*\\shell\\EncryptMe\\command";

	HKEY hContextMenuKey;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, CONTEXT_MENU_KEY, 0, KEY_READ, &hContextMenuKey) == ERROR_SUCCESS)
	{
		RegCloseKey(hContextMenuKey);
		return true;
	}
	return false;
}