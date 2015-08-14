#include "../model/GameState.h"
#include "help.h"
#include "RenderData.h"

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

void Render::GameStateData::blur(HGE* hge, Camera* cam, HTARGET a, HTARGET b) {
	{//make temp resized
        hge->Gfx_BeginScene(b);
    	hge->Gfx_Clear(0);
    	q.blend=BLEND_DEFAULT;
    	col = ARGB(255,255,255,255);
    	q.tex = hge->Target_GetTexture(a);
    	fillQuad(q, {
        	            {(float)(kSize/2),                     (float)(kSize/2),                     0.5f, col, 0.f,                           0.f},
        	            {(float)cam->SCREEN_WIDTH + (kSize/2), (float)(kSize/2),                     0.5f, col, (float)cam->SCREEN_WIDTH/1024, 0.f},
        				{(float)cam->SCREEN_WIDTH + (kSize/2), (float)cam->SCREEN_HIGHT + (kSize/2), 0.5f, col, (float)cam->SCREEN_WIDTH/1024, (float)cam->SCREEN_HIGHT/1024},
        				{(float)(kSize/2),                     (float)cam->SCREEN_HIGHT + (kSize/2), 0.5f, col, 0.f,                           (float)cam->SCREEN_HIGHT/1024},
        			});
        hge->Gfx_RenderQuad(&q);
        hge->Gfx_EndScene();
    }
    
    hge->Shader_SetValue(blurShader, "kSize", (void*)(&kSize), sizeof(int));
	hge->Shader_SetValue(blurShader, "koeff", (void*)(kernel), kSize*sizeof(float));
	
    {//Horizontal
    	int kSize = kSize;
    	
    	hge->Gfx_BeginScene(a);
    	hge->Gfx_SetShader(blurShader, SHADER_PIXEL);
    	hge->Shader_SetValue(blurShader, "direction", (void*)(dirX), 2*sizeof(float));
		
		q.blend=BLEND_DEFAULT;
    	col = ARGB(255,255,255,255);
    	q.tex = hge->Target_GetTexture(b);
    	fillQuad(q, {
        	            {0,                                      0,                                      0.5f, col, 0.f,                                         0.f},
        	            {(float)cam->SCREEN_WIDTH + (kSize/2)*2, 0,                                      0.5f, col, (float)(cam->SCREEN_WIDTH+(kSize/2)*2)/1024, 0.f},
        				{(float)cam->SCREEN_WIDTH + (kSize/2)*2, (float)cam->SCREEN_HIGHT + (kSize/2)*2, 0.5f, col, (float)(cam->SCREEN_WIDTH+(kSize/2)*2)/1024, (float)(cam->SCREEN_HIGHT+(kSize/2)*2)/1024},
        				{0,                                      (float)cam->SCREEN_HIGHT + (kSize/2)*2, 0.5f, col, 0.f,                                         (float)(cam->SCREEN_HIGHT+(kSize/2)*2)/1024},
        			});
        hge->Gfx_RenderQuad(&q);
        hge->Gfx_EndScene();
	} 
	{//Vertical
		int kSize = kSize;
    	
		hge->Gfx_BeginScene(b);    	
    	hge->Gfx_SetShader(blurShader, SHADER_PIXEL);
    	hge->Shader_SetValue(blurShader, "direction", (void*)(dirY), 2*sizeof(float));
		
		q.blend=BLEND_DEFAULT;
    	col = ARGB(255,255,255,255);
    	q.tex = hge->Target_GetTexture(a);
    	fillQuad(q, {
        	            {0,                                      0,                                      0.5f, col, 0.f,                                         0.f},
        	            {(float)cam->SCREEN_WIDTH + (kSize/2)*2, 0,                                      0.5f, col, (float)(cam->SCREEN_WIDTH+(kSize/2)*2)/1024, 0.f},
        				{(float)cam->SCREEN_WIDTH + (kSize/2)*2, (float)cam->SCREEN_HIGHT + (kSize/2)*2, 0.5f, col, (float)(cam->SCREEN_WIDTH+(kSize/2)*2)/1024, (float)(cam->SCREEN_HIGHT+(kSize/2)*2)/1024},
        				{0,                                      (float)cam->SCREEN_HIGHT + (kSize/2)*2, 0.5f, col, 0.f,                                         (float)(cam->SCREEN_HIGHT+(kSize/2)*2)/1024},
        			});
        hge->Gfx_RenderQuad(&q);
        hge->Gfx_EndScene();
    }
    {//resize
    	hge->Gfx_BeginScene(a);
    	hge->Gfx_SetShader(NULL, SHADER_PIXEL);
    	
		q.blend=BLEND_DEFAULT;
    	col = ARGB(255,255,255,255);
    	q.tex = hge->Target_GetTexture(b);
    	fillQuad(q, {
        	            {0,                        0,                        0.5f, col, (float)(kSize/2)/1024,                     (float)(kSize/2)/1024},
        	            {(float)cam->SCREEN_WIDTH, 0,                        0.5f, col, (float)(cam->SCREEN_WIDTH-(kSize/2))/1024, (float)(kSize/2)/1024},
        				{(float)cam->SCREEN_WIDTH, (float)cam->SCREEN_HIGHT, 0.5f, col, (float)(cam->SCREEN_WIDTH-(kSize/2))/1024, (float)(cam->SCREEN_HIGHT-(kSize/2))/1024},
        				{0,                        (float)cam->SCREEN_HIGHT, 0.5f, col, (float)(kSize/2)/1024,                     (float)(cam->SCREEN_HIGHT-(kSize/2))/1024},
        			});
        hge->Gfx_RenderQuad(&q);
        hge->Gfx_EndScene();
	} 
}

