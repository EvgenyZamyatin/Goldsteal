#include "../model/SimpleObject.h"
#include "SimpleObjectData.h"
#include "help.h"
#include "../geometry/Geometry.h"

void SimpleObject::render(HGE* hge, Camera const* cam) {
	Geo::Box r(bounds);
	Geo::Box c(cam->getPos().x-cam->CAMERA_WIDTH/2, cam->getPos().y-cam->CAMERA_HIGHT/2, cam->CAMERA_WIDTH, cam->CAMERA_HIGHT);
	if (!intersects(r, c))
		return; 	                        
	static double pt[4][2];
	for (int i = 0; i < 4; ++i) {
		pt[i][0] = bounds[rData.point[i]].x;
		pt[i][1] = bounds[rData.point[i]].y;
		cam->convertGS(pt[i][0], pt[i][1]);
	}
	rData.spr.Render4V(pt[0][0], pt[0][1],
				pt[1][0], pt[1][1],
				pt[2][0], pt[2][1],
				pt[3][0], pt[3][1]);
}
