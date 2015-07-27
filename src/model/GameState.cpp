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

void GameState::tryMove(IBody* body, Geo::Vector& vel) {
	if (Geo::equals(vel.len(), 0))
		return; 
	
	Geo::Vector newPos = body->pos + vel;
	//double d = 1000000000;
	//Geo::Segment mn;
	bool was = false;
	static std::vector<Geo::Vector> newVels;
	newVels.clear();
	newVels.push_back(vel);

	for (IObject* obj : env->objs) {
		for (int i = 0; i < obj->bounds.size(); ++i) {
			Geo::Segment s(obj->bounds[i], obj->bounds[(i+1)%obj->bounds.size()]);
			double d = Geo::distance(newPos, s);
			if (Geo::less(d, body->collisionRadius)) {
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
			if (Geo::less(Geo::distance(newPos, obj->bounds), body->collisionRadius))
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
}







