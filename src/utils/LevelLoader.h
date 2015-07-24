#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H
#include <hge.h>
#include <string>
#include <vector>
#include <hgeresource.h>     
#include "../model/Environment.h"
#include "../model/GameState.h"
#include "../model/SimpleObstacle.h"
#include "tmx/Tmx.h.in"
#include "../model/Hero.h"

struct LevelLoader {
	bool load(HGE* hge, const char* level, const char* resources, GameState* &state, hgeResourceManager* &res);
	SimpleObstacle* constructSimpleObstacle(Tmx::Object* o, hgeResourceManager* res);
	Hero* constructHero(Tmx::Object* o, hgeResourceManager* res);
};
#endif