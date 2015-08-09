#ifndef IStaticObject_H
#define IStaticObject_H

#include "../geometry/Geometry.h"
#include "IRenderable.h"

#include <tmx/Tmx.h.in>
#include <hgeresource.h>

struct GameState;
struct IObject : IRenderable {
	IObject(Tmx::Object const* obj, hgeResourceManager* res);

	virtual ~IObject(){}
	virtual void activate() {}
	virtual Geo::Vector getPosition() const {return pos;}
	virtual Geo::Polygon getBounds() const {return bounds;}
	virtual bool isObstruct() {return true;}
	virtual bool isObstacle() {return true;}
	//void setPos(Geo::Vector const& p) {this->pos=p;}
	//void setBounds(Geo::Polygon const& p) {this->bounds=p;}

	friend struct GameState;
protected:
	Geo::Polygon bounds;
	Geo::Vector pos;
	bool havePoly;
	int radius2;
	Geo::Vector center;
};

#endif