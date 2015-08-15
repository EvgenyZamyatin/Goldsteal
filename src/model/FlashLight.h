#ifndef FLASHLIGHT_H
#define FLASHLIGHT_H
#include "LightSource.h"
#include "IBody.h"
#include <hge.h>
#include "Camera.h"
#include "../geometry/Geometry.h"
#include <cmath>

struct Environment;
struct FlashLight : LightSource {	
	FlashLight(IBody* body, Geo::Vector const& shift, float angle) : owner(body) {
		this->shift = shift;
		sn = sinf(angle);
		cs = cosf(angle);
	};
	void frame();
	virtual void setEnvironment(Environment* env);
	virtual void render(HGE* hge, Camera* cam) {};
private:
	IBody* owner;
	Geo::Vector shift;
	float sn;
	float cs;
};
#endif