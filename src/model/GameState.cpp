#include "GameState.h"

std::vector<IBody*> GameState::getBodies() {return bodies;}
std::vector<IBody*>& GameState::getBodies() const {return bodies;}

void GameState::addBody(IBody* body) {
	bodies.push_back(body);
	body->setGameState(this);		
}

void GameState::frame(int event) {}

Environment* getEnvironment() {return env;}