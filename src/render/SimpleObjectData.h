#ifndef SIMPLEOBJECTDATA_H
#define SIMPLEOBJECTDATA_H
#include <hge.h>
#include <vector>
#include "../geometry/Geometry.h"

namespace Render {
	struct Data {
		Data(hgeSprite const& s, Geo::Vector const& v, 
			float rot, int width, int hight) : spr(s), pos(v), rot(rot), width(width), hight(hight) {}

		hgeSprite spr;
		Geo::Vector pos;//relative
		float rot;
		int width;
		int hight;
	};
	typedef std::vector<Data> SimpleObjectData;
	
}

#endif
