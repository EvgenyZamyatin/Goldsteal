#include "LevelLoader.h"
#include "../model/GameState.h"
#include "../model/Environment.h"
#include "../geometry/Geometry.h"
#include "../model/SimpleObstacle.h"

#include "ITileSet.h"
#include <iostream>
#include <hgeresource.h>
#include <stdlib.h>
#include <hge.h>
#include <tmx/Map.h>         
#include <cassert>

void LevelLoader::MyVisitor::visitTileLayer(const tmx::Map& map, const tmx::TileLayer& layer) {
	assert(layer.hasProperty("tileSet"));
	std::string tileSetName = layer.getProperty("tileSet", "");
	HTEXTURE tileSetTex = res->GetTexture(tileSetName.c_str());
	const tmx::TileSet *ts = NULL;
	for (auto tsit : map.getTileSets()) {
		if (tsit->getName() == tileSetName) {
			ts = tsit;
			break;
		}
	}
	assert(ts != NULL);
	std::vector<std::pair<int, int>> tasks;
	for (auto cell = layer.begin(); cell != layer.end(); ++cell) {
		int id = (int)cell->getGID() - (int)ts->getFirstGID();
		if (id < 0) {
			tasks.push_back(std::make_pair(-1, -1));
			continue;
		}
		tmx::Rect r = ts->getCoords(id, {ts->getImage()->getWidth(), ts->getImage()->getHeight()});
		tasks.push_back(std::make_pair(r.x, r.y));
	}
	Renderer::EnvironmentRenderData::Layer l;
	l.tex=tileSetTex;                         
	l.texWidth = hge->Texture_GetWidth(l.tex);
	l.texHight = hge->Texture_GetHeight(l.tex);
	l.alpha = (int)std::floor(layer.getOpacity()*255);
	l.tcorners = tasks;
	envData.layers.push_back(l);
}

IObject* constructSimpleObstacle(tmx::TileObject* obj) {
	//SimpleObstacle *res = new SimpleObstacle();
	//std::cerr << obj->getWidth();
	std::cerr << obj->getRotation() << " " << obj->getY() << "\n";
	return NULL;
}

void LevelLoader::MyVisitor::visitObjectLayer(const tmx::Map& map, const tmx::ObjectLayer& layer) {
	for (auto obj = layer.begin(); obj != layer.end(); ++obj) {
		if ((*obj)->getType() == std::string("SimpleObstacle")) {
			state->env->addObject(constructSimpleObstacle((tmx::TileObject*)(*obj)));
		}
	}
}

void LevelLoader::MyVisitor::visitImageLayer(const tmx::Map& map, const tmx::ImageLayer& layer) {
	std::cerr << "ImageLayer" << "\n";
}

bool LevelLoader::load(HGE* hge, const char* file, const char* resources, GameState* &state, hgeResourceManager* &res, Renderer* &renderer) {
	res = new hgeResourceManager(resources);
	std::unique_ptr<tmx::Map> map = tmx::Map::parseFile(file);
	if (map == NULL)
		return false;
	
	state = new GameState(new Environment());
	MyVisitor vis(hge, res, state);
	
	vis.envData.tilesInRow=map->getWidth();
	vis.envData.tilesInColumn=map->getHeight();   
	vis.envData.texTileWidth=map->getTileWidth();
	vis.envData.texTileHight=map->getTileHeight();        
	
	vis.state->env->width=map->getTileWidth() * map->getWidth();
	vis.state->env->hight=map->getTileHeight() * map->getHeight();
	       
	vis.envData.envTileWidth=vis.state->env->width/map->getWidth();
	vis.envData.envTileHight=vis.state->env->hight/map->getHeight();
	
	map->visitLayers(vis);

	renderer = new Renderer();
	renderer->envData=vis.envData;
	return true;
}
