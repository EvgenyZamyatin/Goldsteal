#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <hgeresource.h>
#include <vector>
#include <tmx/Tmx.h.in>

#include "Environment.h"                   
#include "IRenderable.h"
#include "Camera.h"
#include "IBody.h"

struct GameState : IRenderable {
	GameState() {}
	GameState(Tmx::Map const* map, hgeResourceManager* res);
	    
//	std::vector<IBody*> GameState::getBodies() {return bodies;}
//	std::vector<IBody*>& GameState::getBodies() const {return bodies;}

	void addBody(IBody* body) {body->setGameState(this); bodies.push_back(body);}
	
	void render(HGE* hge, Camera* cam);
	
	void setEnvironment(Environment* env) {this->env=env;}
	Environment* getEnvironment() {return env;}
	
	//void setHero(Hero* hero) {this->hero = hero;}
	//Hero* getHero() {return hero;}

private:
	std::vector<IBody*> bodies;
	Environment* env;
};
#endif