#ifndef IBODY_H
#define IBODY_H

#include <tmx/Tmx.h.in>
#include <hgeresource.h>
#include <functional>

#include "IObject.h"
#include "Environment.h"
#include "../geometry/Geometry.h"
#include "../render/BodyData.h"
#include "brains/IBrain.h"
#include "Camera.h"


struct GameState;
struct BrainPlayerInput;

struct IBody : IRenderable {                
	static const int STATE_IDLE = 0;
	static const int STATE_WALK = 1;
	
	static const int MAX_SPEED;
	static const int ACCELERATION;

	IBody(Tmx::Object const* obj, hgeResourceManager* res);
	virtual ~IBody(){};                      
	virtual void render(HGE* hge, Camera const* cam);
    
	void setGameState(GameState* state) {this->state=state;}
	
	Geo::Vector getDir() const {return dir;}
	Geo::Vector getPosition() const {return pos;}

	void frame();
    int getRadius() {return radius;}
    int getRadius2() {return radius2;}

    //virtual void onCollision(IObject* obj) {}
	//virtual void onCollision(IBody* body) {}

	friend struct GameState;
	friend struct BrainPlayerInput;

private:
	Render::BodyData rData;

	IBrain* brain;
	int moveState=0;
	Geo::Vector pos;
	Geo::Vector velocity;
	Geo::Polygon visible;
	Geo::Vector dir;	
	
	GameState* state;
	int radius;
	int radius2;
};

#endif



