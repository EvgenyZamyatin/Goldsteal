#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H
#include "IObject.h"
#include "Camera.h"
#include <hge.h>
#include <hgeresource.h>
struct Environment;
struct LightSource : IObject {
	LightSource() {}
	LightSource(Tmx::Object const* o, hgeResourceManager* res) : IObject(o, res) {}
	virtual void setEnvironment(Environment* env);
	virtual void render(HGE* hge, Camera* cam);
	friend struct Environment;
protected:
	Environment* env;
};

#endif