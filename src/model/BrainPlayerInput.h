#ifndef BRAINPLAYERINPUT_H
#define BRAINPLAYERINPUT_H
#include "IBrain.h"
#include "IBody.h"

struct BrainPlayerInput : IBrain {
	BrainPlayerInput(InputData const* input);
	virtual void decide(IBody* body);
private:
	InputData const* input;
};
#endif