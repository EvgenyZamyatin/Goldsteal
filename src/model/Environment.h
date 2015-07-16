#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "IObject.h"
#include "IRenderable.h"
#include "../utils/ITileSet.h"
#include <vector>
#include <hge.h>


struct Environment : IRenderable {
	
	Environment(int width, int hight, 
				int tileWidth, int tileHight,
				std::vector<int> grids, ITileSet* tiles) {
		this->width=width;
		this->hight=hight;
		this->tileWidth=tileWidth;
		this->tileHight=tileHight;
		this->gridIndxs=grids;
		this->tiles=tiles;			
	}              
	
	//Gfx_BeginScene must be called on tar.	
	virtual void render(HGE* h);

	//Begins scene.
	HTARGET compile(HGE* hge);	
	
	std::vector<IObject*> getObjects() const {return objs;}
	void addObject(IObject* obj) {objs.push_back(obj);}

private:
	std::vector<IObject*> objs;	
	int width;
	int hight;
	int tileWidth;
	int tileHight;
	std::vector<int> gridIndxs;
	ITileSet* tiles;
	HTARGET compiled=0;
};
#endif