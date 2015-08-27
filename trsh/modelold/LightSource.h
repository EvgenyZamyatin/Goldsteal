#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H
#include "IObject.h"
#include "Camera.h"
#include <hge.h>
#include <hgeresource.h>
struct Environment;
struct LightSource : IObject {
	LightSource(hgeResourceManager* res) {
		lightShader=res->GetShader("LightShader");
	}	
	LightSource(Tmx::Object const* o, hgeResourceManager* res) : IObject(o, res) {
		lightShader=res->GetShader("LightShader");
	}
	virtual void setEnvironment(Environment* env);
	virtual void render(HGE* hge, Camera* cam);
	virtual void renderLight(HGE* hge, Camera* cam);
	virtual void renderLamp(HGE* hge, Camera* cam);
	virtual void frame();
	bool isEnable() const {return enabled;}

	friend struct Environment;
protected:
	bool enabled=true;
	Environment* env;
	HSHADER lightShader;
};

#endif