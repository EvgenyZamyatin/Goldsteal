#include "Camera.h"
#include <vector>
#include <hge.h>    

Camera::Camera(GameState const* state, Geo::Vector const& pos, 
				double cameraWidth, double cameraHight, double screenWidth, double screenHight,
				double radius) {
	body = NULL;
	this->state = state;
	this->pos = pos;
	this->cameraWidth = cameraWidth;
	this->cameraHight = cameraHight;
	this->body=body;                
	this->screenWidth = screenWidth;
	this->screenHight = screenHight;
	this->radius = radius;
}

inline void fillQuad(hgeQuad& quad, std::vector<hgeVertex> v) {
	for (int i = 0; i < 4; ++i)
		quad.v[i]=v[i];
}

inline void fillTriple(hgeTriple& trip, std::vector<hgeVertex> v) {
	for (int i = 0; i < 3; ++i)
		trip.v[i]=v[i];
}

void Camera::view(HGE* hge, hgeResourceManager* res) {
	//===onEnvTarget
	
	HTARGET envTarget = res->getTarget("envTarget");
	HTEXTURE envTexture = 0;
	bool ok = state->env->clear;
	for (IObject* obj : state->env->objs)
		ok &= obj->clear;
	if (!ok) {
		hge->Gfx_BeginScene(envTarget);
		hge->Gfx_Clear(0);
		state->env->render(hge, res);
		for (IObject* obj : state->env->objs)
			obj->render(hge, res);
		hge->Gfx_EndScene();
	}
	envTexture = hge->Target_GetTexture(envTarget);
	//===onDynamicTarget

	HTARGET dynTarget = res->getTarget("dynamicTarget");
	HTEXTURE dynTexture = 0;
	hge->Gfx_BeginScene(dynTarget);
	hge->Gfx_Clear(0);
	for (IBody* body : state->bodies)
		body->render(hge, res);
	hge->Gfx_EndScene();
	dynTexture = hge->Target_GetTexture(dynTarget);
	//===onScreen

	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	Geo::Vector crnr = pos - Geo::Vector(cameraWidth/2, cameraHight/2);
	double ew = state->env->widht;
	double eh = state->env->widht;
	
	static hgeQuad quad;	
	static hgeTriple trip;

	hgeU32 col = ARGB(255,255,255,255);
	quad.blend = BLEND_DEFAULT_Z;
	quad.tex = envTexture;
	fillQuad(quad, {
					{0,0,0.5,col, pos.x/ew, pos.y/eh},
					{screenWidth, 0, 0.5, col, (pos.x+cameraWidth)/ew, pos.y/eh},
					{screenWidth, screenHight, 0.5, col, (pos.x+cameraWidth)/ew, (pos.y+cameraHight)/eh},
					{0, screenHight, 0.5, col, pos.x/ew, (pos.y+cameraHight)/eh},		
				   });
	hge->Gfx_RenderQuad(&quad);
	
	Geo::Polygon const& vis = state->hero->visible;
	Geo::Vector const& heroPos = state->hero->pos;
	double kx = screenWidth/cameraWidth;
	double ky = screenHight/cameraHight;

	for (int i = 0; i < vis.size(); ++i) {
    	Geo::Vector const& a = vis[i];
    	Geo::Vector const& b = vis[(i+1)%out.size()];
    	hgeU32 col = ARGB(255,255,255,255);
    	trip.blend = BLEND_DEFAUT_Z;
    	trip.tex=dynTexture;
    	fillTriple(trip, {
    					  {(heroPos.x-crnr.x)*kx, (heroPos.y-crnr.y)*ky, 0.5, col, heroPos.x/ew, heroPos.y/eh},
    					  {(a.x-crnr.x)*kx, (a.y-crnr.y)*ky, 0.5, col, a.x/ew, a.y/eh},
    					  {(b.x-crnr.x)*kx, (b.y-crnr.y)*ky, 0.5, col, b.x/ew, b.y/eh} 
    					 });
    	hge->Gfx_RenderTriple(&trip);
    }
	hge->Gfx_EndScene();
}

void Camera::frame(Geo::Vector const& mouse, bool freeMode) {
	Geo::Vector tmp;
	if (freeMode) {
		tmp = (mouse-Geo::Vector(screenWidth/2, screenHight/2));
	} else {
		tmp = (mouse - state->hero->pos);
	}
	if (greater(tmp.len(), radius)) 
		move(tmp/radius);
}
	
