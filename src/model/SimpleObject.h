#ifndef SIMPLEOBSTACLE_H
#define SIMPLEOBSTACLE_H
#include <hge.h>
#include "IObject.h"
#include "Camera.h"
#include "../render/SimpleObjectData.h"

struct LevelLoader;
struct SimpleObject : IObject {
	SimpleObject(Tmx::Object const* obj, hgeResourceManager* res);

	virtual bool isObstruct() const {return obstruct;}
	virtual bool isObstacle() const {return obstacle;}
	
	virtual void render(HGE* hge, Camera const* can);

private:	
	bool obstruct;
	bool obstacle;
	Render::SimpleObjectData rData;
};
#endif