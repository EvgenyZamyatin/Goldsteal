#ifndef IStaticObject_H
#define IStaticObject_H

#include "../Geometry.h"
#include "IRenderable.h"

#include <tmx/Tmx.h.in>
#include <hgeresource.h>

struct GameState;
struct IObject : IRenderable {
	IObject(Tmx::Object const* obj, hgeResourceManager* res);
	IObject() {}
	virtual ~IObject(){}
	virtual void activate() {}
	
	Vector getPosition() const {return pos;}
	Ring getBounds() const {return bounds;}
	
	bool isObstruct() {return obstruct;}
	bool isObstacle() {return obstacle;}
	
	friend struct GameState;
protected:
	bool obstruct;
	bool obstacle;
	Ring bounds;
	Vector pos;
	bool havePoly;
	
	
};

#endif