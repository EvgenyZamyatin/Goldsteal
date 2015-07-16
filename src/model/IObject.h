#ifndef IStaticObject_H
#define IStaticObject_H
#include "IRenderable.h"
#include "../geometry/Geometry.h"

struct IObject : IRenderable {
	virtual ~IObject(){}
	virtual void activate() {}
	virtual Geo::Polygon getPosition() {return pos;}
	virtual Geo::Vector getOrientation() {return orientation;}
	bool isStatic() const {return sstatic;}
	std::string getType() const {return type;}
	static IObject* construct(){}

protected:
	Geo::Polygon pos;
	Geo::Vector orientation;
	std::string type;
	bool sstatic=false;
};

#endif