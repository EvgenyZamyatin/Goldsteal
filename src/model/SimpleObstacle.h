#ifndef SIMPLEOBSTACLE_H
#define SIMPLEOBSTACLE_H
#include "IObject.h"
#include "Forward.h"
#include "Camera.h"
#include "../render/SimpleObstacleData.h"

struct LevelLoader;
struct SimpleObstacle : IObject {
	friend struct LevelLoader;	
	friend struct Camera;
	
	SimpleObstacle() {}
		
	virtual bool isObstruct() const {
		return true;
	}
	virtual void render(HGE* hge, Camera const* can);

private:	
	Render::SimpleObstacleData rData;
};
#endif