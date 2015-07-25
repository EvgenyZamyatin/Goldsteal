#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include <tmx/TmxMap.h>
#include <hgeresource.h>
#include <tmx/Tmx.h.in>
                        
#include "IRenderable.h"
#include "IObject.h"
#include "../render/EnvironmentData.h"


struct GameState;
struct Environment : IRenderable {
	Environment() {};
	Environment(Tmx::Map const* map, hgeResourceManager* res);
	
	std::vector<IObject*> getObjects() const {return objs;}
	void addObject(IObject* obj) {objs.push_back(obj);}
	double getWidth() const {return width;}
	double getHight() const {return hight;}
	void render(HGE* hge, Camera const* cam);
	friend struct GameState;
private:
	std::vector<IObject*> objs;
	double width;
	double hight;
	Render::EnvironmentData rData;
};
#endif