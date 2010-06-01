/*
 * App to create folder in current directory formatted as %d%m%y
 */

#include <windows.h>
#include <time.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	time_t t = time(NULL);
	struct tm *tmp = localtime(&t);
	char path[MAX_PATH];

	char datepath[16];
	strftime(datepath, sizeof(datepath), "%d%m%y", tmp);

	GetCurrentDirectory(MAX_PATH, path);
	strcat(path, "\\");
	strcat(path, datepath);

	CreateDirectory(path, NULL);
/*
	DWORD attribs = GetFileAttributes(path);
	if (attribs == INVALID_FILE_ATTRIBUTES || !(attribs & FILE_ATTRIBUTE_DIRECTORY)) {
		ShellExecute(NULL, "open", root, NULL, NULL, SW_SHOW);
	} else {
		ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOW);
	}*/

	return 0;
}
