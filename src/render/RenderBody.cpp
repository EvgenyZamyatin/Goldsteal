#include "../model/IBody.h"
#include "BodyData.h"
#include <hgeanim.h>
#include <hge.h>
#include "help.h"


void IBody::render(HGE *hge, Camera const* cam) {
	
	Rect r = boundingBox(bounds);
	Rect c(cam);
	if (!intersect(r, c))
		return; 	                        
	
	double kx = cam->sWidth()/cam->cWidth();
	double ky = cam->sHight()/cam->cHight();

	hgeAnimation& a = rData.get(moveState);
	
	double dt = hge->Timer_GetDelta();
	if (moveState == rData.lastState)
		a.Update(dt);
	else
		a.Play();
	
	if (moveState == rData.lastState)
		a.Update(dt);
	else
		a.Play();                         

	a.Render4V((bounds[0].x-c.x)*kx, (bounds[0].y-c.y)*ky,
					(bounds[1].x-c.x)*kx, (bounds[1].y-c.y)*ky, 
					(bounds[2].x-c.x)*kx, (bounds[2].y-c.y)*ky,
					(bounds[3].x-c.x)*kx, (bounds[3].y-c.y)*ky);
	rData.lastState = moveState;
}
