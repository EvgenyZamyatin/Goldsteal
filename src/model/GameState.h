#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "Environment.h"                   
#include "../utils/Resources.h"
#include <hge.h>
#include <vector>
#include "IBody.h"

struct GameState {
	Environment* env;
	Resources* res;
	std::vector<IBody*> getBodies() {return bodies;}
	void addBody(IBody* body) {bodies.push_back(body);}
	//HTEXTURE getCurrenConfiguration(HGE* hge);

private:
	//HTARGET target;
	std::vector<IBody*> bodies;

};
#endif