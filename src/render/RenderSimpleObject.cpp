#include "../model/SimpleObject.h"
#include "RenderData.h"
#include "help.h"
#include "../Geometry.h"

void SimpleObject::render(HGE* hge, Camera const* cam) {
	geo::Box<Vertex> r(bounds);
	geo::Box<Vertex> c(cam->getPos().x-cam->CAMERA_WIDTH/2, cam->getPos().y-cam->CAMERA_HIGHT/2, cam->CAMERA_WIDTH, cam->CAMERA_HIGHT);
	if (!geo::intersects(r, c))
		return;

	for (Render::SimpleObjectTex& data : rData) {
		float x = data.pos.x + bounds[0].x;
		float y = data.pos.y + bounds[0].y;
		cam->convertGS(x, y);
		data.spr.RenderEx(x, y, data.rot, cam->KX*data.width/data.spr.GetWidth(), cam->KY*data.hight/data.spr.GetHeight());
   	}
}
