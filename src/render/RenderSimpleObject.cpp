#include "../model/SimpleObject.h"
#include "SimpleObjectData.h"
#include "help.h"
#include "../geometry/Geometry.h"

void SimpleObject::render(HGE* hge, Camera const* cam) {
	Geo::Box r(bounds);
	Geo::Box c(cam->getPos().x-cam->CAMERA_WIDTH/2, cam->getPos().y-cam->CAMERA_HIGHT/2, cam->CAMERA_WIDTH, cam->CAMERA_HIGHT);
	if (!intersects(r, c))
		return;

	for (Render::Data& data : rData) {
		float x = data.pos.x + pos.x;
		float y = data.pos.y + pos.y;
		cam->convertGS(x, y);
		//std::cerr << x << " " << y << "\n" << cam->KX*data.width/data.spr.GetWidth() << " " << cam->KY*data.hight/data.spr.GetHeight() << "\n";
		data.spr.RenderEx(x, y, data.rot, cam->KX*data.width/data.spr.GetWidth(), cam->KY*data.hight/data.spr.GetHeight());
   	}
}
