#ifndef IRENDERABLE_H
#define IRENDERABLE_H
#include <hge.h>
#include "Forward.h"                

struct IRenderable {
	virtual ~IRenderable() {}
	virtual void render(HGE* hge, Camera const* cam) = 0;
};
#endif