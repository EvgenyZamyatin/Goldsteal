#ifndef STUFF_H
#define STUFF_H
#include "../geometry/Geometry.h"
#include "../utils/ITileSet.h"
#include <hge.h>
#include <cassert>
#include "IObject.h"

struct Stuff : IObject {
	Stuff(const Geo::Polygon& poly, ITileSet* tiles, int index, const std::string& type) {
		pos = poly;       
		quad = tiles->get(index);
		this->type = type;
		this->sstatic = true;
		this->clean = false;
	}

	virtual void activate() {}
	virtual Geo::Polygon getPosition(){return pos;}
	virtual Geo::Vector getOrientation() {return Geo::Vector(0,0);}
	virtual void render(HGE* hge) {
		assert(pos.size() == 4);
		for (int i = 0; i < 4; ++i) {   
			quad.v[i].x=pos.points[i].x;
			quad.v[i].y=pos.points[i].y;
			quad.v[i].z=0.5;
		}
		quad.blend=BLEND_DEFAULT_Z;
		hge->Gfx_RenderQuad(&quad);
	}
private:
	hgeQuad quad;
	
};

#endif