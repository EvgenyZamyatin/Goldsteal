#include "../model/SimpleObstacle.h"
#include "SimpleObstacleData.h"
#include "help.h"
#include "../geometry/Geometry.h"

void SimpleObstacle::render(HGE* hge, Camera const* cam) {
	Rect r = boundingBox(bounds);
	Rect c(cam);
	if (!intersect(r, c))
		return; 	                        
	double kx = cam->sWidth()/cam->cWidth();
	double ky = cam->sHight()/cam->cHight();
	//std::cerr << bounds[rData.pt1] << " " << bounds[rData.pt2] << "\n";
	rData.spr.Render4V((bounds[rData.point[0]].x-c.x)*kx, (bounds[rData.point[0]].y-c.y)*ky, 
						(bounds[rData.point[1]].x-c.x)*kx, (bounds[rData.point[1]].y-c.y)*ky, 
						(bounds[rData.point[2]].x-c.x)*kx, (bounds[rData.point[2]].y-c.y)*ky, 
						(bounds[rData.point[3]].x-c.x)*kx, (bounds[rData.point[3]].y-c.y)*ky); 
	//rData.spr.Render((bounds[rData.pt1].x-c.x)*kx, (bounds[rData.pt1].y-c.y)*ky); 
}
