//
// MouseSlow
// Copyright 2010, Zane Ashby, http://demonastery.org
//


//
// Includes
//

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT		0x0500
#include <windows.h>

//
// Defines
//

// Timers
#define TIMER_CHECKMOUSE	1

int mousespeed = 10;
int in_window = 0;

//HHOOK key_hook;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Normal messages
	switch (msg)
	{
		case WM_CREATE:
			SetTimer(hwnd, TIMER_CHECKMOUSE, 1, NULL);
			break;

		case WM_CLOSE:
			KillTimer(hwnd, TIMER_CHECKMOUSE);
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(WM_QUIT);
			break;

		case WM_TIMER:
			{
				POINT mousepos;
				GetCursorPos(&mousepos);

				RECT windowrect;
				GetWindowRect(GetForegroundWindow(), &windowrect);

				if ((mousepos.x > windowrect.right - 36) &&
					(mousepos.x < windowrect.right) &&
					(mousepos.y > windowrect.top) &&
					(mousepos.y < windowrect.bottom)) {
					if (!in_window) {
						int newpos = mousespeed / 5;

						SystemParametersInfo(SPI_SETMOUSESPEED, 0, newpos, SPIF_SENDCHANGE);
						in_window = 1;
					}
				} else {
					if (in_window) {
						int newpos = mousespeed;

						SystemParametersInfo(SPI_SETMOUSESPEED, 0, newpos, SPIF_SENDCHANGE);
						in_window = 0;
					}
				}

			}
			break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MSG msg = { 0 };

	WNDCLASSEX winClass = {
		.cbSize = sizeof(WNDCLASSEX),
		.lpfnWndProc = WndProc,
		.hInstance = hInstance,
		.lpszClassName = "MouseSlow"
	};

	// Fatal, this shouldn't happen though
	if (!RegisterClassEx(&winClass)) {
		MessageBox(NULL, "Failed to Register Window Class", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	// For Window position
//	RECT rc;
//	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);

	SystemParametersInfo(SPI_GETMOUSESPEED, 0, &mousespeed, 0);

	HWND hwnd = CreateWindowEx(0, "MouseSlow", "MouseSlow", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, hInstance, NULL);

	if (!hwnd) {
		MessageBox(NULL, "Failed to Create Message Window", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	// Pump messages
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
