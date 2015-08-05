#ifndef ENVIRONMENTDATA_H
#define ENVIRONMENTDATA_H
#include <vector>
#include <hge.h>

namespace Render {
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
}
#endif