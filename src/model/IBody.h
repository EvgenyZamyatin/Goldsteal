#ifndef IBODY_H
#define IBODY_H
#include "IObject.h"
#include "Environment.h"

struct IBody : IObject {
	virtual ~IBody(){};                      
	virtual double getViewAngle() = 0;
	virtual void frame(HGE* hge) = 0;
private:
	Environment* env;	
};

#endif