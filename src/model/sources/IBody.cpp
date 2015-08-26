#include "../IBody.h"
#include "../IBrain.h"

const int IBody::MAX_SPEED = 8;
const int IBody::ACCELERATION = 1;

IBody::IBody(Tmx::Object const* o, hgeResourceManager* res) : dir(0,-1), velocity(0,0) {
	pos.x = o->GetX()+o->GetWidth()/2;
	pos.y = o->GetY()-o->GetHeight()/2;
	
	rData.width = (int)o->GetWidth();
	rData.hight = (int)o->GetHeight();
	brain = IBrain::getBrain(o->GetProperties().GetStringProperty("Brain"));
	rData.lastState=-1;
	
	radius = o->GetProperties().GetFloatProperty("Radius");
	radius2 = radius * radius;
	
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