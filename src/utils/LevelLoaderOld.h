#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H
#include <hge.h>
#include <string>
#include <vector>
#include <hgeresource.h>     
#include <tmx/LayerVisitor.h>
#include <tmx/TileLayer.h>
#include <tmx/ObjectLayer.h>
#include <tmx/ImageLayer.h>
#include "../model/Environment.h"
#include "../model/GameState.h"
#include "../model/Renderer.h"
#include "../model/SimpleObstacle.h"

struct LevelLoader {
	bool load(HGE* hge, const char* level, const char* resources, GameState* &state, hgeResourceManager* &res, Renderer* &rend);
	
	struct MyVisitor : tmx::LayerVisitor {
		MyVisitor(HGE* hge, hgeResourceManager* res, GameState* state) : hge(hge), res(res), state(state) {}
		virtual ~MyVisitor() {}
		virtual void visitTileLayer(const tmx::Map& map, const tmx::TileLayer& layer) override;
  		virtual void visitObjectLayer(const tmx::Map& map, const tmx::ObjectLayer& layer) override;
  		virtual void visitImageLayer(const tmx::Map& map, const tmx::ImageLayer& layer) override;
  		GameState* state;
    	hgeResourceManager* res;
    	HGE* hge;
    	Renderer::EnvironmentRenderData envData;
	};
};
#endif