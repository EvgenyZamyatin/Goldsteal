#include "IObject.h"

IObject::IObject (Tmx::Object const* o, hgeResourceManager* res) {
	int x = o->GetX();
	int y = o->GetY();
	pos = Vector(x, y);
	Tmx::Polygon const* p = o->GetPolygon();
	if (p != NULL) {
    	Ring ring;
    	for (int i = 0; i < p->GetNumPoints(); ++i) {
    		ring.push_back(Vector((int)(p->GetPoint(i).x), (int)(p->GetPoint(i).y)) + pos);
    	}
    	bounds = ring;
    	geo::correct(bounds);
    	havePoly = true;
    	
    } else {
    	havePoly = false;
    }
}