#ifndef IOBJECT_H
#define IOBJECT_H
#include <hgeresource.h>
#include <Tmx/Tmx.h.in>
#include "../Geometry.h"
#include "Vertex.h"
#include "IRenderable.h"

struct IObject : IRenderable {
	IObject() {};
	IObject(Tmx::Object const* obj, hgeResourceManager* res);
	virtual ~IObject() {};
	
	bool isObstruct() {return obstruct;}
	bool isObstacle() {return obstacle;}
	
	geo::Ring<Vertex> getBounds() const {return bounds;}
	
protected:
	bool obstruct;
	bool obstacle;
	geo::Ring<Vertex> bounds;
	bool havePoly;
	
};

#endif
