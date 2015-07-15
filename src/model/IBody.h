#ifndef IBODY_H
#define IBODY_H
#include "IReal.h"

struct IBody : IReal {

public:
	virtual ~IBody(){};                      
	virtual double getViewAngle() = 0;
};

#endif