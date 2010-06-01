//
// Gapslock
// Copyright 2010, Zane Ashby, http://demonastery.org
//

#define WIN32_LEAN_AND_MEAN
#define WINVER			0x0501
#define _WIN32_WINNT		0x0501
#include <windows.h>
#include <shellapi.h>
#include <stdlib.h>
#include <stdio.h>

HHOOK key_hook;

char buffer[256] = { 0 };

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) {
		return CallNextHookEx(key_hook, nCode, wParam, lParam);
	}

	static int active = 0;

	if (nCode == HC_ACTION) {
		switch (wParam)
		{
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				{
					PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;

					if (key->vkCode == VK_CAPITAL) {
						active = !active;
						memset(buffer, 0, sizeof(buffer));

						return 1;
					} else if (active) {
						if (key->vkCode == VK_RETURN) {
							// GO
							char final[512] = { 0 };
							sprintf(final, "http://google.com/search?q=%s", buffer);
							ShellExecute(NULL, "open", final, NULL, NULL, SW_SHOWNORMAL);
							active = 0;
						} else {
							int code = key->vkCode;
							if (code > 64 && code < 91) code += 32;
							if (code == 96) code = 43;
							sprintf(buffer, "%s%c", buffer, (char)code);
						}

						return 1;
					}
				}
		}
	}

	return CallNextHookEx(key_hook, nCode, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MSG msg = { 0 };

	// Fatal
	if (!(key_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, 0))) {
		MessageBox(NULL, "Failed to set Keyboard Hook", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	// Pump messages
	while (GetMessage(&msg, NULL, 0, 0) > 0);

	return msg.wParam;
}
