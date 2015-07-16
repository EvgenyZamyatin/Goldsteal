#ifndef RENDERABLE_H
#define RENDERABLE_H
#include <hge.h>

struct IRenderable {
	virtual ~IRenderable() {};
	virtual void render(HGE* hge) = 0;
};


#endif