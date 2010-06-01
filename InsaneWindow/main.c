#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int main(int argc, char **argv)
{
	int x, y;

	printf("Enter a resolution to run at (eg. 1024x768): ");

	scanf("%ix%i", &x, &y);

	printf("Resolution will be %ix%i, waiting for 1NSANE window..\n", x, y);

	while (1) {
		char winClass[256];
		HWND wind = GetForegroundWindow();

		GetClassName(wind, winClass, sizeof(winClass));

		if (!strcmp(winClass, "InsaneWinClass")) {
			printf("Found window, setting resolution and exiting..\n");

			SetWindowPos(wind, HWND_TOP, 200, 100, x, y, SWP_FRAMECHANGED);

			exit(0);
		}

		Sleep(200);
	}

	// Should never get here

	return 1;
}
