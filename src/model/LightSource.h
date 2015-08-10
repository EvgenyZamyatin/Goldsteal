#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H
#include "IObject.h"
#include "Camera.h"
#include <hge.h>
#include <hgeresource.h>
struct Environment;
struct LightSource : IObject {
	LightSource(Tmx::Object const* o, hgeResourceManager* res);
	void frame();
	void setEnvironment(Environment* env) {this->env = env;}
	virtual void render(HGE* hge, Camera* cam);
	friend struct Environment;
private:
	Environment* env;
};

#endif