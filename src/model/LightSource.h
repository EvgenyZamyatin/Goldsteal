#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H
#include "IObject.h"
#include <hgeresource.h>
struct Environment;
struct LightSource : IObject {
	LightSource(Tmx::Object const* o, hgeResourceManager* res);
	void frame();
	void setEnvironment(Environment* env) {this->env = env;}
	friend struct Environment;
private:
	Environment* env;
};

#endif