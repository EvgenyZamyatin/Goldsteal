#include "IObject.h"

IObject::IObject (Tmx::Object const* o, hgeResourceManager* res) {
	double x = o->GetX();
	double y = o->GetY();
	
	double width = o->GetWidth();
	double hight = o->GetHeight();
	
	this->bounds = Geo::Polygon({{x, y-hight}, {x+width, y-hight}, {x+width, y}, {x, y}});

	double angle = o->GetRot()*M_PI/180;
	this->bounds.rotate(3, angle);
	this->bounds = bounds;
	
	this->pos = bounds.center();
}