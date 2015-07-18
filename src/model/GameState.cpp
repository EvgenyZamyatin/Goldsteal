#include "GameState.h"


/*HTEXTURE GameState::getCurrentConfiguration(HGE* hge) {
	if (target == 0)
		target = hge->Target_Create(env->getWidth(), env->getHight(), true);
	hge->Gfx_BeginScene(target);
	hge->Gfx_Clear(0);
	for (IBody* body : bodies) 
		body->render(hge);
	hge->Gfx_EndScene();
	return hge->Target_GetTexture(target);
}*/