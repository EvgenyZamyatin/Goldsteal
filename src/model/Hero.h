#ifndef HERO_H
#define HERO_H
#include <hge.h>
#include "../geometry/Geometry.h"
#include "IBody.h"

struct LevelLoader;
struct Hero : IBody {
	static const int INPUT_UP = 1;
	static const int INPUT_DOWN = 2;
	static const int INPUT_RIGHT = 4;
	static const int INPUT_LEFT = 8;
	
	friend struct LevelLoader;
	void frame(int event, Geo::Vector mouse);
};
#endif