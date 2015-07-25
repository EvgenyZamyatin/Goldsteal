#include "IBody.h"

const double IBody::MAX_SPEED = 3;
const double IBody::ACCELERATION = 1;

IBody::IBody(Tmx::Object const* o, hgeResourceManager* res) : IObject(o, res) {
	brain = IBrain::getBrain(o->GetProperties().GetStringProperty("Brain"));
	rData.lastState=-1;
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

void IBody::move(Geo::Vector const& v) {
	pos += v;
	for (Geo::Vector& b : bounds.points)
		b += v;
}