#ifndef CAMERA_H
#define CAMERA_H
#include "Forward.h"
#include "IObject.h"
#include "../geometry/Geometry.h"
#include "GameState.h"
#include "IBody.h"
#include <hge.h>
#include <hgeresource.h>
#include <vector>


struct Camera {
	Camera(GameState* state, Geo::Vector pos, 
			double cameraWidth, double cameraHight, double screenWidth, double screenHight,
			double radius);

	//calls after gamestate frame;
	void view(HGE* hge, hgeResourceManager* res);
	void frame(Geo::Vector const& mousePos, bool freeMode);
	
	void setGameState(GameState* state) {this->state=state;}
	void bind(IBody* body) {this->body=body;}
	void move(Geo::Vector const& v) {pos += v;}
	friend struct Renderer;

private:
	IBody* body=NULL;
	Geo::Vector pos;
	GameState* state;
	double radius;

	double cameraWidth;
	double cameraHight;
	double screenWidth;
	double screenHight;
};
#endif