void GameState::render(HGE* hge, Camera* cam) {
	{//DEBUG
	 	#ifdef DEBUG                                   
        	HTEXTURE tex = hge->Target_GetTexture(rData.target1);
        	assert(hge->Texture_GetWidth(tex) == 1024);
        	assert(hge->Texture_GetHeight(tex) == 1024);
        	tex = hge->Target_GetTexture(rData.target2);
        	assert(hge->Texture_GetWidth(tex) == 1024);
        	assert(hge->Texture_GetHeight(tex) == 1024);
        #endif
	}

	{//MAKE VIEW-LIGHT TARGET(in target1)
		hge->Gfx_BeginScene(rData.target2);
		hge->Gfx_Clear(0);
		for (LightSource const* ls : env->getLightSources()) {
			rendWhitePolygon(hge, cam, ls->pos, ls->getBounds());		
	   	}
		hge->Gfx_EndScene();

		hge->Gfx_BeginScene(rData.target1);
		hge->Gfx_Clear(0);
		
		rendWhitePolygon(hge, cam, hero->pos, hero->visible);		
		
		q.blend=BLEND_DARKEN;
		col = ARGB(255,255,255,255);
		q.tex = hge->Target_GetTexture(rData.target2);
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
    	rData.blur(hge, cam, rData.target1, rData.target2);
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
		q.tex = hge->Target_GetTexture(rData.target1);
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
    
}



/*
        fillQuad(q, {
        	            {0,                                      0,         0.5f, col, 0.f,                           0.f},
        	            {(float)cam->SCREEN_WIDTH + (kSize/2)*2, 0,         0.5f, col, (float)cam->SCREEN_WIDTH/1024, 0.f},
        				{(float)cam->SCREEN_WIDTH + (kSize/2)*2, (kSize/2), 0.5f, col, (float)cam->SCREEN_WIDTH/1024, 0.f},
        				{0,                                      (kSize/2), 0.5f, col, 0.f,                           0.f},
        			});
        hge->Gfx_RenderQuad(&q);
        
        fillQuad(q, {
        	            {(float)cam->SCREEN_WIDTH + (kSize/2),   0,                                      0.5f, col, (float)cam->SCREEN_WIDTH/1024, 0.f},
        	            {(float)cam->SCREEN_WIDTH + (kSize/2)*2, 0,                                      0.5f, col, (float)cam->SCREEN_WIDTH/1024, 0.f},
        				{(float)cam->SCREEN_WIDTH + (kSize/2)*2, (float)cam->SCREEN_HIGHT + (kSize/2)*2, 0.5f, col, (float)cam->SCREEN_WIDTH/1024, (float)cam->SCREEN_HIGHT/1024},
        				{(float)cam->SCREEN_WIDTH + (kSize/2),   (float)cam->SCREEN_HIGHT + (kSize/2)*2, 0.5f, col, (float)cam->SCREEN_WIDTH/1024, (float)cam->SCREEN_HIGHT/1024},
        			});
        hge->Gfx_RenderQuad(&q);
        
        
        fillQuad(q, {
        	            {0,                                      (float)cam->SCREEN_HIGHT + (kSize/2),         0.5f, col, 0.f,                           (float)cam->SCREEN_HIGHT/1024},
        	            {(float)cam->SCREEN_WIDTH + (kSize/2)*2, (float)cam->SCREEN_HIGHT + (kSize/2),         0.5f, col, (float)cam->SCREEN_WIDTH/1024, (float)cam->SCREEN_HIGHT/1024},
        				{(float)cam->SCREEN_WIDTH + (kSize/2)*2, (float)cam->SCREEN_HIGHT + (kSize/2)*2,       0.5f, col, (float)cam->SCREEN_WIDTH/1024, (float)cam->SCREEN_HIGHT/1024},
        				{0,                                      (float)cam->SCREEN_HIGHT + (kSize/2)*2,       0.5f, col, 0.f,                           (float)cam->SCREEN_HIGHT/1024},
        			});
        hge->Gfx_RenderQuad(&q);
        
        fillQuad(q, {
        	            {0,       0,                                      0.5f, col, 0, 0.f},
        	            {kSize/2, 0,                                      0.5f, col, 0, 0.f},
        				{kSize/2, (float)cam->SCREEN_HIGHT + (kSize/2)*2, 0.5f, col, 0, (float)cam->SCREEN_HIGHT/1024},
        				{0,       (float)cam->SCREEN_HIGHT + (kSize/2)*2, 0.5f, col, 0, (float)cam->SCREEN_HIGHT/1024},
        			});
        hge->Gfx_RenderQuad(&q);
        */
        