#include "../IBrain.h"
#include "../BrainPLayerInput.h"
#include "../GameState.h"

IBrain* IBrain::getBrain(std::string name) {
	if (name == "BrainPlayerInput")
		return brainPlayerInput;
}

void IBrain::initBrains(InputData const* input) {
	IBrain::brainPlayerInput = new BrainPlayerInput(input);
}

IBrain* IBrain::brainPlayerInput = NULL;


void BrainFollow::decide(IBody* body) {
	std::vector<Vertex> path;	
	while (!init || !body->state->getEnvironment()->findPath(body->pos, end, path)) {
		body->velocity = Vector(0, 0);
		std::vector<Vertex> const& nodes = body->state->getEnvironment()->getGraph()->getNodes();
		end = nodes[rand() % nodes.size()];
		init = true;
	}
	body->velocity = path[1] - path[0];
	body->velocity *= 5.f/sqrt(1.f*geo::distance2(body->velocity));
}