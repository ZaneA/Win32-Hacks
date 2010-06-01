/*
 *	ExplorerGestures
 *	Enables simple Middle click gestures in Windows Explorer
 */

/* Windows defines and includes */
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT		0x0500
#include <windows.h>

#define THRESHOLD 60 /* Amount of pixels to move mouse up for it to be recognised as an up gesture */

/* Yay globals */
HHOOK hook = NULL;
POINT mouse;
int mouseDown = 0;

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	/* Apparently we're supposed to ignore if nCode < 0 */
	if (nCode < 0) return CallNextHookEx(hook, nCode, wParam, lParam);

	if (wParam == WM_MBUTTONDOWN) {
		char winClass[256];

		GetClassName(GetForegroundWindow(), winClass, sizeof(winClass));

		/* This is Windows Explorer, and probably other things, but most importantly it's Windows Explorer */
		if (!strcmp(winClass, "CabinetWClass")) {
			GetCursorPos(&mouse);
			mouseDown = 1;

			return 1; /* Handled */
		}
	} else if (wParam == WM_MBUTTONUP && mouseDown) {
		mouseDown = 0;
		POINT newMouse;
		GetCursorPos(&newMouse);

		if (mouse.y - THRESHOLD > newMouse.y) { /* Up - Backspace */
			char title[256];
			GetWindowText(GetForegroundWindow(), title, sizeof(title));

			INPUT keys[2];
			memset(keys, 0, sizeof(keys));

			keys[0].type = INPUT_KEYBOARD;
			keys[0].ki.wVk = VK_BACK;

			keys[1].type = INPUT_KEYBOARD;
			keys[1].ki.wVk = VK_BACK;
			keys[1].ki.dwFlags = KEYEVENTF_KEYUP;

			SendInput(2, keys, sizeof(INPUT)); /* Add keyboard event (Backspace) to event queue */

			int i;
			for (i = 0; i < strlen(title); i++) {
				INPUT refocus[2];
				memset(refocus, 0, sizeof(refocus));

				refocus[0].type = INPUT_KEYBOARD;
				refocus[0].ki.wVk = VkKeyScan(title[i]);

				refocus[1].type = INPUT_KEYBOARD;
				refocus[1].ki.wVk = VkKeyScan(title[i]);
				refocus[1].ki.dwFlags = KEYEVENTF_KEYUP;

				SendInput(2, refocus, sizeof(INPUT));
			}

		} else if (mouse.x - THRESHOLD > newMouse.x) { /* Left - Alt+Left */
			INPUT keys[4];
			memset(keys, 0, sizeof(keys));

			keys[0].type = INPUT_KEYBOARD;
			keys[0].ki.wVk = VK_MENU;

			keys[1].type = INPUT_KEYBOARD;
			keys[1].ki.wVk = VK_LEFT;

			keys[2].type = INPUT_KEYBOARD;
			keys[2].ki.wVk = VK_LEFT;
			keys[2].ki.dwFlags = KEYEVENTF_KEYUP;

			keys[3].type = INPUT_KEYBOARD;
			keys[3].ki.wVk = VK_MENU;
			keys[3].ki.dwFlags = KEYEVENTF_KEYUP;

			SendInput(4, keys, sizeof(INPUT));
		} else if (mouse.x + THRESHOLD < newMouse.x) { /* Right - Alt+Right */
			INPUT keys[4];
			memset(keys, 0, sizeof(keys));

			keys[0].type = INPUT_KEYBOARD;
			keys[0].ki.wVk = VK_MENU;

			keys[1].type = INPUT_KEYBOARD;
			keys[1].ki.wVk = VK_RIGHT;

			keys[2].type = INPUT_KEYBOARD;
			keys[2].ki.wVk = VK_RIGHT;
			keys[2].ki.dwFlags = KEYEVENTF_KEYUP;

			keys[3].type = INPUT_KEYBOARD;
			keys[3].ki.wVk = VK_MENU;
			keys[3].ki.dwFlags = KEYEVENTF_KEYUP;

			SendInput(4, keys, sizeof(INPUT));
		} else if (mouse.y + THRESHOLD < newMouse.y) { /* Down - Delete */
			INPUT keys[2];
			memset(keys, 0, sizeof(keys));

			keys[0].type = INPUT_KEYBOARD;
			keys[0].ki.wVk = VK_DELETE;

			keys[1].type = INPUT_KEYBOARD;
			keys[1].ki.wVk = VK_DELETE;
			keys[1].ki.dwFlags = KEYEVENTF_KEYUP;

			SendInput(2, keys, sizeof(INPUT)); /* Add keyboard event to event queue */

			//if (MessageBox(NULL, "Are you sure you want to exit ExplorerGestures?", "Exit?", MB_YESNO) == IDYES) {
			//	UnhookWindowsHookEx(hook);
			//	exit(0); /* Ugly */
			//}
		//} else {
//			return CallNextHookEx(hook, nCode, wParam, lParam);
		}

		return 1; /* Handled */
	}

	return CallNextHookEx(hook, nCode, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MSG msg;

	if (!(hook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)LowLevelMouseProc, hInstance, 0))) {
		MessageBox(NULL, "Failed to set Hook :(", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	while (GetMessage(&msg, NULL, 0, 0) > 0); /* GetMessage forever <3 */

	UnhookWindowsHookEx(hook); /* Probably pointless as you have to end task this process to kill it anyway */

	return 0;
}
