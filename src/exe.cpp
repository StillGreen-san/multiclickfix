#include "dll.hpp"

#include <thread>

using namespace std::chrono_literals;

#define WIN32_LEAN_AND_MEAN
#define UNICODE
#include <Windows.h>

int main()
{
	SetHook();

	MSG msg;
	while(GetMessageW(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	RemoveHook();

	return 0;
}
