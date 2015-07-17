#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "Environment.h"                   
#include "../utils/Resources.h"

struct GameState {
	Environment* env;
	Resources* res;
	double screenWidth;
	double screenHight;

};
#endif