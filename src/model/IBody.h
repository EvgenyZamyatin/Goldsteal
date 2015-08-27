#ifndef IBODY_H
#define IBODY_H

#include <tmx/Tmx.h.in>
#include <hgeresource.h>
#include "../Geometry.h"
#include "Vertex.h"
#include "../render/RenderData.h"
#include "IRenderable.h"

struct GameState;
struct IBrain;
struct BrainPlayerInput;
struct BrainFollow;

struct IBody : IRenderable {                
	static const int STATE_IDLE = 0;
	static const int STATE_WALK = 1;
	
	static const int MAX_SPEED;
	static const int ACCELERATION;

	IBody(Tmx::Object const* obj, hgeResourceManager* res);
	
	virtual ~IBody(){};                      
	
	virtual void render(HGE* hge, Camera const* cam);
    
	void setGameState(GameState* state) {this->state=state;}
	
	Vertex getDir() const {return dir;}
	
	Vertex getPosition() const {return pos;}

	void frame();
    
    friend struct GameState;
    friend struct BrainPlayerInput;
	friend struct BrainFollow;


private:
	Render::BodyData rData;

	IBrain* brain;
	
	int moveState=0;
	
	int radius;

	int radius2;

	Vertex pos;
	
	Vector velocity;
	
	geo::Ring<Vertex> visible;
	
	Vector dir;	
	
	GameState* state;
};

#endif



