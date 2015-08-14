#ifndef SIMPLEOBSTACLE_H
#define SIMPLEOBSTACLE_H
#include <hge.h>
#include "IObject.h"
#include "Camera.h"
#include "../render/RenderData.h"

struct LevelLoader;
struct SimpleObject : IObject {
	SimpleObject(Tmx::Object const* obj, Tmx::ObjectGroup const* textureLayer, hgeResourceManager* res);
    
	virtual void render(HGE* hge, Camera const* can);

private:	
	Render::SimpleObjectData rData;
};
#endif