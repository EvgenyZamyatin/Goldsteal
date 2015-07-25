#include "GameState.h"

GameState::GameState(Tmx::Map const* map, hgeResourceManager* res) {
	env = new Environment(map, res);
	for (Tmx::ObjectGroup const* ob : map->GetObjectGroups()) {
		for (Tmx::Object const* o : ob->GetObjects()) {
			if (o->GetType() == "Body") {
				addBody(new IBody(o, res));
			} else if (o->GetType() == "Hero") {
				hero = new IBody(o, res);
			}
		}
	}
}