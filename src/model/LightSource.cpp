#include "Environment.h"
#include "LightSource.h"

void LightSource::setEnvironment(Environment* env) {this->env = env; bounds = env->calcVisible(pos);}