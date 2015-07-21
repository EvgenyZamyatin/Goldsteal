#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "IRenderable.h"
#include <vector>
#include "IObject.h"

struct Environment : IRenderable {
	
	Environment(double width, double hight, RenderFunction render) : width(width), hight(hight), render(render);
	std::vector<IObject*> getObjects() const {return objs;}
	void addObject(IObject* obj) {objs.push_back(obj);}
	double getWidth() const {return width;}
	double getHight() const {return hight;}
	
	friend struct Camera;

private:
	std::vector<IObject*> objs;
	double width;
	double hight;
	std::function<void(HGE* hge, hgeResourceManager* res)> render;
};
#endif