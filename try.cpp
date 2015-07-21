/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hge_tut02 - Using input, sound and rendering
*/


// Copy the files "particles.png" and "menu.wav"
// from the folder "precompiled" to the folder with
// executable file. Also copy hge.dll and bass.dll
// to the same folder.


#include "hge.h"
#include <vector>


HGE *hge=0;

hgeQuad quad;
HTEXTURE tex;

inline void fillQuad(hgeQuad& quad, std::vector<hgeVertex> v) {
	for (int i = 0; i < 4; ++i)
		quad.v[i]=v[i];
}


bool FrameFunc()
{       
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
    return false;
}
bool RenderFunc()
{
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	quad.tex=tex;
	quad.blend=BLEND_DEFAULT_Z;
	hgeU32 col = ARGB(255, 255,255,255);
	fillQuad(quad, {
					{0,0,0.5,col,0,0},
					{800,0,0.5,col,1,0},
					{800,600,0.5,col,1,1},
					{0,600,0.5,col,0,1}
				   });	
	hge->Gfx_RenderQuad(&quad);
	quad.tex=tex;
	quad.blend=BLEND_DEFAULT_Z;
	col = ARGB(255,255,255,255);
	fillQuad(quad, {
					{0,0,0.5,col,0,0},
					{100,0,0.5,col,1.0,0},
					{100,100,0.5,col,1.0,1.0},
					{0,100,0.5,col,0,1.0}
				   });	
	hge->Gfx_RenderQuad(&quad);
	hge->Gfx_EndScene();
	return false;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Get HGE interface
	hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_LOGFILE, "hge_tut02.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "HGE Tutorial 02 - Using input, sound and rendering");

	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, 800);
	hge->System_SetState(HGE_SCREENHEIGHT, 600);
	hge->System_SetState(HGE_SCREENBPP, 32);

	if(hge->System_Initiate())
	{
		tex=hge->Texture_Load("../resources/textures/crate1.png");
		hge->System_Start();               
	}
	hge->System_Shutdown();
	hge->Release();
	return 0;
}
