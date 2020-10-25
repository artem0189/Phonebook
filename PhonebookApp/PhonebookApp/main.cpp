#include <windows.h>

#define MAIN_CLASS_NAME L"MainWindow"
#define MAIN_WINDOW_NAME L"Phonebook"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	HWND hMainWindow;
	MSG msg;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.lpszClassName = MAIN_CLASS_NAME;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;

	RegisterClassEx(&wc);
	hMainWindow = CreateWindowEx(NULL, MAIN_CLASS_NAME, MAIN_WINDOW_NAME, WS_OVERLAPPED | WS_SYSMENU, 400, 100, 500, 600, NULL, NULL, hInstance, NULL);
	
	ShowWindow(hMainWindow, nCmdShow);
	UpdateWindow(hMainWindow);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}