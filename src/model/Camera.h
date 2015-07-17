#ifndef CAMERA_H
#define CAMERA_H
#include "IObject.h"
#include "../geometry/Geometry.h"
#include "GameState.h"
#include "IBody.h"

struct Camera {
	Camera(GameState* state, IBody* body, const Geo::Vector& pos, double width, double hight);
	void view(HGE* hge);
	void bind(IObject* obj);
	void move(const Geo::Vector& v);
	void setFreeMode(bool freeMode);
	void frame(HGE* hge);
private:
	void make(hgeQuad& quad, Geo::Vector c, double width, double hight, double col, double z);
	bool freeMode=true;
	Geo::Vector getPos();
	IBody* body=NULL;
	Geo::Vector pos;
	GameState* state;
	double cameraWidth;
	double cameraHight;
};
#endif