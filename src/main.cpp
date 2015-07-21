#include "hge.h"
#include "hgefont.h"
#include "geometry/Geometry.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "model/GameState.h"
#include "utils/LevelLoader.h"
#include "model/Camera.h"
#include "model/IBody.h"
#include <hgeresource.h>

#define WIDTH 800

#define HIGHT 600
             
HGE *hge = 0;

/*GameState* state;
hgeFont* fnt;
Camera* cam;
IBody* bd;

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
    virtual void frame(HGE* hge) {
    	for (Geo::Vector& v : pos.points) {
        	if (hge->Input_GetKeyState(HGEK_UP)) 
        	    v.y -= 5;
        	if (hge->Input_GetKeyState(HGEK_DOWN)) 
           		v.y += 5;
        	if (hge->Input_GetKeyState(HGEK_RIGHT)) 
            	v.x += 5;
        	if (hge->Input_GetKeyState(HGEK_LEFT)) 
            	v.x -= 5;
        }     
    }
};
*/
bool FrameFunc() {
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
	//cam->frame(hge);
	//bd->frame(hge);
	return false;
}          
		
bool RenderFunc() {
	//cam->view(hge);
	//fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d (constant)", hge->Timer_GetDelta(), hge->Timer_GetFPS());
	//std::cerr << hge->Timer_GetFPS() << "\n";
	return false;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LevelLoader loader;
	GameState* state;
	hgeResourceManager* res;
	loader.load("level1.tmx", "", state, res);
	/*hge = hgeCreate(HGE_VERSION);

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
		hgeResourceManager* res = new hgeResourceManager();
		HTEXTURE tex = hge->Texture_Load("crate1.png");
		std::cerr << res->GetTexture("crate1.png") << "\n" << tex;
		hge->System_Start();
  	}	

  	hge->System_Shutdown();
	hge->Release();
	*/
	return 0; 
}
