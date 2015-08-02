#include <hge.h>             
#include <hgedistort.h>
#include <cstdlib>
#include <iostream>

                 
#define WIDTH 800
#define HIGHT 600
#define TEXW 512
#define TEXH 512
             
HGE *hge = 0;
HTEXTURE perlin;
hgeQuad quad;
hgeQuad quad1;
hgeDistortionMesh*	dis;


float dx[] = {0.f, 1.f, 1.f, 0.f};
float dy[] = {0.f, 0.f, 1.f, 1.f};

bool FrameFunc() {
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
	return false;
}          

		
bool RenderFunc() {
	
	hge->Gfx_BeginScene();
    hge->Gfx_Clear(0);
    
    for (int i = 0; i < 4; ++i) {
		quad.v[i].x = (WIDTH-TEXW)/2 + dx[i]*TEXW;
		quad.v[i].y = (HIGHT-TEXH)/2 + dy[i]*TEXH;
		quad.v[i].col = ARGB(100,255,255,255);
		quad.v[i].tx = dx[i];
		quad.v[i].ty = dy[i];

    }       
    quad.blend = BLEND_DEFAULT;
    quad.tex = perlin;       
    hge->Gfx_RenderQuad(&quad);
    
    quad.tex = 0;
    quad.blend = BLEND_ALPHAADD;
    for (int i = 0; i < 4; ++i) {
		quad.v[i].x = (WIDTH-TEXW)/2 + dx[i]*TEXW/2;
		quad.v[i].y = (HIGHT-TEXH)/2 + dy[i]*TEXH;
		quad.v[i].col = ARGB(155,0,0,0);
		quad.v[i].tx = dx[i];
		quad.v[i].ty = dy[i];

    }       
    hge->Gfx_RenderQuad(&quad);
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
	hge->System_SetState(HGE_FPS, 30);
	hge->System_SetState(HGE_HIDEMOUSE, false);
    
   	if (hge->System_Initiate()) {
        //std::cerr << perlin;
    	perlin = hge->Texture_Load("tutorial.bmp");
        hge->System_Start();
  	}	

  	hge->System_Shutdown();
	hge->Release();
	return 0; 
}

 
