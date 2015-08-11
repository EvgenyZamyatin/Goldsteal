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
	target1 = res->GetTarget("GameStateTarget1");
	if (target1 == 0) {
		std::cerr << "Can't find GameStateTarget1\n";
		exit(0);
	}

	target2 = res->GetTarget("GameStateTarget2");
	if (target2 == 0) {
		std::cerr << "Can't find GameStateTarget2\n";
		exit(0);
	}
}

void GameState::process(IBody* body) {
	//body->pos += body->velocity;
	//return;
	Geo::Vector& vel = body->velocity; 
	if (vel.len2() == 0)
		return; 
	
	Geo::Vector newPos = body->pos + vel;
	bool was = false;
	std::vector<Geo::Vector> newVels;
	newVels.push_back(vel);

	for (IObject* obj : env->objs) {
		if ((obj->center - body->pos).len2() >= ((std::max(body->radius2, obj->radius2))<<2))
			continue; 
		for (int i = 0; i < obj->bounds.size(); ++i) {
			Geo::Segment s(obj->bounds[i], obj->bounds[i+1]);
			int d = Geo::distance2(newPos, s);
			if (d < body->radius2) {
				Geo::Vector n((s.b-s.a).normal());
				Geo::Vector part(n*(vel^n));
				Geo::Vector vel1(vel);
				vel1 *= n.len2();
				Geo::Vector res=vel1-part;
				res.x = n.x > 0 ? (((res.x + n.len2() - 1) / n.len2())) : res.x / n.len2();
				res.y = n.y > 0 ? (((res.y + n.len2() - 1) / n.len2())) : res.y / n.len2();
                //res.x = n.x > 0 ? (res.x >= 0 ? ((res.x + n.len2() - 1) / n.len2()) : ((res.x - n.len2() + 1) / n.len2())) : res.x / n.len2();
                newVels.push_back(res);
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
			if (Geo::distance2(newPos, obj->bounds) < body->radius2)
				ok = false;
		}
		if (ok) {
			body->pos = newPos;
			vel = nvel;
			break;
		}
	}                                                 
	body->pos.x = std::max(body->radius, body->pos.x);
	body->pos.x = std::min(env->getWidth() - body->radius, body->pos.x);

	body->pos.y = std::max(body->radius, body->pos.y);
	body->pos.y = std::min(env->getHight() - body->radius, body->pos.y);
}

void GameState::frame() {
	hero->frame();
	for (IBody* b : bodies)
		b->frame();

	process(hero);
	for (IBody* body : bodies) 
		process(body);
	env->frame();
}




























