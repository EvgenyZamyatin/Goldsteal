#include "IBrain.h"
#include "BrainPLayerInput.h"

IBrain* IBrain::getBrain(std::string name) {
	if (name == "BrainPlayerInput")
		return brainPlayerInput;
}

void IBrain::initBrains(InputData const* input) {
	IBrain::brainPlayerInput = new BrainPlayerInput(input);
}

IBrain* IBrain::brainPlayerInput = NULL;