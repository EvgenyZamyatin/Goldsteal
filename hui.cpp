#include "hge.h"
#include "hgefont.h"
#include "src/geometry/Geometry.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define WIDTH 800

#define HIGHT 600
             
HGE *hge = 0;

bool FrameFunc() {
	return false;
}          
		
bool RenderFunc() {
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);

	hgeQuad quad;
	quad.tex=0;
	quad.blend=BLEND_DEFAULT_Z;             
	quad.v[0].x=0,quad.v[0].y=0,quad.v[0].z=0.4,quad.v[0].col=0xFFFF0000;
	quad.v[1].x=1000000,quad.v[1].y=0,quad.v[1].z=0.4,quad.v[1].col=0xFFFF0000;
	quad.v[2].x=1000000,quad.v[2].y=1000000,quad.v[2].z=0.4,quad.v[2].col=0xFFFF0000;
	quad.v[3].x=0,quad.v[3].y=1000000,quad.v[3].z=0.4,quad.v[3].col=0xFFFF0000;
	
	quad.v[0].tx=0,quad.v[0].ty=0;
	quad.v[1].tx=1,quad.v[1].ty=0;
	quad.v[2].tx=1,quad.v[2].ty=1;
	quad.v[3].tx=0,quad.v[3].ty=1;

	hge->Gfx_RenderQuad(&quad);
	std::cerr << hge->Timer_GetFPS() << "\n";
		
	quad.v[0].x=-1000000,quad.v[0].y=1000000,quad.v[0].z=0.3,quad.v[0].col=0xFF00FF00;
	quad.v[1].x=0,quad.v[1].y=0,quad.v[1].z=0.3,quad.v[1].col=0xFF00FF00;
	quad.v[2].x=1000000,quad.v[2].y=1000000,quad.v[2].z=0.3,quad.v[2].col=0xFF00FF00;
	quad.v[3].x=0,quad.v[3].y=2000000,quad.v[3].z=0.3,quad.v[3].col=0xFF00FF00;
	hge->Gfx_RenderQuad(&quad);

	/*quad.v[0].x=0,quad.v[0].y=0,quad.v[0].z=0.4,quad.v[0].col=0;
	quad.v[1].x=-50,quad.v[1].y=0,quad.v[1].z=0.4,quad.v[1].col=0;
	quad.v[2].x=-50,quad.v[2].y=50,quad.v[2].z=0.4,quad.v[2].col=0;
	quad.v[3].x=0,quad.v[3].y=50,quad.v[3].z=0.4,quad.v[3].col=0;
	hge->Gfx_RenderQuad(&quad);*/
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
	
	Geo::Vector o(405.000000 , 305.000000);
	std::vector<Geo::Polygon> polys({ Geo::Polygon({Geo::Vector(428.000000 , 246.000000), Geo::Vector(471.301270 , 271.000000), Geo::Vector(494.301270 , 231.162827), Geo::Vector(451.000000 , 206.162827)})});
	std::cerr << Geo::visibilityPolygon(o, polys, 640, 512) << "\n";
	std::cerr << "===\n";
	
	if (hge->System_Initiate()) {
		Geo::Vector o(405.000000 , 305.000000);
		std::vector<Geo::Polygon> polys({ Geo::Polygon({Geo::Vector(428.000000 , 246.000000), Geo::Vector(471.301270 , 271.000000), Geo::Vector(494.301270 , 231.162827), Geo::Vector(451.000000 , 206.162827)})});
		std::cerr << Geo::visibilityPolygon(o, polys, 640, 512) << "\n";
		std::cerr << sizeof(double);
		exit(0);		
		hge->System_Start();
		
  	}	

  	hge->System_Shutdown();
	hge->Release();

	return 0;
}
