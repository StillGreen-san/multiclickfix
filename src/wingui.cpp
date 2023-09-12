#include "wingui.hpp"

#define WIN32_LEAN_AND_MEAN
#define UNICODE
#include <Windows.h>

void MessageQueue()
{
	MSG msg;
	while(GetMessageW(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}
