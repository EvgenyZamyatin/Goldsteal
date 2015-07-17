#include "hge.h"
#include "hgefont.h"
#include "geometry/Geometry.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "model/GameState.h"
#include "utils/LevelLoader.h"
#include "utils/Resources.h"
#include "model/Camera.h"

#define WIDTH 800

#define HIGHT 600
             
HGE *hge = 0;
GameState* state;
Resources res;
hgeFont* fnt;
Camera* cam;

bool FrameFunc() {
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
	cam->frame(hge);
	return false;
}          
		
bool RenderFunc() {
	cam->view(hge);
	//fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d (constant)", hge->Timer_GetDelta(), hge->Timer_GetFPS());
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
	hge->System_SetState(HGE_ZBUFFER, true);


	if (hge->System_Initiate()) {
		fnt=new hgeFont("font1.fnt");
 		fnt->SetColor(ARGB(255,0,0,0));
		LevelLoader loader(hge);
		state = loader.load("level1.xml", res);
		state->screenWidth = WIDTH;
		state->screenHight = HIGHT;
		cam = new Camera(state, NULL, Geo::Vector(400,300), 80*5, 60*5);
		hge->System_Start();
  	}	

  	hge->System_Shutdown();
	hge->Release();

	return 0;
}
