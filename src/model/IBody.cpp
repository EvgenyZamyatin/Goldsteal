#include "IBody.h"

const double IBody::MAX_SPEED = 3;
const double IBody::ACCELERATION = 1;

IBody::IBody(Tmx::Object const* o, hgeResourceManager* res) : IObject(o, res), dir(0,-1) {
	brain = IBrain::getBrain(o->GetProperties().GetStringProperty("Brain"));
	rData.lastState=-1;
	collisionRadius = o->GetProperties().GetFloatProperty("CollisionRadius");
	hgeAnimation* anim=res->GetAnimation(o->GetProperties().GetStringProperty("STATE_IDLE").c_str());
	if (anim == 0) {
		std::cerr << "Can't find " << o->GetProperties().GetStringProperty("STATE_IDLE");
		exit(0);
    }                                              
	rData.anims.push_back(*anim);
	anim=res->GetAnimation(o->GetProperties().GetStringProperty("STATE_WALK").c_str());
	if (anim == 0) {
		std::cerr << "Can't find " << o->GetProperties().GetStringProperty("STATE_WALK");
		exit(0);
    }                                              
	rData.anims.push_back(*anim);
}

void IBody::frame() {
	brain->decide(this);
}