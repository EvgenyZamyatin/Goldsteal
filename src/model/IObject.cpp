#include "IObject.h"

IObject::IObject (Tmx::Object const* o, hgeResourceManager* res) {
	int x = o->GetX();
	int y = o->GetY();
	pos = Geo::Vector(x, y);
	Tmx::Polygon const* p = o->GetPolygon();
	if (p != NULL) {
    	std::vector<Geo::Vector> poly;
    	for (int i = 0; i < p->GetNumPoints(); ++i) {
    		poly.push_back(Geo::Vector((int)(p->GetPoint(i).x), (int)(p->GetPoint(i).y)) + pos);
    	}
    	bounds = Geo::Polygon(poly);
    	havePoly = true;
    } else {
    	havePoly = false;
    }

}