#ifndef IBODY_H
#define IBODY_H

#include <tmx/TmxObject.h>
#include <hgeresource.h>

#include "IObject.h"
#include "Environment.h"
#include "../geometry/Geometry.h"
#include "../render/BodyData.h"
#include "Camera.h"


struct GameState;

struct IBody : IObject {                
	const int LEG_STATE_IDLE = 0;
	const int BODY_STATE_IDLE = 0;
	const double MAX_SPEED = 3;
	const double ACCELERATION = 1;

	IBody(Tmx::Object const* obj, hgeResourceManager* res);
	virtual ~IBody(){};                      
	
	void setGameState(GameState* state) {this->state=state;}
	
	Geo::Vector getDir() const {return dir;}
	double getViewAngle() const {return viewAngle;}
   
    virtual void render(HGE* hge, Camera const* cam);
	
private:
	Render::BodyData rData;

	int legState=0;
	int bodyState=0;
	
	double viewAngle;	
	Geo::Vector velocity;
	GameState* state;
	Geo::Polygon visible;
	Geo::Vector dir;	
};

#endif