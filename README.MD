utility program to prevent accidental double clicks caused by a faulty mouse button

windows only, only tested on win10

works by hooking LowLevelMouseProc and consuming/blocking click events that happen to fast after one another

documentation of SetWindowsHook suggests needing both 32&64 bit versions, but that seems to not be the case (at least for this use case), 
both 32&64 bit applications are supported with both 32&64 bit versions of this program.

however some apps like taskmanager are only covered when this program is run as admin

usage: "mcfexe.exe MinClickDelta" 
where MinClickDelta is the minimum time in milliseconds that need to pass before a second click is accepted, default is 24

only 1 instance can run at a time
