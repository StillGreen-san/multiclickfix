#include "dll.hpp"

#include <Windows.h>

constexpr LRESULT STOP_HOOK_CHAIN_RESULT = 1;
DWORD minTimeDelta = MIN_CLICK_DELTA_DEFAULT;

struct Button
{
	bool handleDown(DWORD clickTime)
	{
		if(clickTime - lastClickTime < minTimeDelta)
		{
			downSkipped = true;
			return true;
		}
		return false;
	}
	bool handleUp(DWORD clickTime)
	{
		if(downSkipped)
		{
			downSkipped = false;
			return true;
		}
		if(clickTime - lastClickTime < minTimeDelta)
		{
			return true;
		}
		lastClickTime = clickTime;
		return false;
	}
	bool downSkipped = false;
	DWORD lastClickTime = 0;
};

HHOOK hkKey = nullptr;
HINSTANCE hInstHookDll = nullptr;
Button leftClick;
Button rightClick;

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

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode < 0)
	{
		return CallNextHookEx(hkKey, nCode, wParam, lParam);
	}
	const MSLLHOOKSTRUCT& msllhookstruct = *reinterpret_cast<const MSLLHOOKSTRUCT*>(lParam);
	if(msllhookstruct.flags & LLMHF_INJECTED)
	{
		return CallNextHookEx(hkKey, nCode, wParam, lParam);
	}
	switch(wParam)
	{
	case WM_LBUTTONDOWN:
		if(leftClick.handleDown(msllhookstruct.time))
		{
			return STOP_HOOK_CHAIN_RESULT;
		}
		break;
	case WM_LBUTTONUP:
		if(leftClick.handleUp(msllhookstruct.time))
		{
			return STOP_HOOK_CHAIN_RESULT;
		}
		break;
	case WM_RBUTTONDOWN:
		if(rightClick.handleDown(msllhookstruct.time))
		{
			return STOP_HOOK_CHAIN_RESULT;
		}
		break;
	case WM_RBUTTONUP:
		if(rightClick.handleUp(msllhookstruct.time))
		{
			return STOP_HOOK_CHAIN_RESULT;
		}
		break;
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

void /*__stdcall*/ SetMinClickDelta(unsigned delta)
{
	minTimeDelta = delta;
}
