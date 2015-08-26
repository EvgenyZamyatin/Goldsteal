#include "../Environment.h"
#include "../LightSource.h"

void LightSource::setEnvironment(Environment const* env) {this->env = env; env->findVisibility(pos, bounds);}

void LightSource::frame() {
}