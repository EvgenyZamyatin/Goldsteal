#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Forward.h"
#include <vector>
#include "Environment.h"                   
#include "Camera.h"

struct GameState {
    GameState(Environment* env) : env(env) {}
	std::vector<IBody*> getBodies();
	std::vector<IBody*>& getBodies() const;
	void addBody(IBody* body);
	void frame(int event);
	Environment* getEnvironment();
	friend class Camera;

private:
	std::vector<IBody*> bodies;
	Environment* env;
	IBody* hero;
};
#endif