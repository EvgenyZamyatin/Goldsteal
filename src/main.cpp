#include "hge.h"
#include "hgefont.h"
#include "geometry/Geometry.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "model/GameState.h"  
#include "model/Camera.h"
#include "model/IBody.h"
#include <hgeresource.h>

#define WIDTH 800

#define HIGHT 600
             
HGE *hge = 0;

GameState* state;
hgeResourceManager* res;
Camera* cam;

bool FrameFunc() {
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
	float a,b;
	hge->Input_GetMousePos(&a,&b);
	/*int evt = 0;
	if (hge->Input_KeyDown(HGEK_W))
		evt |= Hero::INPUT_UP;
	else
		if (hge->Input_KeyDown(HGEK_S))
			evt |= Hero::INPUT_DOWN;


	if (hge->Input_KeyDown(HGEK_D))
		evt |= Hero::INPUT_RIGHT;
	else
		if (hge->Input_KeyDown(HGEK_A))
			evt |= Hero::INPUT_LEFT;

	state->getHero()->frame(evt, Geo::Vector(a,b));*/
	cam->frame(Geo::Vector(a,b), true);
	//bd->frame(hge);
	//cam->view(hge, res);
	return false;
}          
		
bool RenderFunc() {
	//cam->view(hge);
	//fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d (constant)", hge->Timer_GetDelta(), hge->Timer_GetFPS());
	//std::cerr << hge->Timer_GetFPS() << "\n";
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	state->getEnvironment()->render(hge, cam);
	for (IObject* o : state->getEnvironment()->getObjects())
		o->render(hge, cam);
	//state->getHero()->render(hge, cam);
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
	hge->System_SetState(HGE_ZBUFFER, true);		
	hge->System_SetState(HGE_HIDEMOUSE, false);

	if (hge->System_Initiate()) {
		//LevelLoader loader;
		//loader.load(hge, "level.tmx", "level.res", state, res);
	    //return 0;
	    res = new hgeResourceManager("level.res");
	    Tmx::Map map;
		map.ParseFile("level.tmx");
	    state = new GameState(&map, res);
	    cam = new Camera(state->getEnvironment()->getWidth(), state->getEnvironment()->getHight(), 
	    					{400,300}, 80*6, 60*6, 800, 600, 50);
		hge->System_Start();
  	}	

  	hge->System_Shutdown();
	hge->Release();
	return 0; 
}
