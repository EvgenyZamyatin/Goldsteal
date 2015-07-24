#include "SimpleObstacle.h"

SimpleObstacle::SimpleObstacle(Tmx::Object const* o, hgeResourceManager* res) : IObject(o, res) {
	rData.spr = *(res->GetSprite(o->GetName().c_str()));
	if (o->GetProperties().HasProperty("SpritePoints")) 
		sscanf(o->GetProperties().GetStringProperty("SpritePoints").c_str(), "%d,%d,%d,%d", 
				&rData.point[0], &rData.point[1], &rData.point[2], &rData.point[3]); 
	else { 
		for (int i = 0; i < 4; ++i)rData.point[i] = i;	
	}
}