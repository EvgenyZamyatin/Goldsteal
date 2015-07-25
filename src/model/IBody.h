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

struct IBody : IObject {                
	static const int STATE_IDLE = 0;
	static const int STATE_WALK = 1;
	
	static const double MAX_SPEED;
	static const double ACCELERATION;

	IBody(Tmx::Object const* obj, hgeResourceManager* res);
	virtual ~IBody(){};                      
	
	void setGameState(GameState* state) {this->state=state;}
	
	Geo::Vector getDir() const {return dir;}
	double getViewAngle() const {return viewAngle;}
	void postChanges() {changes();}
    void frame();
    virtual void render(HGE* hge, Camera const* cam);
    void move(Geo::Vector const& v);
	
	friend struct BrainPlayerInput;

private:
	Render::BodyData rData;

	IBrain* brain;

	int moveState=0;
	
	double viewAngle;	
	Geo::Vector velocity;
	GameState* state;
	Geo::Polygon visible;
	Geo::Vector dir;	

	std::function<void()> changes=[](){};
};

#endif