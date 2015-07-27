#include "../model/IBody.h"
#include "BodyData.h"
#include <hgeanim.h>
#include <hge.h>
#include "help.h"
#include "../model/GameState.h"


void IBody::render(HGE *hge, Camera const* cam) {
	
	for (int i = 0; i < visible.size(); ++i) {
		Geo::Vector a = visible[i], b = visible[(i+1)%visible.size()];
		hgeTriple trip;
		hgeU32 col = ARGB(50,0,100,0);                      
		double sw = state->getEnvironment()->getWidth();
		double sh = state->getEnvironment()->getHight();
		double x1=pos.x,y1=pos.y,x2=a.x,y2=a.y,x3=b.x,y3=b.y;
		cam->convertGS(x1,y1);
		cam->convertGS(x2,y2);
		cam->convertGS(x3,y3);

		trip.tex = 0;
		trip.blend = BLEND_DEFAULT;
		fillTriple(trip, {
						  {(float)x1, (float)y1, 0.f, col, 0, 0},
						  {(float)x2, (float)y2, 0.f, col, 1, 0},
						  {(float)x3, (float)y3, 0.f, col, 1, 1}
						 });
		hge->Gfx_RenderTriple(&trip);
	}
	
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
