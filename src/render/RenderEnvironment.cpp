#include "../geometry/Geometry.h"
#include "EnvironmentData.h"
#include "../model/Environment.h"
#include "help.h"

inline void Render::EnvironmentData::Layer::get(int i, int j, int& ti, int& tj, int tilesInRow) {
	std::pair<int, int> &p = tcorners.at(j*tilesInRow + i);
	ti = p.first;
	tj = p.second;
}

void Environment::render(HGE* hge, Camera const* cam) {
	Environment const* env = this;
	Geo::Box c(cam->getPos().x-cam->CAMERA_WIDTH/2, cam->getPos().y-cam->CAMERA_HIGHT/2, cam->CAMERA_WIDTH, cam->CAMERA_HIGHT);
	Geo::Box cur;
	static hgeQuad quad;                          
	
	float kx = cam->KX;
	float ky = cam->KY;
	
	int firstcolumn = std::max(0, int( floor(c.a.x*1.0 / env->rData.envTileWidth) ));
    int firstrow = std::max(0,int( floor(c.a.y*1.0 / env->rData.envTileHight) ));
    int lastcolumn = std::min(rData.tilesInRow, int( ceil(c.b.x*1.0 / env->rData.envTileWidth) ));
    int lastrow = std::min(rData.tilesInColumn, int( ceil(c.b.y*1.0 / env->rData.envTileHight) ));
    
	for (int i = firstcolumn; i < lastcolumn; ++i) {
		for (int j = firstrow; j < lastrow; ++j) {
			cur.a.x=i*env->rData.envTileWidth;
			cur.a.y=j*env->rData.envTileHight;  
			cur.b.x=cur.a.x + env->rData.envTileWidth;
			cur.b.y=cur.a.y + env->rData.envTileHight;
			for (Render::EnvironmentData::Layer l : env->rData.layers) {
				int ti, tj;
				l.get(i, j, ti, tj, env->rData.tilesInRow);
				if (ti == -1 && tj == -1)
					continue;
				quad.tex=l.tex;	
				quad.blend = BLEND_DEFAULT;
				hgeU32 col = ARGB(l.alpha, 255, 255, 255);
				fillQuad(quad, {                                                   
								{kx*(cur.a.x-c.a.x), ky*(cur.a.y-c.a.y), 0.5f, col, ti*1.f/l.texWidth,                           tj*1.f/l.texHight},
								{kx*(cur.b.x-c.a.x), ky*(cur.a.y-c.a.y), 0.5f, col, (ti+env->rData.texTileWidth)*1.f/l.texWidth, tj*1.f/l.texHight},
								{kx*(cur.b.x-c.a.x), ky*(cur.b.y-c.a.y), 0.5f, col, (ti+env->rData.texTileWidth)*1.f/l.texWidth, (tj+env->rData.texTileHight)*1.f/l.texHight},
								{kx*(cur.a.x-c.a.x), ky*(cur.b.y-c.a.y), 0.5f, col, ti*1.f/l.texWidth,                           (tj+env->rData.texTileHight)*1.f/l.texHight}
							   });
				                           
				hge->Gfx_RenderQuad(&quad);
			}
		}
	}
	//std::cerr << "ELAPSED: " << (clock() - start)/CLOCKS_PER_SEC << "\n";
}
 
/*int firstcolumn = int( floor(c.x / envData.envTileWidth) );
  int firstrow = int( floor(c.y / envData.envTileHight) );
  int lastcolumn = int( ceil((c.x + cam->cameraWidth) / envData.envTileWidth) );
  int lastrow = int( ceil((c.y + cam->cameraHight) / envData.envTileHight) );*/














