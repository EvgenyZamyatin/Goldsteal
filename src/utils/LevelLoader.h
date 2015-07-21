#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H
#include <hge.h>
#include <string>
#include <vector>
#include <hgeresource.h>
#include "../model/Environment.h"
#include "../model/GameState.h"

struct LevelLoader {
	bool load(const char* level, const char* resources, GameState* &state, hgeResourceManager* &res);
	
private:
	/*ITileSet* parseTileSet(TiXmlNode* v);
	Environment* parseEnvironment(TiXmlNode* v, const Resources& res);
	IObject* parseObject(TiXmlNode* v, const Resources& res);
	HGE* hge;
	std::vector<ITileSet*> tileSets;*/
};
#endif