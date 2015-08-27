#include "../Geometry.h"
#include "RenderData.h"
#include "../model/Environment.h"
#include "help.h"

inline void Render::EnvironmentData::Layer::get(int i, int j, int& ti, int& tj, int tilesInRow) {
	std::pair<int, int> &p = tcorners.at(j*tilesInRow + i);
	ti = p.first;
	tj = p.second;
}

void Environment::render(HGE* hge, Camera const* cam) {
	Environment const* env = this;
	Box c(cam->getPos().x-cam->CAMERA_WIDTH/2, cam->getPos().y-cam->CAMERA_HIGHT/2, cam->CAMERA_WIDTH, cam->CAMERA_HIGHT);
	Box cur;
	static hgeQuad quad;                          
	
	float kx = cam->KX;
	float ky = cam->KY;
	
	int firstcolumn = std::max(0, int( floor(c.minCor.x*1.0 / env->rData.envTileWidth) ));
    int firstrow = std::max(0,int( floor(c.minCor.y*1.0 / env->rData.envTileHight) ));
    int lastcolumn = std::min(rData.tilesInRow, int( ceil(c.maxCor.x*1.0 / env->rData.envTileWidth) ));
    int lastrow = std::min(rData.tilesInColumn, int( ceil(c.maxCor.y*1.0 / env->rData.envTileHight) ));
    
	for (int i = firstcolumn; i < lastcolumn; ++i) {
		for (int j = firstrow; j < lastrow; ++j) {
			cur.minCor.x=i*env->rData.envTileWidth;
			cur.minCor.y=j*env->rData.envTileHight;  
			cur.maxCor.x=cur.minCor.x + env->rData.envTileWidth;
			cur.maxCor.y=cur.minCor.y + env->rData.envTileHight;
			for (Render::EnvironmentData::Layer l : env->rData.layers) {
				int ti, tj;
				l.get(i, j, ti, tj, env->rData.tilesInRow);
				if (ti == -1 && tj == -1)
					continue;
				quad.tex=l.tex;	
				quad.blend = BLEND_DEFAULT;
				hgeU32 col = ARGB(l.alpha, 255, 255, 255);
				fillQuad(quad, {                                                   
								{kx*(cur.minCor.x-c.minCor.x), ky*(cur.minCor.y-c.minCor.y), 0.5f, col, ti*1.f/l.texWidth,                           tj*1.f/l.texHight},
								{kx*(cur.maxCor.x-c.minCor.x), ky*(cur.minCor.y-c.minCor.y), 0.5f, col, (ti+env->rData.texTileWidth)*1.f/l.texWidth, tj*1.f/l.texHight},
								{kx*(cur.maxCor.x-c.minCor.x), ky*(cur.maxCor.y-c.minCor.y), 0.5f, col, (ti+env->rData.texTileWidth)*1.f/l.texWidth, (tj+env->rData.texTileHight)*1.f/l.texHight},
								{kx*(cur.minCor.x-c.minCor.x), ky*(cur.maxCor.y-c.minCor.y), 0.5f, col, ti*1.f/l.texWidth,                           (tj+env->rData.texTileHight)*1.f/l.texHight}
							   });
				                           
				hge->Gfx_RenderQuad(&quad);
			}
		}
	}

	for (Vertex const& v : graph->getNodes()) {
		float x = v.x, y = v.y;
		cam->convertGS(x, y);
		hgeU32 col = 0xFFFF0000;
		fillQuad(quad, {                                                   
						{x-5, y-5, 0.5f, col, 1.f, 1.f},
						{x+5, y-5, 0.5f, col, 1.f, 1.f},
						{x+5, y+5, 0.5f, col, 1.f, 1.f},
						{x-5, y+5, 0.5f, col, 1.f, 1.f}
					   });
		hge->Gfx_RenderQuad(&quad);		                           
	}

	for (Vertex const& v : graph->getNodes()) {
		for (Vertex const& v1 : graph->getAdjacent(v)) {
			float x1 = v.x, y1 = v.y;
			float x2 = v1.x, y2 = v1.y;
			
			cam->convertGS(x1, y1);
			cam->convertGS(x2, y2);
			hge->Gfx_RenderLine(x1, y1, x2, y2, 0xFFFF0000);
		}
	}

}
 











