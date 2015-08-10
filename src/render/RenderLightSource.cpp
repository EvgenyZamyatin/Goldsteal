#include "../model/LightSource.h"
#include "../model/Environment.h"
#include "help.h"

void LightSource::render(HGE* hge, Camera* cam) {
    Geo::Box r(bounds);
	Geo::Box c(cam->getPos().x-cam->CAMERA_WIDTH/2, cam->getPos().y-cam->CAMERA_HIGHT/2, cam->CAMERA_WIDTH, cam->CAMERA_HIGHT);
	if (!intersects(r, c))
		return;

	for (int i = 0; i < bounds.size(); ++i) {
		Geo::Vector a = bounds[i], b = bounds[i+1];
		hgeTriple trip;
		hgeU32 col1 = ARGB(50, 255, 238, 173);
		hgeU32 col2 = ARGB(150, 255/2, 238/2, 173/2);

		float x1=pos.x,y1=pos.y,x2=a.x,y2=a.y,x3=b.x,y3=b.y;
		
		cam->convertGS(x1,y1);
		cam->convertGS(x2,y2);
		cam->convertGS(x3,y3);

		trip.tex = 0;
		trip.blend = BLEND_DEFAULT;
		fillTriple(trip, {
						  {x1, y1, 0.f, col1, 0, 0},
						  {x2, y2, 0.f, col1, 1, 0},
						  {x3, y3, 0.f, col1, 1, 1}
						 });
		hge->Gfx_RenderTriple(&trip);
	}
}