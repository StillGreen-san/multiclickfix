#include "dll.hpp"
#include "wingui.hpp"

int main()
{
	SetHook();

	MessageQueue();

	RemoveHook();

	return 0;
}
