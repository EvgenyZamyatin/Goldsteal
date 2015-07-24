#ifndef IBODY_H
#define IBODY_H

#include "Forward.h"
#include "IObject.h"
#include "Environment.h"
#include "../geometry/Geometry.h"
#include "GameState.h"
#include "../render/BodyData.h"

struct LevelLoader;
struct IBrain;

struct IBody : IObject {                
	const int LEG_STATE_IDLE = 0;
	
	const int BODY_STATE_IDLE = 0;
	const int BODY_STATE_WALK = 1;

	const double maxSpeed = 3;
	const double acceleration = 1;

	virtual ~IBody(){};                      
	void setGameState(GameState* state) {this->state=state;}
	double getViewAngle() const {return viewAngle;}
    void move(Geo::Vector vec) {pos += vec;}
    virtual void render(HGE* hge, Camera const* cam);
	virtual void frame();
	friend struct LevelLoader;
private:
	Render::BodyData rData;
	int legState=0;
	int bodyState=0;
	
	//IBrain* brain;
	double viewAngle;	
	Geo::Vector velocity;
	GameState* state;
	Geo::Polygon visible;	
};

#endif