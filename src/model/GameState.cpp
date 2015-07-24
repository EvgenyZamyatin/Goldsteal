#include "GameState.h"

GameState::GameState(Tmx::Map const* map, hgeResourceManager* res) {
	env = new Environment(map, res);
}