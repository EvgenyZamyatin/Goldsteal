#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <hgeresource.h>
#include <Tmx/Tmx.h.in>
#include "Graph.h"
#include "Vertex.h"
#include "../Geometry.h"
#include "LightSource.h"
#include "../render/RenderData.h"

struct Environment {
    Environment(Tmx::Map const* map, hgeResourceManager* res);

    virtual ~Environment() {
    	delete graph;
    }
	
	bool findPath(Vertex const& a, Vertex const& b, std::vector<Vertex>& out) const;
	void findVisibility(Vertex const& o, geo::Ring<Vertex>& out) const;

    
    void addObject(IObject* obj);
    void addLightSource(LightSource* ls);
	
	int getWidth() const {return width;}
	int getHight() const {return hight;}

	std::vector<IObject*> getObjects() const {return objects;}
	std::vector<LightSource*> getLightSources() const {return lightSources;}

	void frame();
	
	void render(HGE* hge, Camera const* cam);
	
private:
	Graph<Vertex>* graph;
	
	std::vector<Vertex> helpVertices;

	geo::Polygon<geo::Ring<Vertex>> obstructPolygon;
	//geo::Polygon<IObject::Bounds> obstaclePolygon;//inflated

	std::vector<IObject*> objects;
	std::vector<LightSource*> lightSources;
	
	int width;
	int hight;
	
	Render::EnvironmentData rData;
};

#endif
