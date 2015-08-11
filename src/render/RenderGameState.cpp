#include "../model/GameState.h"
#include "help.h"
//#include "../model/LightSource.h"

hgeQuad q;
hgeTriple trip;    		
hgeU32 col;
    
void rendWhitePolygon(HGE* hge, Camera* cam, Geo::Vector const& pos, Geo::Polygon const& poly) {
	trip.tex = 0;
	trip.blend = BLEND_DEFAULT;
	for (int i = 0; i < poly.size(); ++i) {
		Geo::Vector a = poly[i], b = poly[i+1];
		col = ARGB(255,255,255,255);                      	
		float x1=pos.x,y1=pos.y,x2=a.x,y2=a.y,x3=b.x,y3=b.y;
		cam->convertGS(x1,y1);
		cam->convertGS(x2,y2);
		cam->convertGS(x3,y3);
		fillTriple(trip, {
						  {x1, y1, 0.f, col, 1.f, 1.f},
						  {x2, y2, 0.f, col, 1.f, 1.f},
						  {x3, y3, 0.f, col, 1.f, 1.f}
						 });
		hge->Gfx_RenderTriple(&trip);
	}	
}

void GameState::render(HGE* hge, Camera* cam) {
	{//DEBUG
	 	#ifdef DEBUG                                   
        	HTEXTURE tex = hge->Target_GetTexture(target1);
        	assert(hge->Texture_GetWidth(tex) == 1024);
        	assert(hge->Texture_GetHeight(tex) == 1024);
        	tex = hge->Target_GetTexture(target2);
        	assert(hge->Texture_GetWidth(tex) == 1024);
        	assert(hge->Texture_GetHeight(tex) == 1024);
        #endif
	}

	{//MAKE VIEW-LIGHT TARGET(in target1)
		hge->Gfx_BeginScene(target2);
		hge->Gfx_Clear(0);
		for (LightSource const* ls : env->getLightSources()) {
			rendWhitePolygon(hge, cam, ls->pos, ls->getBounds());
	   	}
		hge->Gfx_EndScene();

		hge->Gfx_BeginScene(target1);
		hge->Gfx_Clear(0);
		rendWhitePolygon(hge, cam, hero->pos, hero->visible);
		
		q.blend=BLEND_DARKEN;
		col = ARGB(255,255,255,255);
		q.tex = hge->Target_GetTexture(target2);
		fillQuad(q, {
    	            	{0, 0, 0.5f, col, 0.f, 0.f},
    	            	{(float)cam->SCREEN_WIDTH, 0, 0.5f, col, (float)cam->SCREEN_WIDTH/1024, 0.f},
    					{(float)cam->SCREEN_WIDTH, (float)cam->SCREEN_HIGHT, 0.5f, col, (float)cam->SCREEN_WIDTH/1024, (float)cam->SCREEN_HIGHT/1024},
    					{0, (float)cam->SCREEN_HIGHT, 0.5f, col, 0.f, (float)cam->SCREEN_HIGHT/1024},
    				});
    	hge->Gfx_RenderQuad(&q);
    	
    	q.blend=BLEND_ALPHAADD;
		col = ARGB(255,16,16,16);
		q.tex = 0;
		fillQuad(q, {
    	            	{0, 0, 0.5f, col, 1.f, 1.f},
    	            	{(float)cam->SCREEN_WIDTH, 0, 0.5f, col, 1.f, 1.f},
    					{(float)cam->SCREEN_WIDTH, (float)cam->SCREEN_HIGHT, 0.5f, col, 1.f, 1.f},
    					{0, (float)cam->SCREEN_HIGHT, 0.5f, col, 1.f, 1.f},
    				});
    	hge->Gfx_RenderQuad(&q);
    	
    	hge->Gfx_EndScene();
	}

    {
    	hge->Gfx_BeginScene();
    	hge->Gfx_Clear(0);
    	env->render(hge, cam);
    	for (IObject* obj : env->getObjects()) {
    		obj->render(hge, cam);
    	}
    	hero->render(hge, cam);
    	for (IBody * b : bodies)
    		b->render(hge, cam);
    	for (LightSource* ls : env->getLightSources())
    		ls->render(hge, cam);
    	q.blend=BLEND_DEFAULT;
		col = ARGB(50, 255, 238, 173);
		q.tex = 0;
		fillQuad(q, {
    	            	{0, 0, 0.5f, col, 1.f, 1.f},
    	            	{(float)cam->SCREEN_WIDTH, 0, 0.5f, col, 1.f, 1.f},
    					{(float)cam->SCREEN_WIDTH, (float)cam->SCREEN_HIGHT, 0.5f, col, 1.f, 1.f},
    					{0, (float)cam->SCREEN_HIGHT, 0.5f, col, 1.f, 1.f},
    				});
    	hge->Gfx_RenderQuad(&q);
		q.blend=BLEND_DARKEN;
		col = ARGB(255, 255, 255, 255);
		q.tex = hge->Target_GetTexture(target1);
		fillQuad(q, {
    	            	{0, 0, 0.5f, col, 0.f, 0.f},
    	            	{(float)cam->SCREEN_WIDTH, 0, 0.5f, col,(float)cam->SCREEN_WIDTH/1024, 0.f},
    					{(float)cam->SCREEN_WIDTH, (float)cam->SCREEN_HIGHT, 0.5f, col, (float)cam->SCREEN_WIDTH/1024, (float)cam->SCREEN_HIGHT/1024},
    					{0, (float)cam->SCREEN_HIGHT, 0.5f, col,  0.f, (float)cam->SCREEN_HIGHT/1024},
    				});
    	hge->Gfx_RenderQuad(&q);
    	hge->Gfx_EndScene();
    }

    return;
    
        
    
    {
    	hge->Gfx_BeginScene(target2);
    	
    	HTEXTURE tex1 = hge->Target_GetTexture(target1);
    	hgeU32 col = ARGB(255,255,255,255);
    	q.blend=BLEND_DEFAULT;
    	q.tex=tex1;
    	fillQuad(q, {
    	            	{0, 0, 0.5f, col, 0.f, 0.f},
    	            	{(float)cam->SCREEN_WIDTH, 0, 0.5f, col, (float)cam->SCREEN_WIDTH/1024, 0.f},
    					{(float)cam->SCREEN_WIDTH, (float)cam->SCREEN_HIGHT, 0.5f, col, (float)cam->SCREEN_WIDTH/1024, (float)cam->SCREEN_HIGHT/1024},
    					{0, (float)cam->SCREEN_HIGHT, 0.5f, col, 0.f, (float)cam->SCREEN_HIGHT/1024},
    				});
    	hge->Gfx_RenderQuad(&q);
		        
        
    	hero->render(hge, cam);
    	for (IBody* body : bodies)
    		body->render(hge, cam);
    	for (LightSource* ls : env->getLightSources()) {
    		ls->render(hge, cam);
    	}
		hge->Gfx_EndScene();
	}	
	{
		hge->Gfx_BeginScene();
    	hge->Gfx_Clear(0);
    	q.tex=hge->Target_GetTexture(target1);
    	q.blend=BLEND_DEFAULT;
    	hgeU32 col = ARGB(255, 255, 255, 255);
    	fillQuad(q, {
    	            	{0, 0, 0.5f, col, 0.f, 0.f},
    	            	{(float)cam->SCREEN_WIDTH, 0, 0.5f, col, (float)cam->SCREEN_WIDTH/1024, 0.f},
    					{(float)cam->SCREEN_WIDTH, (float)cam->SCREEN_HIGHT, 0.5f, col, (float)cam->SCREEN_WIDTH/1024, (float)cam->SCREEN_HIGHT/1024},
    					{0, (float)cam->SCREEN_HIGHT, 0.5f, col, 0.f, (float)cam->SCREEN_HIGHT/1024},
    				});
    	hge->Gfx_RenderQuad(&q);

    	q.tex=0;
    	q.blend=BLEND_DARKEN | BLEND_NOZWRITE;
    	col = ARGB(255, 0, 0, 0);
    	fillQuad(q, {
    	            	{0, 0, 0.5f, col, 0.f, 0.f},
    	            	{(float)cam->SCREEN_WIDTH, 0, 0.5f, col, 1.f, 0.f},
    					{(float)cam->SCREEN_WIDTH, (float)cam->SCREEN_HIGHT, 0.5f, col, 1.f, 1.f},
    					{0, (float)cam->SCREEN_HIGHT, 0.5f, col, 0.f, 1.f},
    				});
    	hge->Gfx_RenderQuad(&q);

    
        trip.tex = hge->Target_GetTexture(target2);
    	trip.blend = BLEND_DEFAULT;
    		
    	for (int i = 0; i < hero->visible.size(); ++i) {
    		Geo::Vector a = hero->visible[i], b = hero->visible[i+1];
    		hgeU32 col = ARGB(255,255,255,255);                      
    		
    		float x1=hero->pos.x,y1=hero->pos.y,x2=a.x,y2=a.y,x3=b.x,y3=b.y;
    		
    		cam->convertGS(x1,y1);
    		cam->convertGS(x2,y2);
    		cam->convertGS(x3,y3);

    		fillTriple(trip, {
    						  {x1, y1, 0.f, col, x1/1024, y1/1024},
    						  {x2, y2, 0.f, col, x2/1024, y2/1024},
    						  {x3, y3, 0.f, col, x3/1024, y3/1024}
    						 });
    		hge->Gfx_RenderTriple(&trip);
    	}

    	hge->Gfx_EndScene();
    }
}

