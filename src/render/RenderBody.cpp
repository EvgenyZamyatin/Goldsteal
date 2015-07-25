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
	static double pt[4][2];
	for (int i = 0; i < 4; ++i) {
		pt[i][0] = bounds[i].x;
		pt[i][1] = bounds[i].y;
		cam->convertGS(pt[i][0], pt[i][1]);
	}

	a.Render4V(pt[0][0], pt[0][1],
				pt[1][0], pt[1][1],
				pt[2][0], pt[2][1],
				pt[3][0], pt[3][1]);
	rData.lastState = moveState;
}
