#include "hge.h"
#include "hgefont.h"
#include "geometry/Geometry.h"
#include "geometry/Boosting.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "model/GameState.h"
#include "utils/LevelLoader.h"
#include "utils/Resources.h"

#define WIDTH 800

#define HIGHT 600
             
HGE *hge = 0;
GameState* state;
Resources res;

bool FrameFunc() {
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
	return false;
}
          
hgeFont* fnt;
		
bool RenderFunc() {
	
	HTARGET t =	state->env->compile(hge);	
	hge->Gfx_EndScene();
	hgeQuad quad; 
	quad.v[0].x=0;
	quad.v[0].y=0;
	quad.v[1].x=WIDTH;
	quad.v[1].y=0;
	quad.v[2].x=WIDTH;
	quad.v[2].y=HIGHT;
	quad.v[3].x=0;
	quad.v[3].y=HIGHT;
	quad.v[0].tx = 0;
	quad.v[0].tx = 0;
	quad.v[0].ty = 0;
	quad.v[1].tx = 1;
	quad.v[1].ty = 0;
	quad.v[2].tx = 1;
	quad.v[2].ty = 1;
	quad.v[3].tx = 0;
	quad.v[3].ty = 1;	
	quad.blend=BLEND_ALPHAADD | BLEND_COLORMUL | BLEND_ZWRITE;
	quad.tex = hge->Target_GetTexture(t);
	for(int i=0;i<4;i++) {
		quad.v[i].z=0.5f;
		quad.v[i].col=ARGB(255,255,255,255);
	}
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	hge->Gfx_RenderQuad(&quad);
	fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d (constant)", hge->Timer_GetDelta(), hge->Timer_GetFPS());
	hge->Gfx_EndScene();
	return false;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "Try");

	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, WIDTH);
	hge->System_SetState(HGE_SCREENHEIGHT, HIGHT);
	hge->System_SetState(HGE_SCREENBPP, 32);
	hge->System_SetState(HGE_FPS, 100);
	//hge->System_SetState(HGE_ZBUFFER, true);


	if (hge->System_Initiate()) {
		fnt=new hgeFont("font1.fnt");
 		fnt->SetColor(ARGB(255,0,0,0));	
		LevelLoader loader(hge);
		state = loader.load("level1.xml", res);
		hge->System_Start();
  	}	

  	hge->System_Shutdown();
	hge->Release();

	return 0;
}
