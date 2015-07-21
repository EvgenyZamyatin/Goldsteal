#ifndef IStaticObject_H
#define IStaticObject_H
#include "../geometry/Geometry.h"

struct IObject : IRenderable{
	virtual ~IObject(){}
	virtual void activate() {}
	virtual Geo::Vector getPosition() const {return pos;}
	virtual Geo::Polygon getBounds() const {return bounds;}
	virtual Geo::Vector getOrientation() const {return dir;}
	bool isObstruct() const {return obstruct;}
	int getType() const {return type;}

private:
	int type;
	bool obstuct;
	Geo::Polygon bounds;
	Geo::Vector pos;
	Geo::Vector dir;
};

#endif