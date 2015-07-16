#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H
#include <hge.h>
#include <tinyxml.h>
#include <string>
#include <vector>
#include "../model/GameState.h"
#include "Resources.h"

struct LevelLoader {
	LevelLoader(HGE* hge) : hge(hge) {};
	GameState* load(std::string file, Resources& res);
	
private:
	ITileSet* parseTileSet(TiXmlNode* v);
	Environment* parseEnvironment(TiXmlNode* v, const Resources& tsets);
	HGE* hge;
	std::vector<ITileSet*> tileSets;
};
#endif