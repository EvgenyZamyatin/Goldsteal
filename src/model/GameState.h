#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Forward.h"
#include <vector>
#include "Environment.h"                   
#include "Camera.h"
#include "IRenderable.h"
#include "Hero.h"
struct LevelLoader;


struct GameState {
	GameState() {}
    GameState(Environment* env) : env(env) {}
	std::vector<IBody*> getBodies();
	std::vector<IBody*>& getBodies() const;
	void addBody(IBody* body);
	void frame(int event);
	Environment* getEnvironment() {return env;}
	void setHero(Hero* hero) {this->hero = hero;}
	Hero* getHero() {return hero;}
	
	friend struct Camera;
	friend struct LevelLoader;

private:
	std::vector<IBody*> bodies;
	Environment* env;
	Hero* hero;
};
#endif