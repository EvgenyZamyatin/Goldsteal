#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H
#include <hge.h>
#include <tinyxml.h>
#include <string>
#include <vector>
#include "../model/GameState.h"

struct LevelLoader {
	LevelLoader(HGE* hge) : hge(hge) {};
	GameState* load(std::string file);
	struct ITileSet {
		virtual ~ITileSet() {}
		virtual hgeQuad get(int index) const = 0;
		virtual int size() const = 0;
	};
	
private:
	HGE* hge;
	std::vector<ITileSet*> tileSets;
};


struct TileSet : LevelLoader::ITileSet {
	TileSet(std::vector<HTEXTURE> textures, int tileWidth, int tileHight) {
		this->textures = textures;
		this->tileWidth = tileWidth;
		this->tileHight = tileHight;
	}
	virtual ~TileSet() {}
	virtual hgeQuad get(int index) {
		hgeQuad ans;
		ans.tex = textures[index];
		ans.v[0].tx=0.0;
		ans.v[0].ty=0.0;

		ans.v[1].tx=1.0;
		ans.v[1].ty=0.0;

		ans.v[2].tx=1.0;
		ans.v[2].ty=1.0;

		ans.v[3].tx=0.0;
		ans.v[3].ty=1.0;

		return ans;
	}
	

private:
	int tileWidth;
	int tileHight;
	std::vector<HTEXTURE> textures;
};

struct TileSetFromImage : LevelLoader::ITileSet {
   	TileSetFromImage(HTEXTURE texture, int width, int hight, 
   						int tileWidth, int tileHight, int margin, int spacing) {
   		tex = texture;    
   		this->width=width;
   		this->hight=hight;        
   		this->tileWidth=tileWidth;
   		this->tileHight=tileHight;
   		this->margin=margin;
   		this->spacing=spacing;
   	}
    virtual ~TileSetFromImage() {}
    virtual hgeQuad get(int index) {
    	int row = index/columns();
    	index %= columns(); 	
    	hgeQuad ans;
    	ans.tex=tex;
    	ans.v[0].tx=(margin+(index-1)*(spacing+tileWidth))*1.0/width;
		ans.v[0].ty=(margin+(row-1)*(spacing+tileHight))*1.0/hight;

		ans.v[1].tx=(margin+(index-1)*(spacing+tileWidth) + tileWidth)*1.0/width;
		ans.v[1].ty=(margin+(row-1)*(spacing+tileHight))*1.0/hight;

		ans.v[2].tx=(margin+(index-1)*(spacing+tileWidth) + tileWidth)*1.0/width;
		ans.v[2].ty=(margin+(row-1)*(spacing+tileHight) + tileHight)*1.0/hight;

    	ans.v[3].tx=(margin+(index-1)*(spacing+tileWidth))*1.0/width;
		ans.v[3].ty=(margin+(row-1)*(spacing+tileHight) + tileHight)*1.0/hight;

    	return ans;
    }
    virtual int size() {
    	return rows() * columns();
    }

private:
	int rows() {
		return ((hight-2*margin+spacing)/(tileHight+spacing));
	}
	int columns() {
		return ((width-2*margin+spacing)/(tileWidth+spacing));;
	}

	HTEXTURE tex;
	int width;
	int hight;
	int tileWidth;
	int tileHight;
	int margin;
	int spacing;
};



#endif