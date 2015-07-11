#include "hge.h"
#include "geometry/Boosting.h"
#include "geometry/Geometry.h"
#include <iostream>

HGE *hge = 0;

bool FrameFunc()
{
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);

	hge->System_SetState(HGE_TITLE, "HGE Tutorial 01 - Minimal HGE application");
	hge->System_SetState(HGE_WINDOWED, true);

	hge->System_SetState(HGE_USESOUND, false);
	if(hge->System_Initiate())
	{
		hge->System_Start();
	}
	else
	{
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}

	hge->System_Shutdown();

	hge->Release();

	return 0;
}
