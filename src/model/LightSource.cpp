#include "LightSource.h"
#include "Environment.h"

LightSource::LightSource(Tmx::Object const* o, hgeResourceManager* res) : IObject(o, res){}

void LightSource::frame() {
	bounds = env->calcVisible(pos);
}