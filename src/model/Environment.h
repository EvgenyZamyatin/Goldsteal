#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Forward.h"
#include "IRenderable.h"
#include <vector>
#include "IObject.h"
#include "../render/EnvironmentData.h"

struct LevelLoader;

struct Environment : IRenderable {
	Environment() {};
	Environment(double width, double hight) : width(width), hight(hight) {}
	std::vector<IObject*> getObjects() const {return objs;}
	void addObject(IObject* obj) {objs.push_back(obj);}
	double getWidth() const {return width;}
	double getHight() const {return hight;}
	void render(HGE* hge, Camera const* cam);
	
	friend struct LevelLoader;
	friend struct Camera;

private:
	std::vector<IObject*> objs;
	double width;
	double hight;
	Render::EnvironmentData rData;
};
#endif