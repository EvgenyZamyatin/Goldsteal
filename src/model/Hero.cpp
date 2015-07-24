#include "Hero.h"

int sign(double x) {
	return (Geo::less(x, 0)) -1 : 1;
}

void Hero::frame(int event, Geo::Vector mouse) {
	if ((event & INPUT_UP) || (evnet & INPUT_DOWN))
		velocity.y += event == INPUT_UP ? -acceleration : acceleration;
	else 
		velocity.y += -sign(velocity.y)*acceleration;
	
	if ((event & INPUT_LEFT) || (event & INPUT_RIGHT))
		velocity.x += event == INPUT_LEFT ? -acceleration : acceleration;
	else 
		velocity.x += -sign(velocity.x)*acceleration;
	velocity.x = std::min(velocity.x, maxSpeed);
	velocity.y = std::min(velocity.y, maxSpeed);

	velocity.x = std::max(velocity.x, -maxSpeed);
	velocity.y = std::max(velocity.y, -maxSpeed);
	if (Geo::greater(velocity.len(), 0))
		legState = LEG_STATE_WALK;
	else
	    legState = LEG_STATE_IDLE;
	dir = mouse-pos;
	pos += velocity;
	for (Geo::Vector& v : bounds.points)
		v += velocity;
}