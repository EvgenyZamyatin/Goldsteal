#ifndef RENDERDATA_H
#define RENDERDATA_H
#include <vector>
#include <hge.h>
#include <hgeanim.h>


struct Camera;
namespace Render {
	struct GameStateData {
		static const int MAX_KERNEL_SIZE = 16;
		HTARGET target1;
		HTARGET target2;
		HTARGET target3;

		HSHADER blurShader;
		int kSize;
		float kernel[MAX_KERNEL_SIZE];
		float dirX[2];
		float dirY[2];
		float sigma;

		float gaussian(float x, float sigma) {
			return exp(-(x*x)/(2*sigma*sigma))/sqrt(2*M_PI*sigma*sigma);
		}
		void blur(HGE* hge, Camera* cam, HTARGET a, HTARGET b);

	};

	struct BodyData {
		std::vector<hgeAnimation> anims;
		int lastState;
		int width;
		int hight;
		hgeAnimation& get(int state) {
			return anims[state];
		}
	};

	struct EnvironmentData {
    	int tilesInRow;
    	int tilesInColumn;

    	int texTileWidth;
    	int texTileHight;

    	int envTileWidth;
    	int envTileHight;
    	
    	struct Layer {
    		HTEXTURE tex;
    		int texWidth;
    		int texHight;
    		int alpha;
    		inline void get(int i, int j, int& ti, int& tj, int tilesInRow);
    		std::vector<std::pair<int, int>> tcorners;
    	};
    	std::vector<Layer> layers;
    };

    struct SimpleObjectTex {
		SimpleObjectTex(hgeSprite const& s, Vector const& v, 
			float rot, int width, int hight) : spr(s), pos(v), rot(rot), width(width), hight(hight) {}

		hgeSprite spr;
		Vector pos;//relative
		float rot;
		int width;
		int hight;
	};
	typedef std::vector<SimpleObjectTex> SimpleObjectData;

}


#endif