/*
Geo::Vector Camera::getPos() {
	if (freeMode)
		return pos;
	Geo::Vector ans(0,0);
	for (Geo::Vector v : body->getPosition().points)
		ans += v;
	ans /= body->getPosition().size();
	
	ans.x = std::max(ans.x, cameraWidth/2);
    ans.x = std::min(ans.x, state->env->getWidth() - cameraWidth/2);

	ans.y = std::max(ans.y, cameraHight/2);
    ans.y = std::min(ans.y, state->env->getHight() - cameraHight/2);

	return pos = ans;
}

void Camera::make(hgeQuad& quad, Geo::Vector c, double width, double hight,
			double col, double z) {
	quad.v[0].x=0, quad.v[0].y=0;
	quad.v[1].x=screenWidth, quad.v[1].y=0;
	quad.v[2].x=screenWidth, quad.v[2].y=screenHight;
	quad.v[3].x=0, quad.v[3].y=screenHight;

	quad.v[0].tx=(c.x-width/2)/state->env->getWidth(), quad.v[0].ty=(c.y-hight/2)/state->env->getHight();
	quad.v[1].tx=(c.x+width/2)/state->env->getWidth(), quad.v[1].ty=(c.y-hight/2)/state->env->getHight();
	quad.v[2].tx=(c.x+width/2)/state->env->getWidth(), quad.v[2].ty=(c.y+hight/2)/state->env->getHight();
	quad.v[3].tx=(c.x-width/2)/state->env->getWidth(), quad.v[3].ty=(c.y+hight/2)/state->env->getHight();

	for (int i = 0; i < 4; ++i) {
		quad.v[i].col=col, quad.v[i].z=z;
	}
	quad.blend=BLEND_DEFAULT_Z;	
}

void Camera::fill(hgeQuad& quad, Geo::Vector a, Geo::Vector b, Geo::Vector c, Geo::Vector d) {
	quad.v[0].x=a.x, quad.v[0].y=a.y;
	quad.v[1].x=b.x, quad.v[1].y=b.y;	
	quad.v[2].x=c.x, quad.v[2].y=c.y;
	quad.v[3].x=d.x, quad.v[3].y=d.y;
}

void Camera::fill(hgeTriple& trip, Geo::Vector a, Geo::Vector b, Geo::Vector c) {
	trip.v[0].x=a.x, trip.v[0].y=a.y;
	trip.v[1].x=b.x, trip.v[1].y=b.y;	
	trip.v[2].x=c.x, trip.v[2].y=c.y;
}



void Camera::view(HGE* hge) {
	hgeQuad quad;
	hgeTriple trip;
	HTEXTURE stat = state->env->compile(hge);
	
	if (target == 0)
		target = hge->Target_Create(state->env->getWidth(), state->env->getHight(), true);			    	
    hge->Gfx_BeginScene(target);
	hge->Gfx_Clear(0);
	quad.tex=stat;
	quad.v[0].x=0, quad.v[0].y=0, quad.v[0].tx = 0, quad.v[0].ty = 0, quad.v[0].col = ARGB(255,255,255,255), quad.v[0].z=0.5;
	quad.v[1].x=state->env->getWidth(), quad.v[1].y=0, quad.v[1].tx = 1, quad.v[1].ty = 0, quad.v[1].col = ARGB(255,255,255,255), quad.v[1].z=0.5;
	quad.v[2].x=state->env->getWidth(), quad.v[2].y=state->env->getHight(), quad.v[2].tx = 1, quad.v[2].ty = 1, quad.v[2].col = ARGB(255,255,255,255), quad.v[2].z=0.5;
	quad.v[3].x=0, quad.v[3].y=state->env->getHight(), quad.v[3].tx = 0, quad.v[3].ty = 1, quad.v[3].col = ARGB(255,255,255,255), quad.v[3].z=0.5;
	quad.blend=BLEND_DEFAULT_Z;
	hge->Gfx_RenderQuad(&quad);
	for (IBody* body : state->getBodies())
		body->render(hge);
	hge->Gfx_EndScene();
	HTEXTURE full = hge->Target_GetTexture(target);		

	quad.tex=stat;
	make(quad, pos, cameraWidth, cameraHight, ARGB(150, 255, 255, 255), 0.5);
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	hge->Gfx_RenderQuad(&quad);
	
	if (body != NULL) {
		Geo::Vector bpos = body->getPosition().center();
    	Geo::Vector cpos = getPos();
    	
    	//should be in body class.
    	std::vector<Geo::Polygon> polys;
    	for (IObject* o : state->env->getObjects())
    		polys.push_back(o->getPosition());
    	Geo::Polygon out = Geo::visibilityPolygon(bpos, polys, state->env->getWidth(), state->env->getHight());
    	
    	double kx = screenWidth/cameraWidth;
		double ky = screenHight/cameraHight;
		double ew = state->env->getWidth();
		double eh = state->env->getHight();
    	
    	Geo::Vector v(cpos.x-cameraWidth/2, cpos.y-cameraHight/2);
		
		for (int i = 0; i < out.size(); ++i) {
    		Geo::Vector a = out[i];
    		Geo::Vector b = out[(i+1)%out.size()];
    		trip.tex=full;
    		for (int j = 0; j < 3; ++j)
    			trip.v[j].z = 0.4, trip.v[j].col=ARGB(255,255,255,255);	
    		trip.v[0].tx=bpos.x/ew, trip.v[0].ty=bpos.y/eh;
    		trip.v[1].tx=a.x/ew, trip.v[1].ty=a.y/eh;
    		trip.v[2].tx=b.x/ew, trip.v[2].ty=b.y/eh;
    		trip.blend=BLEND_DEFAULT_Z;
    		
    		trip.v[0].x=(bpos.x-v.x)*kx, trip.v[0].y=(bpos.y-v.y)*ky;
    		trip.v[1].x=(a.x-v.x)*kx, trip.v[1].y=(a.y-v.y)*ky;
    		trip.v[2].x=(b.x-v.x)*kx, trip.v[2].y=(b.y-v.y)*ky;
    		
    		hge->Gfx_RenderTriple(&trip);
    	}
    	
    } 
	hge->Gfx_EndScene();	
}

void Camera::setFreeMode(bool freeMode) {
	this->freeMode=freeMode;
}                       


void Camera::frame(HGE* hge) {
	hgeInputEvent evt;
	while (hge->Input_GetEvent(&evt)) {
  		switch (evt.type) {
      		case INPUT_KEYDOWN:
        		if (evt.key==HGEK_SPACE) {
        			freeMode ^= true;	
        		}
        		break;
   		}
	}
	if (!freeMode)
		return;
	if (hge->Input_GetKeyState(HGEK_W)) 
        pos.y -= 5;
    if (hge->Input_GetKeyState(HGEK_S)) 
       	pos.y += 5;
    if (hge->Input_GetKeyState(HGEK_D)) 
        pos.x += 5;
    if (hge->Input_GetKeyState(HGEK_A)) 
        pos.x -= 5;                        
    pos.x = std::max(pos.x, cameraWidth/2);
    pos.x = std::min(pos.x, state->env->getWidth() - cameraWidth/2);

	pos.y = std::max(pos.y, cameraHight/2);
    pos.y = std::min(pos.y, state->env->getHight() - cameraHight/2);

}
*/



