#include <windows.h>
#include "FollowMe.h"


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdline,int show)
{
	FollowMe dt;
	dt.create(hInstance, 700, 700, 100, true);
	return 0;
}