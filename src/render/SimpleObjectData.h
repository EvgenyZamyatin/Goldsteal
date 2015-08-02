#ifndef SIMPLEOBJECTDATA_H
#define SIMPLEOBJECTDATA_H
#include <hge.h>
#include "../geometry/Geometry.h"

namespace Render {
	struct SimpleObjectData {
		SimpleObjectData() : spr(0,0,0,0,0) {}
		hgeSprite spr;
		//point in polygon for sprite render.
		int point[4];
	};
}

#endif
