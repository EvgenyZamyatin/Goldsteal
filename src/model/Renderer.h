#ifndef RENDERER_H
#define RENDERER_H
#include <hge.h>
#include "Environment.h"
#include "Camera.h"

struct Renderer {
	void show(HGE* hge, Environment* env, Camera* cam);
	friend struct LevelLoader;
private:	
	struct EnvironmentRenderData {
		int tilesInRow;
		int tilesInColumn;

		int texTileWidth;
		int texTileHight;

		double envTileWidth;
		double envTileHight;
		
		struct Layer {
			HTEXTURE tex;
			int texWidth;
			int texHight;
			int alpha;
			inline void get(int i, int j, int& ti, int& tj, int tilesInRow);
			std::vector<std::pair<int, int>> tcorners;
		};
		std::vector<Layer> layers;
	} envData;
};
#endif
