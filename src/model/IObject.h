#ifndef IStaticObject_H
#define IStaticObject_H

#include "Forward.h"
#include "../geometry/Geometry.h"
#include "IRenderable.h"

struct LevelLoader;

struct IObject : IRenderable {
	virtual ~IObject(){}
	virtual void activate() {}
	virtual Geo::Vector getPosition() const {return pos;}
	virtual Geo::Polygon getBounds() const {return bounds;}
	virtual Geo::Vector getOrientation() const {return dir;}
	virtual bool isObstruct() {return true;}
	
	void setPos(Geo::Vector const& p) {this->pos=p;}
	void setBounds(Geo::Polygon const& p) {this->bounds=p;}
	void setDir(Geo::Vector const& p) {this->dir=p;}
	
	
protected:
	Geo::Polygon bounds;
	Geo::Vector pos;
	Geo::Vector dir;
};

#endif