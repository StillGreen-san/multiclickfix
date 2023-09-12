#include "dll.hpp"
#include "wingui.hpp"

int main()
{
	SetMinClickDelta(24);
	SetHook();

	MessageQueue();

	RemoveHook();

	return 0;
}
