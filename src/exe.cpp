#include "dll.hpp"
#include "wingui.hpp"

#include <cstdio>
#include <cstdlib>

int WinMain()
{
	unsigned minClickDelta = MIN_CLICK_DELTA_DEFAULT;

	if(__argc > 1)
	{
		char* strEnd = nullptr;
		const unsigned userMinDelta = std::strtoul(__argv[1], &strEnd, 10);
		if(strEnd == __argv[1])
		{
			std::puts("usage: mcfexe.exe <MinClickDelta>\n");
			return 1;
		}
		minClickDelta = userMinDelta;
	}

	if(!CreateTrayInstance())
	{
		return 1;
	}
	SetMinClickDelta(minClickDelta);
	SetHook();

	MessageQueue();

	RemoveHook();
	CleanupTrayInstance();

	return 0;
}
