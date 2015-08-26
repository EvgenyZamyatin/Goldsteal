#include "../model/LightSource.h"
#include "../model/Environment.h"
#include "help.h"

void LightSource::renderLamp(HGE* hge, Camera* cam) {
	geo::Box<Vertex> r(bounds);
	geo::Box<Vertex> c(cam->getPos().x-cam->CAMERA_WIDTH/2, cam->getPos().y-cam->CAMERA_HIGHT/2, cam->CAMERA_WIDTH, cam->CAMERA_HIGHT);
	if (!geo::intersects(r, c))
		return;

	hgeQuad q;
	q.blend = BLEND_DEFAULT;
	q.tex=0;
	hgeU32 col = ARGB(255, 255, 238, 173);
	float x1,y1,x2,y2;
	x1 = pos.x-10;
	y1 = pos.y-5;
	x2 = pos.x+10;
	y2 = pos.y+5;         
	cam->convertGS(x1,y1);
	cam->convertGS(x2,y2);	
	fillQuad(q, {                        
					{x1,y1,0.5f,col,0,0},
					{x2,y1,0.5f,col,0,0},
					{x2,y2,0.5f,col,0,0},
					{x1,y2,0.5f,col,0,0}
				});
	hge->Gfx_RenderQuad(&q);
}

void LightSource::renderLight(HGE* hge, Camera* cam) {
	geo::Box<Vertex> r(bounds);
	geo::Box<Vertex> c(cam->getPos().x-cam->CAMERA_WIDTH/2, cam->getPos().y-cam->CAMERA_HIGHT/2, cam->CAMERA_WIDTH, cam->CAMERA_HIGHT);
	if (!geo::intersects(r, c))
		return;
    hge->Gfx_SetShader(lightShader, SHADER_PIXEL);
	float tmp[2];
	tmp[0]=pos.x;
	tmp[1]=pos.y;
	cam->convertGS(tmp[0], tmp[1]);
	tmp[0]/=800, tmp[1]/=600;
	hge->Shader_SetValue(lightShader, "lightLocation", (void*)tmp, 2*sizeof(float));
	
	for (int i = 0; i < bounds.size() - 1; ++i) {
		Vector a = bounds[i], b = bounds[i+1];
		hgeTriple trip;
		hgeU32 col1 = 0;//ARGB(70, 255, 238, 173);
		float x1=pos.x,y1=pos.y,x2=a.x,y2=a.y,x3=b.x,y3=b.y;
		
		cam->convertGS(x1,y1);
		cam->convertGS(x2,y2);
		cam->convertGS(x3,y3);

		trip.tex = 0;
		trip.blend = BLEND_DEFAULT;
		fillTriple(trip, {
						  {x1, y1, 0.f, col1, x1/cam->SCREEN_WIDTH, y1/cam->SCREEN_HIGHT},
						  {x2, y2, 0.f, col1, x2/cam->SCREEN_WIDTH, y2/cam->SCREEN_HIGHT},
						  {x3, y3, 0.f, col1, x3/cam->SCREEN_WIDTH, y3/cam->SCREEN_HIGHT}
						 });
		hge->Gfx_RenderTriple(&trip);
	}
	hge->Gfx_SetShader(NULL, SHADER_PIXEL);
}

void LightSource::render(HGE* hge, Camera* cam) {
	renderLight(hge, cam);
	renderLamp(hge, cam);
}