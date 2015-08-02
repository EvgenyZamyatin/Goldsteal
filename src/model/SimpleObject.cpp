#include "SimpleObject.h"

SimpleObject::SimpleObject(Tmx::Object const* o, hgeResourceManager* res) : IObject(o, res) {
	hgeSprite* spr = res->GetSprite(o->GetName().c_str());
	if (spr == 0) {
		std::cerr <<"Can't find " << o->GetName() << "\n";
		exit(0);
	}
	rData.spr = *(spr);
	
	if (o->GetProperties().HasProperty("SpritePoints")) 
		sscanf(o->GetProperties().GetStringProperty("SpritePoints").c_str(), "%d,%d,%d,%d", 
				&rData.point[0], &rData.point[1], &rData.point[2], &rData.point[3]); 
	else { 
		for (int i = 0; i < 4; ++i)rData.point[i] = i;	
	}
	if (o->GetProperties().HasProperty("IsObstacle"))
		obstacle = o->GetProperties().GetIntProperty("IsObstacle");
	else 
		obstacle = true;

	if (o->GetProperties().HasProperty("IsObstruct"))
		obstruct = o->GetProperties().GetIntProperty("IsObstruct");
	else 
		obstruct = true;

}
