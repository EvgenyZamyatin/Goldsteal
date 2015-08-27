#include "../IObject.h"

IObject::IObject (Tmx::Object const* o, hgeResourceManager* res) {
	int x = o->GetX();
	int y = o->GetY();
	
	Tmx::Polygon const* p = o->GetPolygon();
	Vector pos = Vector(x, y);
	if (p != NULL) {    	
    	for (int i = 0; i < p->GetNumPoints(); ++i) {
    		bounds.push_back(Vector((int)(p->GetPoint(i).x), (int)(p->GetPoint(i).y)) + pos);
    	}
    	geo::correct(bounds);
    	havePoly = true;
    } else {
    	bounds.push_back(pos);
    	bounds.push_back(pos);
    	havePoly = false;
    }

    for (Vertex& v : bounds)
    	v.owner = this;
}