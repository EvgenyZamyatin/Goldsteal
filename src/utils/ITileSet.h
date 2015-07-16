#ifndef ITILESET_H
#define ITILESET_H
#include <vector>
#include <hge.h>

struct ITileSet {
	virtual ~ITileSet() {}
	virtual hgeQuad get(int index) const = 0;
	virtual int size() const = 0;
	int tileWidth;
	int tileHight;
};
	
struct TileSet : ITileSet {
	TileSet(std::vector<HTEXTURE> textures, int tileWidth, int tileHight) {
		this->textures = textures;
		this->tileWidth = tileWidth;
		this->tileHight = tileHight;
	}
	virtual ~TileSet() {}
	virtual hgeQuad get(int index) const {
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
		for (int i = 0; i < 4; ++i)
			ans.v[i].col=ARGB(255,255,255,255);
		ans.blend=BLEND_ALPHAADD | BLEND_COLORMUL | BLEND_ZWRITE;


		return ans;
	}

	virtual int size() const {
		return textures.size();
	}
	

private:
	std::vector<HTEXTURE> textures;
};

struct TileSetFromImage : ITileSet {
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
    virtual hgeQuad get(int index) const {
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
		ans.blend=BLEND_ALPHAADD | BLEND_COLORMUL | BLEND_ZWRITE;
		for (int i = 0; i < 4; ++i)
			ans.v[i].col=ARGB(255,255,255,255);
    	return ans;
    }
    virtual int size() const {
    	return rows() * columns();
    }

private:
	int rows() const {
		return ((hight-2*margin+spacing)/(tileHight+spacing));
	}
	int columns() const {
		return ((width-2*margin+spacing)/(tileWidth+spacing));;
	}

	HTEXTURE tex;
	int width;
	int hight;
	int margin;
	int spacing;
};
#endif


