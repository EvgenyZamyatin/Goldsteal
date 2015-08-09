#include "IObject.h"

IObject::IObject (Tmx::Object const* o, hgeResourceManager* res) {
	int x = o->GetX();
	int y = o->GetY();
	pos = Geo::Vector(x, y);
	radius2 = 0;
	Tmx::Polygon const* p = o->GetPolygon();
	if (p != NULL) {
    	std::vector<Geo::Vector> poly;
    	for (int i = 0; i < p->GetNumPoints(); ++i) {
    		poly.push_back(Geo::Vector((int)(p->GetPoint(i).x), (int)(p->GetPoint(i).y)) + pos);
    	}
    	bounds = Geo::Polygon(poly);
    	havePoly = true;
    	center = bounds.center();    	
    	for (int i = 0; i < bounds.size(); ++i) {
    		radius2 = std::max(radius2, (bounds[i] - center).len2());
    	}
    	bounds.makeCCW();
    } else {
    	havePoly = false;
    	center = pos;
    }
}