#include "GameState.h"

GameState::GameState(Tmx::Map const* map, hgeResourceManager* res) {
	env = new Environment(map, res);
	for (Tmx::ObjectGroup const* ob : map->GetObjectGroups()) {
		if (ob->GetName() != "BodyLayer")
			continue;
		for (Tmx::Object const* o : ob->GetObjects()) {
			if (o->GetType() == "Body") {
				addBody(new IBody(o, res));
			} else if (o->GetType() == "Hero") {
				setHero(new IBody(o, res));
			}
		}
	}
}

void GameState::process(IBody* body) {
	body->pos += body->velocity;
	/*
	body->pos += body->velocity;
	for (Geo::Vector& v : body->bounds.points)
		v += body->velocity;
	return;
	Geo::Vector& vel = body->velocity; 
	if (vel.len2() == 0)
		return; 
	
	Geo::Vector newPos = body->pos + vel;
	
	bool was = false;
	std::vector<Geo::Vector> newVels;
	newVels.clear();
	newVels.push_back(vel);

	for (IObject* obj : env->objs) {
		for (int i = 0; i < obj->bounds.size(); ++i) {
			Geo::Segment s(obj->bounds[i], obj->bounds[i+1]);
			int d = Geo::distance2(newPos, s);
			if (d < body->collisionRadius2) {
				Geo::Vector n((s.b-s.a).normal());
				Geo::Vector part(n*(vel^n));
				newVels.push_back(vel-part);
				was = true;
			}
		}
		if (was)
			break;
	}
	newVels.push_back({0,0});
	for (Geo::Vector& nvel : newVels) {		
		newPos = body->pos + nvel;
		bool ok = true;
		for (IObject* obj : env->objs) {
			if (Geo::distance2(newPos, obj->bounds) < body->collisionRadius2)
				ok = false;
		}
		if (ok) {
			body->pos = newPos;
			for (Geo::Vector& v : body->bounds.points)
				v += nvel;
			vel = nvel;
			return;
		}
	}
	assert(false);
	*/
}

void GameState::frame() {
	process(hero);
	for (IBody* body : bodies) 
		process(body);
}
