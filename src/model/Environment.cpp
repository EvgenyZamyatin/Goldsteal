#include "Environment.h"

void Environment::render(HGE* hge) {	
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < hight; ++j) {
			hgeQuad quad = tiles->get(gridIndxs[j*width+i]);
			quad.v[0].x=(i)*tileWidth;
			quad.v[0].y=(j)*tileHight;
			quad.v[0].z=0.5;

			quad.v[1].x=(i)*tileWidth+tileWidth;
			quad.v[1].y=(j)*tileHight;
			quad.v[1].z=0.5;

			quad.v[2].x=(i)*tileWidth+tileWidth;
			quad.v[2].y=(j)*tileHight+tileHight;
			quad.v[2].z=0.5;

			quad.v[3].x=(i)*tileWidth;
			quad.v[3].y=(j)*tileHight+tileHight;
			quad.v[3].z=0.5;
			
			quad.blend=BLEND_DEFAULT_Z;

			for(int i=0;i<4;i++) {
				quad.v[i].z=0.5f;
				quad.v[i].col=ARGB(255,255,255,255);
			}	
			hge->Gfx_RenderQuad(&quad);
		}
	}
}

HTEXTURE Environment::compile(HGE* hge) {
	bool ok = (texture != 0);
	for (IObject* o : objs)
		ok &= o->clean;
	//if (ok) 
	//	return texture;
	if (target == 0)
		target = hge->Target_Create(tileWidth*width, tileHight*hight, true);
	//Strange???
	//if (texture != 0)
	//	hge->Texture_Free(texture);
	hge->Gfx_BeginScene(target);
	hge->Gfx_Clear(0);
	render(hge);
	for (IObject* o : objs) {
		o->render(hge);
		o->clean = true;
	}
	hge->Gfx_EndScene();
	texture=hge->Target_GetTexture(target);
	return texture;	
}