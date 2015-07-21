#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include "Forward.h"
#include <functional>
#include <hge.h>
#include <hgeresource.h>

typedef std::function<void(HGE* hge, hgeResourceManager* res)> RenderFunction;

struct IRenderable {
	friend struct Camera;
protected:
	RenderFunction render;
	bool clear=false;
};

#endif