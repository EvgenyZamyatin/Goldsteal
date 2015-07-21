#ifndef IBODY_H
#define IBODY_H
#include "IObject.h"
#include "Environment.h"

struct IBody : IObject {
	virtual ~IBody(){};                      
	virtual double getViewAngle() = 0;
	virtual void frame(int event) = 0;
	void setGameState(GameState const* state) {this->state=state;}
private:
	GameState* state;
	Geo::Polygon visible;	
};

#endif