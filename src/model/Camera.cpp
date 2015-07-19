#include "Camera.h"
#include <vector>
#include <hge.h>    

Camera::Camera(GameState* state, IBody* body,
				const Geo::Vector& pos, double width, double hight, double screenWidth, double screenHight) {
	this->state = state;
	this->pos = pos;
	this->cameraWidth = width;
	this->cameraHight = hight;
	this->body=body;                
	this->screenWidth = screenWidth;
	this->screenHight = screenHight;
}

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
    	
    	
    	