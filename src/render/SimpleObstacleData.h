#ifndef SIMPLEOBSTACLEDATA_H
#define SIMPLEOBSTACLEDATA_H
#include <hge.h>
#include "../geometry/Geometry.h"

namespace Render {
	struct SimpleObstacleData {
		SimpleObstacleData() : spr(0,0,0,0,0) {}
		hgeSprite spr;
		//point in polygon for sprite render.
		int point[4];
	};
}

#endif
