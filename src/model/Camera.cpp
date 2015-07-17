#include "Camera.h"
#include <vector>
#include <hge.h>

Camera::Camera(GameState* state, IBody* body,
				const Geo::Vector& pos, double width, double hight) {
	this->state = state;
	this->pos = pos;
	this->cameraWidth = width;
	this->cameraHight = hight;
	this->body=body;
}

Geo::Vector Camera::getPos() {
	if (freeMode)
		return pos;
	Geo::Vector ans(0,0);
	for (Geo::Vector v : body->getPosition().points)
		ans += v;
	ans /= body->getPosition().size();
	return pos = ans;
}

void Camera::make(hgeQuad& quad, Geo::Vector c, double width, double hight,
			double col, double z) {
	quad.v[0].x=0, quad.v[0].y=0;
	quad.v[1].x=0+state->screenWidth, quad.v[1].y=0;
	quad.v[2].x=0+state->screenWidth, quad.v[2].y=0+state->screenHight;
	quad.v[3].x=0, quad.v[3].y=0+state->screenHight;

	quad.v[0].tx=(c.x-width/2)/state->env->getWidth(), quad.v[0].ty=(c.y-hight/2)/state->env->getHight();
	quad.v[1].tx=(c.x+width/2)/state->env->getWidth(), quad.v[1].ty=(c.y-hight/2)/state->env->getHight();
	quad.v[2].tx=(c.x+width/2)/state->env->getWidth(), quad.v[2].ty=(c.y+hight/2)/state->env->getHight();
	quad.v[3].tx=(c.x-width/2)/state->env->getWidth(), quad.v[3].ty=(c.y+hight/2)/state->env->getHight();

	for (int i = 0; i < 4; ++i) {
		quad.v[i].col=col, quad.v[i].z=z;
	}
	quad.blend=BLEND_DEFAULT_Z;	
}

void Camera::view(HGE* hge) {
	hgeQuad quad;
	hgeTriple trip;
	HTEXTURE stat = state->env->compile(hge);
	quad.tex=stat;
	make(quad, pos, cameraWidth, cameraHight, ARGB(200, 255, 255, 255), 0.5);
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	hge->Gfx_RenderQuad(&quad);
	if (body != NULL) {
    	/*Geo::Vector pos = getPos();
    	Geo::Polygon visibile = Geo::visibilityPolygon(pos, state->env->getObjects(), state->width(), state->hight());
    	Geo::Vector orientation = body->getOrientation();
    	Geo::Vector dirA = orientation;	
    	Geo::Vector dirB = orientation;     
    	dirA.rotate(body->getViewAngle()/2);
    	dirB.rotate(-body->getViewAngle()/2);
    	dirA *= 100000;
    	ditB *= 100000
    	Geo::Polygon vp({pos, dirA, dirB});
    	Geo::Polygon out;
    	Geo::intersect(visible, vp, out);
    	for (int i = 0; i < (int)out.points.size(); ++i) {
    		Geo::Vector& a = out[i];
    		Geo::Vector& b = out[(i+1)%out.size()];
			trip.v[0].x=pos.x, trip.v[0].y=pos.y, trip.v[0].col=ARGB(255,255,255,255), trip.v[0].z=0.5;
           	trip.v[1].x=a.x, trip.v[1].y=a.y, trip.v[1].col=ARGB(255,255,255,255), trip.v[1].z=0.5;
			trip.v[2].x=b.x, trip.v[2].y=b.y, trip.v[2].col=ARGB(255,255,255,255), trip.v[2].z=0.5;
			trip.tex			
    	}*/
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



