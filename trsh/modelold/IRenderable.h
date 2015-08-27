#ifndef IRENDERABLE_H
#define IRENDERABLE_H
#include <hge.h>
#include "Camera.h"                

struct IRenderable {
	virtual ~IRenderable() {}
	virtual void render(HGE* hge, Camera const* cam) {};
};
#endif