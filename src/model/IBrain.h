#ifndef IBRAIN_H
#define IBRAIN_H

struct IBrain {

public:
	virtual ~IBrain(){};
	virtual void makeDecision(Body&, const Influence&) = 0;
};

#endif