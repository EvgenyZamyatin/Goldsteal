#include "GameState.h"

GameState::GameState(Tmx::Map const* map, hgeResourceManager* res) {
	env = new Environment(map, res);
	for (Tmx::ObjectGroup const* ob : map->GetObjectGroups()) {
		for (Tmx::Object const* o : ob->GetObjects()) {
			if (o->GetType() == "Body") {
				addBody(new IBody(o, res));
			} else if (o->GetType() == "Hero") {
				setHero(new IBody(o, res));
			}
		}
	}
}

void GameState::tryMove(IBody* body, Geo::Vector const& vel) {
	if (Geo::equals(vel.len(), 0))
		return; 
	
	double mx = 0;
	for (Geo::Vector const& v : body->bounds.points) {
		mx = std::max(mx, (body->pos-v).len());
	}

	body->pos += vel;
	bool ok = true;
	for (IObject* obj : env->objs) {
		if (Geo::less(Geo::distance(body->pos, obj->bounds), mx)) {
			body->onCollision(obj);
			ok = false;
		}
	}
	for (IBody* b : bodies) {
		if (b == body)
			continue;
		if (Geo::less(Geo::distance(body->pos, b->bounds), mx)) {
			body->onCollision(b);
			b->onCollision(body);
			ok = false;
		}
	}
	if (ok) {
		for (Geo::Vector& v : body->bounds.points)
			v += vel;
	} else {
		body->pos -= vel;
	}
}