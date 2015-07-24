#ifndef IBRAIN_H
#define IBRAIN_H
#include "Body.h"
struct IBrain {

public:
	virtual ~IBrain(){};
	virtual void makeDecision(Body* body) {};
};

#endif