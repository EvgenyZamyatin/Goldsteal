#include "Renderer.h"
#include "../geometry/Geometry.h"

inline void Renderer::EnvironmentRenderData::Layer::get(int i, int j, int& ti, int& tj, int tilesInRow) {
	std::pair<int, int> &p = tcorners[j*tilesInRow + i];
	ti = p.first;
	tj = p.second;
}

struct Rect {
	Rect(){}
	Rect(double x, double y, double width, double hight) : x(x), y(y), width(width), hight(hight){}
	double x, y, width, hight;
};                            
const int dx[] = {0, 1, 1, 0};
const int dy[] = {0, 0, 1, 1};

inline bool inside(double x, double y, Rect const& r) {
	return Geo::greater(x, r.x) && Geo::less(x, r.x+r.width) &&
			Geo::greater(y, r.y) && Geo::less(y, r.y+r.hight); 
}

inline bool intersect(Rect const& a, Rect const& b) {
	for (int it = 0; it < 4; ++it) { 
		double x = a.x+dx[it]*a.width;
		double y = a.y+dy[it]*a.hight;
		if (inside(x,y,b))
			return true;
		x = b.x+dx[it]*b.width;
		y = b.y+dy[it]*b.hight;						
		if (inside(x,y,a))
			return true;
	}
	return false;
}

inline void fillQuad(hgeQuad& quad, std::vector<hgeVertex> v) {
	for (int i = 0; i < 4; ++i)
		quad.v[i]=v[i];
}

inline void fillTriple(hgeTriple& trip, std::vector<hgeVertex> v) {
	for (int i = 0; i < 3; ++i)
		trip.v[i]=v[i];
}

void Renderer::show(HGE* hge, Environment* env, Camera* cam) {
	Rect c(cam->pos.x - cam->cameraWidth/2, cam->pos.y - cam->cameraHight/2, cam->cameraWidth, cam->cameraHight);
	Rect cur;
	static hgeQuad quad;                          
	
	double kx = cam->screenWidth/cam->cameraWidth;
	double ky = cam->screenHight/cam->cameraHight;
	
	/*int firstcolumn = int( floor(c.x / envData.envTileWidth) );
    int firstrow = int( floor(c.y / envData.envTileHight) );
    int lastcolumn = int( ceil((c.x + cam->cameraWidth) / envData.envTileWidth) );
    int lastrow = int( ceil((c.y + cam->cameraHight) / envData.envTileHight) );*/


	for (int i = 0; i < envData.tilesInRow; ++i) {
		for (int j = 0; j < envData.tilesInColumn; ++j) {
			cur.x=i*envData.envTileWidth;
			cur.y=j*envData.envTileHight;  
			cur.width=envData.envTileWidth;
			cur.hight=envData.envTileHight;
			if (!intersect(c, cur))
				continue;
			for (Renderer::EnvironmentRenderData::Layer l : envData.layers) {
				int ti, tj;
				l.get(i, j, ti, tj, envData.tilesInRow);
				if (ti == -1 && tj == -1)
					continue;
				quad.tex=l.tex;	
				quad.blend = BLEND_DEFAULT_Z;
				hgeU32 col = ARGB(255, 255, 255, 255);
				fillQuad(quad, {                                                   
								{(float)((cur.x-c.x)*kx),           (float)((cur.y-c.y)*ky),           0.5f, col, ti*1.f/l.texWidth,                        tj*1.f/l.texHight},
								{(float)((cur.x+cur.width-c.x)*kx), (float)((cur.y-c.y)*ky),           0.5f, col, (ti+envData.texTileWidth)*1.f/l.texWidth, tj*1.f/l.texHight},
								{(float)((cur.x+cur.width-c.x)*kx), (float)((cur.y+cur.hight-c.y)*ky), 0.5f, col, (ti+envData.texTileWidth)*1.f/l.texWidth, (tj+envData.texTileHight)*1.f/l.texHight},
								{(float)((cur.x-c.x)*kx),           (float)((cur.y+cur.hight-c.y)*ky), 0.5f, col, ti*1.f/l.texWidth,                        (tj+envData.texTileHight)*1.f/l.texHight}
							   });
				                           
				hge->Gfx_RenderQuad(&quad);
			}
		}
	}
}

 
void Environment::render(HGE* hge, Renderer* r, Camera* cam) {r->show(hge, this, cam);} 













