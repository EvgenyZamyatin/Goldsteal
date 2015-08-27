#ifndef IBRAIN_H
#define IBRAIN_H
#include <string>
#include "../InputData.h"
#include "Vertex.h"

struct IBody;
struct IBrain {
	static void initBrains(InputData const* input);

	static IBrain* getBrain(std::string name);
	virtual void decide(IBody* body) = 0;
	
private:
	static IBrain* brainPlayerInput;
};

struct BrainFollow : IBrain {
	BrainFollow() {init = false;}
	virtual void decide(IBody* body);

private:
	Vertex end;
	bool init;
};


#endif