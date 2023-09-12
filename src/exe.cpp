#include "dll.hpp"
#include "wingui.hpp"

#include <cstdio>
#include <cstdlib>

int main(int argc, const char* argv[])
{
	unsigned minClickDelta = MIN_CLICK_DELTA_DEFAULT;

	if(argc > 1)
	{
		char* strEnd = nullptr;
		const unsigned userMinDelta = std::strtoul(argv[1], &strEnd, 10);
		if(strEnd == argv[1])
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
