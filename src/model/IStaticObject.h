#ifndef IStaticObject_H
#define IStaticObject_H
#include "IReal.h"
#include "IRenderable.h"

struct IStaticObject : IReal, IRenderable {
	virtual ~IStaticObject(){};
	virtual void activate() = 0;
	virtual void frame() = 0;
	bool clean=false;                
};

#endif