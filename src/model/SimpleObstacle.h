#ifndef SIMPLEOBSTACLE_H
#define SIMPLEOBSTACLE_H
#include <hge.h>
#include "IObject.h"
#include "Camera.h"
#include "../render/SimpleObstacleData.h"

struct LevelLoader;
struct SimpleObstacle : IObject {
	SimpleObstacle(Tmx::Object const* obj, hgeResourceManager* res);

	virtual bool isObstruct() const {return true;}
	
	virtual void render(HGE* hge, Camera const* can);

private:	
	Render::SimpleObstacleData rData;
};
#endif