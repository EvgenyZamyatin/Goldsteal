#include "../geometry/Geometry.h"
#include "EnvironmentData.h"
#include "../model/Environment.h"
#include "help.h"

inline void Render::EnvironmentData::Layer::get(int i, int j, int& ti, int& tj, int tilesInRow) {
	std::pair<int, int> &p = tcorners[j*tilesInRow + i];
	ti = p.first;
	tj = p.second;
}

void Environment::render(HGE* hge, Camera const* cam) {
	Environment const* env = this;
	Rect c(cam);
	Rect cur;
	static hgeQuad quad;                          
	
	double kx = cam->sWidth()/cam->cWidth();
	double ky = cam->sHight()/cam->cHight();
	
	for (int i = 0; i < env->rData.tilesInRow; ++i) {
		for (int j = 0; j < env->rData.tilesInColumn; ++j) {
			cur.x=i*env->rData.envTileWidth;
			cur.y=j*env->rData.envTileHight;  
			cur.width=env->rData.envTileWidth;
			cur.hight=env->rData.envTileHight;
			if (!intersect(c, cur))
				continue;
			for (Render::EnvironmentData::Layer l : env->rData.layers) {
				int ti, tj;
				l.get(i, j, ti, tj, env->rData.tilesInRow);
				if (ti == -1 && tj == -1)
					continue;
				quad.tex=l.tex;	
				quad.blend = BLEND_DEFAULT_Z;
				hgeU32 col = ARGB(l.alpha, 255, 255, 255);
				fillQuad(quad, {                                                   
								{(float)((cur.x-c.x)*kx),           (float)((cur.y-c.y)*ky),           0.5f, col, ti*1.f/l.texWidth,                           tj*1.f/l.texHight},
								{(float)((cur.x+cur.width-c.x)*kx), (float)((cur.y-c.y)*ky),           0.5f, col, (ti+env->rData.texTileWidth)*1.f/l.texWidth, tj*1.f/l.texHight},
								{(float)((cur.x+cur.width-c.x)*kx), (float)((cur.y+cur.hight-c.y)*ky), 0.5f, col, (ti+env->rData.texTileWidth)*1.f/l.texWidth, (tj+env->rData.texTileHight)*1.f/l.texHight},
								{(float)((cur.x-c.x)*kx),           (float)((cur.y+cur.hight-c.y)*ky), 0.5f, col, ti*1.f/l.texWidth,                           (tj+env->rData.texTileHight)*1.f/l.texHight}
							   });
				                           
				hge->Gfx_RenderQuad(&quad);
			}
		}
	}
}
 
/*int firstcolumn = int( floor(c.x / envData.envTileWidth) );
  int firstrow = int( floor(c.y / envData.envTileHight) );
  int lastcolumn = int( ceil((c.x + cam->cameraWidth) / envData.envTileWidth) );
  int lastrow = int( ceil((c.y + cam->cameraHight) / envData.envTileHight) );*/














