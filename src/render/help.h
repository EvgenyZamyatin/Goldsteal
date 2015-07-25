#include <hge.h>
#include <vector>
#include "../geometry/Geometry.h"
#include "../model/Camera.h"

struct Rect {
	Rect(){}
	Rect(double x, double y, double width, double hight) : x(x), y(y), width(width), hight(hight){}
	Rect(Camera const* cam) {
		width = cam->CAMERA_WIDTH;
		hight = cam->CAMERA_HIGHT;      
		x = cam->getPos().x-cam->CAMERA_WIDTH/2;
		y = cam->getPos().y-cam->CAMERA_HIGHT/2;
	}
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

inline void fillQuad(hgeQuad& quad, std::vector<hgeVertex> const& v) {
	for (int i = 0; i < 4; ++i)
		quad.v[i]=v[i];
}

inline void fillTriple(hgeTriple& trip, std::vector<hgeVertex> const& v) {
	for (int i = 0; i < 3; ++i)
		trip.v[i]=v[i];
}

inline Rect boundingBox(Geo::Polygon const& pol) {
	double minX=pol[0].x, minY=pol[0].y, maxX=pol[0].x, maxY=pol[0].y;
	for (Geo::Vector const& p : pol.points) {
		minX = std::min(p.x,minX);
		minY = std::min(p.y,minY);
		maxX = std::max(p.x,maxX);
		maxY = std::max(p.y,maxY);
	}
	return Rect(minX, minY, maxX-minX, maxY-minY);
}

