#ifndef IBODY_H
#define IBODY_H

#include "Forward.h"
#include "IObject.h"
#include "Environment.h"
#include "../geometry/Geometry.h"
#include "GameState.h"

struct IBody : IObject {
	virtual ~IBody(){};                      
	virtual double getViewAngle() = 0;
	virtual void frame(int event) = 0;
	void setGameState(GameState* state) {this->state=state;}
	//friend struct Camera;
private:
	GameState* state;
	Geo::Polygon visible;	
};

#endif