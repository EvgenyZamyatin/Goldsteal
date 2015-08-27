#include "BrainPlayerInput.h"
#include "../GameState.h"


BrainPlayerInput::BrainPlayerInput(InputData const* input) {
	this->input = input;
}

void BrainPlayerInput::decide(IBody* body) {
	InputData in = *input;

	Vector newDir = Vector(in.mX, in.mY) - body->pos;
	if (newDir != Vector(0,0))	
		body->dir = newDir;
	#ifdef DEBUG
		assert(body->dir != Vector(0,0));
	#endif
	
	body->visible = body->state->getEnvironment()->calcVisible(body->pos);

	if (in.pUp || in.pDown)
		body->velocity.y += in.pUp ? -IBody::ACCELERATION : IBody::ACCELERATION;
	else {
		if (body->velocity.y > 0) {
			body->velocity.y -= IBody::ACCELERATION;
			body->velocity.y = std::max(0LL, body->velocity.y);
		} else {
			body->velocity.y += IBody::ACCELERATION;
			body->velocity.y = std::min(0LL, body->velocity.y);
		}
	}

	if (in.pLeft || in.pRight)
		body->velocity.x += in.pRight ? IBody::ACCELERATION : -IBody::ACCELERATION;
	else {
		if (body->velocity.x > 0) {
			body->velocity.x -= IBody::ACCELERATION;
			body->velocity.x = std::max(0LL, body->velocity.x);
		} else {
			body->velocity.x += IBody::ACCELERATION;
			body->velocity.x = std::min(0LL, body->velocity.x);
		}
	}

		
	body->velocity.x = std::min(body->velocity.x, (long long)IBody::MAX_SPEED);
	body->velocity.x = std::max(body->velocity.x, (long long)-IBody::MAX_SPEED);
		
	body->velocity.y = std::min(body->velocity.y, (long long)IBody::MAX_SPEED);
	body->velocity.y = std::max(body->velocity.y, (long long)-IBody::MAX_SPEED);
		
	if (geo::distance2(body->velocity) > 0)
		body->moveState=IBody::STATE_WALK;
	else
		body->moveState=IBody::STATE_IDLE;
}
