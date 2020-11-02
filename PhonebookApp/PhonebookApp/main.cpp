#include <windows.h>
#include <commctrl.h>
#include <vector>
#include "PhonebookCore/Phonebook.h"

#pragma comment(lib, "ComCtl32.Lib")

#define MAIN_CLASS_NAME L"MainWindow"
#define MAIN_WINDOW_NAME L"Phonebook"
#define IDC_LISTVIEW 1111

VOID InsertListViewItem(HWND hWndListView)
{
	LVITEM lvI;

	lvI.pszText = LPSTR_TEXTCALLBACK;
	lvI.mask = LVIF_TEXT;
	lvI.iItem = 0;
	lvI.iSubItem = 0;

	ListView_InsertItem(hWndListView, &lvI);

	ListView_SetItemText(hWndListView, 0, 0, const_cast<LPWSTR>(L"gdfh"));
	ListView_SetItemText(hWndListView, 0, 1, const_cast<LPWSTR>(L"gdfh1"));
	ListView_SetItemText(hWndListView, 0, 2, const_cast<LPWSTR>(L"gdfh2"));
}

VOID InitListViewColumns(HWND hWndListView)
{
	RECT rcListView;
	LVCOLUMN lvc;
	
	GetClientRect(hWndListView, &rcListView);
	lvc.mask = LVCF_WIDTH | LVCF_TEXT;
	lvc.cx = (rcListView.right - rcListView.left) / columnsName.size();
	for (int i = 0; i < columnsName.size(); i++) {
		lvc.pszText = const_cast<wchar_t*>(columnsName[i]);
		ListView_InsertColumn(hWndListView, i, &lvc);
	}
}

HWND CreateListView(HWND hWndParent)
{
	HWND hWndListView;
	INITCOMMONCONTROLSEX icex;
	RECT rcClient;

	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);
	GetClientRect(hWndParent, &rcClient);
	
	hWndListView = CreateWindowEx(NULL, WC_LISTVIEW, L"", WS_CHILD | LVS_REPORT | LVS_EDITLABELS, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, hWndParent, (HMENU)IDC_LISTVIEW, GetModuleHandle(NULL), NULL);
	return hWndListView;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hWndListView;

	if (uMsg == WM_CREATE) {
		hWndListView = CreateListView(hWnd);
		InitListViewColumns(hWndListView);
		ShowWindow(hWndListView, SW_SHOWDEFAULT);
	}

	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(NULL);
		return NULL;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	HWND hMainWindow;
	MSG msg;
	WNDCLASSEX wc;

	GetPhonebook();

	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.lpszClassName = MAIN_CLASS_NAME;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;

	RegisterClassEx(&wc);
	hMainWindow = CreateWindowEx(NULL, MAIN_CLASS_NAME, MAIN_WINDOW_NAME, WS_OVERLAPPED | WS_SYSMENU, 400, 100, 576, 600, NULL, NULL, hInstance, NULL);
	
	ShowWindow(hMainWindow, nCmdShow);
	UpdateWindow(hMainWindow);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}