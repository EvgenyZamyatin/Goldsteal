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
#include "InputData.h"
#include "model/brains/IBrain.h"
#include "model/FlashLight.h"

#define WIDTH 800

#define HIGHT 600
             
HGE *hge = 0;

GameState* state;
hgeResourceManager* res;
Camera* cam;
InputData* input;
FlashLight*  fl;

bool FrameFunc() {
	//std::cerr << state->getHero()->getPosition() << "\n";
	double start = clock();
	//std::cerr << state->getHero()->getPosition() << "\n";
	//std::cerr << state->getEnvironment()->getObjects()[0]->getBounds() << "\n";
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
	input->update(hge, cam);
	state->frame();
	cam->frame(input, false);	
	std::cerr << "TOTAL FRAME: " << (clock()-start)/CLOCKS_PER_SEC << "\n";
	return false;
}          
		
bool RenderFunc() {
	double start = clock();
	std::cerr << hge->Timer_GetFPS() << "\n";
	state->render(hge, cam);
	std::cerr << "TOTAL REND: " << (clock()-start)/CLOCKS_PER_SEC << "\n";
	return false;
}

HSHADER shader=NULL;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_LOGFILE, "main.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "Try");

	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, WIDTH);
	hge->System_SetState(HGE_SCREENHEIGHT, HIGHT);
	hge->System_SetState(HGE_SCREENBPP, 32);
	hge->System_SetState(HGE_FPS, 50);
	hge->System_SetState(HGE_ZBUFFER, true);		
	hge->System_SetState(HGE_HIDEMOUSE, false);
	if (hge->System_Initiate()) {
		input = new InputData();
	    res = new hgeResourceManager("level.res");
	    IBrain::initBrains(input);
	    Tmx::Map map;
		map.ParseFile("level2.tmx");
	    state = new GameState(&map, res);
	    cam = new Camera(state->getEnvironment()->getWidth(), state->getEnvironment()->getHight(), 
					{400,300}, 80*10, 60*10, 800, 600, 150);
		cam->bind(state->getHero());		
		
		fl = new FlashLight(res, state->getHero(), {0,0}, M_PI/5);
		state->getEnvironment()->addLightSource(fl);
		hge->System_Start();
  	}	

  	hge->System_Shutdown();
	hge->Release();
	return 0; 
}
