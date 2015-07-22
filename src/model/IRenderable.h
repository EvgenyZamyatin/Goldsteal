#ifndef IRENDERABLE_H
#define IRENDERABLE_H
#include <hge.h>
#include "Forward.h"                

struct IRenderable {
	virtual ~IRenderable() {}
	virtual void render(HGE* hge, Renderer* renderer, Camera* cam) = 0;
};
#endif