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
#include "model/IBody.h"

#define WIDTH 800

#define HIGHT 600
             
HGE *hge = 0;
GameState* state;
Resources res;
hgeFont* fnt;
Camera* cam;

struct Dot : IBody {
    virtual double getViewAngle() {return 0;}
    Dot() {
    	pos = Geo::Polygon({{400, 300},{410,300},{410,310},{400,310}});
    }
    virtual void render(HGE* hge) {
    	for (int i = 0; i < (int)pos.size(); ++i) {    		
    		hge->Gfx_RenderLine(pos[i].x, pos[i].y, pos[(i+1)%pos.size()].x, pos[(i+1)%pos.size()].y, 0xFFFF0000, 0.4); 
    	}
    }
};

bool FrameFunc() {
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
	cam->frame(hge);
	return false;
}          
		
bool RenderFunc() {
	cam->view(hge);
	//fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d (constant)", hge->Timer_GetDelta(), hge->Timer_GetFPS());
	//std::cerr << hge->Timer_GetFPS() << "\n";
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
		IBody* bd = new Dot();
		state->addBody(bd);
		cam = new Camera(state, bd, Geo::Vector(400,300), 80*5, 60*5, WIDTH, HIGHT);
		hge->System_Start();
  	}	

  	hge->System_Shutdown();
	hge->Release();

	return 0;
}
