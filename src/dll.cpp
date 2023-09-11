#include "dll.hpp"

#define WIN32_LEAN_AND_MEAN
#define UNICODE
#include <Windows.h>

constexpr LRESULT STOP_HOOK_CHAIN_RESULT = 1;
constexpr DWORD MIN_TIME_DELTA = 24;

HHOOK hkKey = nullptr;
HINSTANCE hInstHookDll = nullptr;
bool leftButtonDownSkipped = false;
DWORD lastClickTime = 0;

BOOL /*APIENTRY*/ DllMain(HANDLE hModule, DWORD reasonForCall, LPVOID lpReserved)
{
	switch(reasonForCall)
	{
	case DLL_PROCESS_ATTACH:
		hInstHookDll = static_cast<HINSTANCE>(hModule);
		break;
	}
	return TRUE;
}

LRESULT /*CALLBACK*/ LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode < 0)
	{
		return CallNextHookEx(hkKey, nCode, wParam, lParam);
	}
	MSLLHOOKSTRUCT& msllhookstruct = *reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
	if(msllhookstruct.flags & LLMHF_INJECTED)
	{
		return CallNextHookEx(hkKey, nCode, wParam, lParam);
	}
	const DWORD timeDelta = msllhookstruct.time - lastClickTime;
	switch(wParam)
	{
	case WM_LBUTTONDOWN:
		if(timeDelta < MIN_TIME_DELTA)
		{
			leftButtonDownSkipped = true;
			return STOP_HOOK_CHAIN_RESULT;
		}
		break;
	case WM_LBUTTONUP:
		if(leftButtonDownSkipped)
		{
			leftButtonDownSkipped = false;
			return STOP_HOOK_CHAIN_RESULT;
		}
		if(timeDelta < MIN_TIME_DELTA)
		{
			return STOP_HOOK_CHAIN_RESULT;
		}
		lastClickTime = msllhookstruct.time;
		break;
//	case WM_RBUTTONDOWN:
//	case WM_RBUTTONUP:
//	case WM_MOUSEMOVE:
//	case WM_MOUSEWHEEL:
	}
	return CallNextHookEx(hkKey, nCode, wParam, lParam);
}

void /*__stdcall*/ SetHook()
{
	if(hkKey == nullptr)
	{
		hkKey = SetWindowsHookExW(WH_MOUSE_LL, LowLevelMouseProc, hInstHookDll, 0);
	}
}

void /*__stdcall*/ RemoveHook()
{
	if(hkKey != nullptr)
	{
		UnhookWindowsHookEx(hkKey);
	}
	hkKey = nullptr;
}
