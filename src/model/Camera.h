#ifndef CAMERA_H
#define CAMERA_H
#include "IObject.h"
#include "../geometry/Geometry.h"
#include "GameState.h"
#include "IBody.h"
#include <hge.h>
#include <vector>

struct Camera {
	Camera(GameState* state, IBody* body, const Geo::Vector& pos, double width, double hight, double screenWidth, double screenHight);
	void view(HGE* hge);
	void bind(IObject* obj);
	void move(const Geo::Vector& v);
	void setFreeMode(bool freeMode);
	void frame(HGE* hge);
	bool isFree() {return freeMode;}
private:
	void fill(hgeQuad& quad, Geo::Vector a, Geo::Vector b, Geo::Vector c, Geo::Vector d);
	void fill(hgeTriple& trip, Geo::Vector a, Geo::Vector b, Geo::Vector c);

	void make(hgeQuad& quad, Geo::Vector c, double width, double hight, double col, double z);
	bool freeMode=false;
	Geo::Vector getPos();
	IBody* body=NULL;
	Geo::Vector pos;
	GameState* state;
	double cameraWidth;
	double cameraHight;
	double screenWidth;
	double screenHight;
	HTARGET target=0;	
};
#endif