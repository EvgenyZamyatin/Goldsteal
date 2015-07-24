#include "../model/IBody.h"
#include "BodyData.h"
#include <hgeanim.h>
#include <hge.h>
#include "help.h"

/*
void IBody::render(HGE *hge, Camera const* cam) {
	
	Rect r = boundingBox(bounds);
	Rect c(cam);
	//check intersect with vis poly/
	if (!intersect(r, c))
		return; 	                        
	
	double kx = cam->sWidth()/cam->cWidth();
	double ky = cam->sHight()/cam->cHight();

	hgeAnimation& leg = rData.getLeg(legState);
	hgeAnimation& bd = rData.getBody(bodyState);
	double dt = hge->Timer_GetDelta();
	
	if (legState == rData.lastLegState)
		leg.Update(dt);
	else
		leg.Play();
	
	if (bodyState == rData.lastBodyState)
		bd.Update(dt);
	else
		bd.Play();                         

	leg.Render4V((bounds[0].x-c.x)*kx, (bounds[0].y-c.y)*ky,
					(bounds[1].x-c.x)*kx, (bounds[1].y-c.y)*ky, 
					(bounds[2].x-c.x)*kx, (bounds[2].y-c.y)*ky,
					(bounds[3].x-c.x)*kx, (bounds[3].y-c.y)*ky);
	bd.Render4V((bounds[0].x-c.x)*kx, (bounds[0].y-c.y)*ky,
					(bounds[1].x-c.x)*kx, (bounds[1].y-c.y)*ky, 
					(bounds[2].x-c.x)*kx, (bounds[2].y-c.y)*ky,
					(bounds[3].x-c.x)*kx, (bounds[3].y-c.y)*ky);
	rData.lastLegState = legState;
	rData.lastBodyState = bodyState;
}
*/