/*Geo::Vector pos = getPos();
		Geo::Vector v(pos.x-cameraWidth/2, pos.y-cameraHight/2);
		double kx = screenWidth/cameraWidth;
		double ky = screenHight/cameraHight;
   		double ew = state->env->getWidth();
   		double eh = state->env->getHight();
    	quad.tex = full;
    	quad.blend=BLEND_DEFAULT_Z;  
    	quad.v[0].tx=0, quad.v[0].ty=0;
    	quad.v[1].tx=1, quad.v[1].ty=0;
    	quad.v[2].tx=1, quad.v[2].ty=1;
    	quad.v[3].tx=0, quad.v[3].ty=1;
    	for (int i = 0; i < 4; i++)
    		quad.v[i].z = 0.4, quad.v[i].col=ARGB(255,255,255,255);
    	quad.v[0].x=(0-v.x)*kx, quad.v[0].y=(0-v.y)*ky;
    	quad.v[1].x=(ew-v.x)*kx, quad.v[1].y=(0-v.y)*ky;
    	quad.v[2].x=(ew-v.x)*kx, quad.v[2].y=(eh-v.y)*ky;
    	quad.v[3].x=(0-v.x)*kx, quad.v[3].y=(eh-v.y)*ky;
    	hge->Gfx_RenderQuad(&quad);*/
    	
    	
    	