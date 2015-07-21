#ifndef IStaticObject_H
#define IStaticObject_H

#include "Forward.h"
#include "../geometry/Geometry.h"
#include "IRenderable.h"

struct IObject : IRenderable {
	virtual ~IObject(){}
	virtual void activate() {}
	virtual Geo::Vector getPosition() const {return pos;}
	virtual Geo::Polygon getBounds() const {return bounds;}
	virtual Geo::Vector getOrientation() const {return dir;}
	bool isObstruct() const {return obstruct;}
	int getType() const {return type;}

protected:
	int type;
	bool obstruct;
	Geo::Polygon bounds;
	Geo::Vector pos;
	Geo::Vector dir;
};

#endif