#include "../model/IBody.h"
#include "BodyData.h"
#include <hgeanim.h>
#include <hge.h>
#include "help.h"
#include "../model/GameState.h"


void IBody::render(HGE *hge, Camera const* cam) {
	
	for (int i = 0; i < visible.size(); ++i) {
		Geo::Vector a = visible[i], b = visible[i+1];
		hgeTriple trip;
		hgeU32 col = ARGB(50,0,100,0);                      
		
		float sw = state->getEnvironment()->getWidth();
		float sh = state->getEnvironment()->getHight();
		float x1=pos.x,y1=pos.y,x2=a.x,y2=a.y,x3=b.x,y3=b.y;
		
		cam->convertGS(x1,y1);
		cam->convertGS(x2,y2);
		cam->convertGS(x3,y3);

		trip.tex = 0;
		trip.blend = BLEND_DEFAULT;
		fillTriple(trip, {
						  {x1, y1, 0.f, col, 0, 0},
						  {x2, y2, 0.f, col, 1, 0},
						  {x3, y3, 0.f, col, 1, 1}
						 });
		hge->Gfx_RenderTriple(&trip);
	}
	
	Geo::Box r(pos, radius);
	Geo::Box c(cam->getPos().x-cam->CAMERA_WIDTH/2, cam->getPos().y-cam->CAMERA_HIGHT/2, cam->CAMERA_WIDTH, cam->CAMERA_HIGHT);
	if (!intersects(r, c))
		return; 	                        
	hgeAnimation& a = rData.get(moveState);
	
	double dt = hge->Timer_GetDelta();
	
	if (moveState == rData.lastState)
		a.Update(dt);
	else
		a.Play();
	
	float x = pos.x;
	float y = pos.y;
	float sn = dir.x/sqrt((float)dir.len2());
	float cs = -dir.y/sqrt((float)dir.len2());
	cam->convertGS(x, y);
	
	a.RenderEx1(x, y, sn, cs, cam->KX*rData.width/a.GetWidth(), cam->KY*rData.hight/a.GetHeight());
	rData.lastState = moveState;
	
}
