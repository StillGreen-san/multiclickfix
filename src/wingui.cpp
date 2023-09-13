#include "wingui.hpp"

#include <Windows.h>

#include <cwchar>

void MessageQueue()
{
	MSG msg;
	while(GetMessageW(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

constexpr const wchar_t* TrayInstanceClassName = L"mcfexeTrayInstanceClassName";
constexpr UINT TrayInstanceExitId = 2001;
enum : UINT
{
	WM_ICON_SYS = WM_APP,
	WM_ICON_APP
};

BOOL TrayInstanceShowPopupMenu(HWND owner)
{
	HMENU hPop = CreatePopupMenu();
	InsertMenuW(hPop, 0, MF_BYPOSITION | MF_STRING, TrayInstanceExitId, L"Exit");

	POINT point;
	GetCursorPos(&point);

	const UINT itemId = TrackPopupMenu(
	    hPop, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, point.x, point.y, {}, owner, nullptr);
	SendMessageW(owner, WM_ICON_APP, itemId, 0);

	DestroyMenu(hPop);

	return 0;
}

void TrayInstanceRemoveTrayIcon(HWND hWnd)
{
	NOTIFYICONDATAW nid{};
	nid.cbSize = sizeof(nid);
	nid.hWnd = hWnd;
	Shell_NotifyIconW(NIM_DELETE, &nid);
}

void TrayInstanceAddTrayIcon(HWND hWnd)
{
	NOTIFYICONDATAW nid{};
	nid.cbSize = sizeof(nid);
	nid.hWnd = hWnd;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_ICON_SYS;
	ExtractIconExW(L"%SystemRoot%\\System32\\mmcndmgr.dll", 50, nullptr, &(nid.hIcon), 1);
	wcscpy_s(nid.szTip, L"multiclickfix");
	Shell_NotifyIconW(NIM_ADD, &nid);
}

LRESULT TrayInstanceWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		TrayInstanceAddTrayIcon(hWnd);
		return 0;
	case WM_CLOSE:
		TrayInstanceRemoveTrayIcon(hWnd);
		PostQuitMessage(0);
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	case WM_ICON_SYS:
		if(lParam == WM_RBUTTONUP)
		{
			SetForegroundWindow(hWnd);
			TrayInstanceShowPopupMenu(hWnd);
		}
		return 0;
	case WM_ICON_APP:
		if(wParam == TrayInstanceExitId)
		{
			PostMessageW(hWnd, WM_CLOSE, 0, 0);
		}
		return 0;
	default:
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}
}

bool CreateTrayInstance()
{
	if(FindWindowExW(HWND_MESSAGE, nullptr, TrayInstanceClassName, nullptr))
	{
		return false;
	}

	HMODULE hModule = GetModuleHandleW(nullptr);

	WNDCLASSW wndClass = {0};
	wndClass.lpfnWndProc = TrayInstanceWndProc;
	wndClass.hInstance = hModule;
	wndClass.lpszClassName = TrayInstanceClassName;

	RegisterClassW(&wndClass);

	CreateWindowExW(0, TrayInstanceClassName, nullptr, 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
	    HWND_MESSAGE, nullptr, hModule, nullptr);

	return true;
}
void CleanupTrayInstance()
{
	UnregisterClassW(TrayInstanceClassName, GetModuleHandleW(nullptr));
}
