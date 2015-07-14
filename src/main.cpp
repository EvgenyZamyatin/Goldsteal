#include "hge.h"
#include "hgefont.h"
#include "geometry/Geometry.h"
#include "geometry/Boosting.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define WIDTH 800
#define HIGHT 600

const double dx = 1;
const double dy = 1;
const double viewAngle = M_PI/4;
const Geo::Polygon boundsPolygon(std::vector<Geo::Vector>({{0,0}, {0,HIGHT}, {WIDTH,HIGHT}, {WIDTH,0}}));

HGE *hge = 0;
hgeTriple quad;
hgeQuad backQuad;
hgeFont* fnt;

std::vector<Geo::Polygon> polygons;
Geo::Vector viewPoint(WIDTH*1.0/2, HIGHT*1.0/2);
Geo::Vector direction(0, -1);
bool createPolyClicked=false;
Geo::Polygon buffer;
Geo::Polygon visiblePolygon;


void stress() {
    buffer.addPoint({10.0,10.0});
    for (int k = 10; k < 50; k+=10) {
    	if ((k/10)%2 == 0) {
    		for (int i = 20; i < 800; i += 10) {
    			buffer.addPoint({i*1.0,k*1.0});
    		}
    	} else {
    		for (int i = 780; i >= 20; i -= 10) {
    			buffer.addPoint({i*1.0,k*1.0});
    		}
    	}
    }
    buffer.addPoint({10.0,590.0});
	polygons.push_back(buffer);
	buffer.points.clear();			
}

Geo::Polygon makeDirPolygon() {
	std::vector<Geo::Vector> ans;
	ans.push_back(viewPoint);
	Geo::Vector dirA(direction);
	dirA.rotate(viewAngle);
	Geo::Line l(viewPoint, dirA);
	std::vector<Geo::Vector> res;
	Geo::intersect(l, boundsPolygon, res);
	for (Geo::Vector v : res) {
		if (Geo::greater((v-viewPoint)^dirA, 0)) {
			ans.push_back(v);
			break;
		}
	}

	Geo::Vector dirB(direction);

	dirB.rotate(-viewAngle);
	l = Geo::Line(viewPoint, dirB);
	res.clear();
	Geo::intersect(l, boundsPolygon, res);
	for (Geo::Vector v : res) {
		if (Geo::greater((v-viewPoint)^dirB, 0)) {
			ans.push_back(v);
			break;
		}
	}
	for (Geo::Vector v : boundsPolygon.points) {
	    if (Geo::less(direction^(v-viewPoint), 0))
	    	continue;
	    if (orientation(ans[1], viewPoint, v) * orientation(ans[2], viewPoint, v) == -1)
	    	ans.push_back(v);		
	}
	std::sort(ans.begin() + 1, ans.end(), [](const Geo::Vector& a, const Geo::Vector& b) {
	        	                                    	return Geo::orientation(a, viewPoint, b) == Geo::LEFT;
													});
	return Geo::Polygon(ans);			 
}

bool FrameFunc() {
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
	hgeInputEvent evt;
	float a, b;
	hge->Input_GetMousePos(&a,&b);
	if (!createPolyClicked)
		direction = Geo::Vector(a,b)-viewPoint;				
	while (hge->Input_GetEvent(&evt)) {
  		switch (evt.type ) {
      		case INPUT_MBUTTONDOWN:
        		if (createPolyClicked) {
        			buffer.addPoint(Geo::Vector(a,b));	
				}
        		break;
        	case INPUT_KEYDOWN:
        		if (evt.key==HGEK_SPACE) {
        			if (createPolyClicked) {
						if (buffer.size() > 2) {
							polygons.push_back(buffer);
						}
						buffer.points.clear();
						createPolyClicked=false;
					} else {
						createPolyClicked=true;
					}	
        		}
        		break;
   		}
	}
	if (hge->Input_GetKeyState(HGEK_W)) 
        viewPoint.y -= dy;
    if (hge->Input_GetKeyState(HGEK_S)) 
       	viewPoint.y += dy;
    if (hge->Input_GetKeyState(HGEK_D)) 
        viewPoint.x += dx;
    if (hge->Input_GetKeyState(HGEK_A)) 
        viewPoint.x -= dx;        	
    
    viewPoint.x = std::min((double)WIDTH-10, viewPoint.x);
	viewPoint.x = std::max(10.0, viewPoint.x);

	viewPoint.y = std::min((double)HIGHT-10, viewPoint.y);
	viewPoint.y = std::max(10.0, viewPoint.y);

	Geo::Polygon dirPolygon = makeDirPolygon();
	std::vector<Geo::Polygon> out;
	Geo::intersect(Geo::visibilityPolygon(viewPoint, polygons, WIDTH, HIGHT), 
					dirPolygon, out);
	//out.push_back(Geo::visibilityPolygon(viewPoint, polygons, WIDTH, HIGHT));
	visiblePolygon = out[0];
	return false;
}

void drawPolygon(Geo::Vector o, Geo::Polygon poly, DWORD color, bool fill = true) {
	if (!fill) {
		for (int i = 0, c = poly.points.size(); c > 0; i++, c--) {
			Geo::Vector& a = poly[i];
			Geo::Vector& b = poly[(i+1)%poly.size()];
			hge->Gfx_RenderLine(a.x,a.y,b.x,b.y,color);		
		}		
		return;
	}
	for (int i = 0; i < 3; ++i)
		quad.v[i].col = color, quad.v[i].z=0;
	quad.v[0].x = o.x, quad.v[0].y = o.y;
	for (int i = 0, c = poly.points.size(); c > 0; i++, c--) {
		Geo::Vector& a = poly[i];
		Geo::Vector& b = poly[(i+1)%poly.size()];
		quad.v[1].x = a.x, quad.v[1].y = a.y;
		quad.v[2].x = b.x, quad.v[2].y = b.y;
		hge->Gfx_RenderTriple(&quad);		
	}	
	
}

void Draw_Circle(float cx, float cy, float Radius, int Segments, DWORD color) {
  	float EachAngle;
  	float a;
  	float x1;
 	float x2;
  	float y1;
  	float y2;
 
  	EachAngle = 2.0 * M_PI / (float)Segments;
 
  	x2 = Radius;
  	y2 = 0.0;
  	Geo::Polygon poly;
 	for(a=0.0; a<= (2.0*M_PI + EachAngle); a+=EachAngle) {
    	x1 = x2;
    	y1 = y2;
    	x2 = Radius * cos(a);
    	y2 = Radius * sin(a);
    	poly.addPoint(Geo::Vector(x1+cx, y1+cy));
		poly.addPoint(Geo::Vector(x2+cx, y2+cy));
  	}
  	drawPolygon(Geo::Vector(cx, cy), poly, color);
}


bool RenderFunc() {
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	hge->Gfx_RenderQuad(&backQuad);
	for (Geo::Polygon p : polygons) {
		drawPolygon(viewPoint, p, ARGB(255,0,0,0),false);
		for (Geo::Vector v : p.points)
			Draw_Circle(v.x,v.y,5,25,ARGB(250,0,0,0));
	}
	for (Geo::Vector v : buffer.points)
		Draw_Circle(v.x,v.y,5,25,ARGB(250,0,0,0));
	
	Draw_Circle(viewPoint.x, viewPoint.y, 10, 50, ARGB(255, 0, 150, 0));
	fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d (constant)", hge->Timer_GetDelta(), hge->Timer_GetFPS());
	if (createPolyClicked)
		fnt->printf(5, 70, HGETEXT_LEFT, "ENTER POLYGON");	
	fnt->printf(5, HIGHT-50, HGETEXT_LEFT, "x:%.2f y:%.2f", viewPoint.x, viewPoint.y);	
	
	drawPolygon(viewPoint, visiblePolygon, ARGB(100,0,100,0));
	
	hge->Gfx_EndScene();
	return false;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "ViewPoint");

	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, WIDTH);
	hge->System_SetState(HGE_SCREENHEIGHT, HIGHT);
	hge->System_SetState(HGE_SCREENBPP, 32);
	hge->System_SetState(HGE_ZBUFFER, true);
	hge->System_SetState(HGE_HIDEMOUSE, false);
	hge->System_SetState(HGE_FPS, 100);
	
	if (hge->System_Initiate()) {
		fnt=new hgeFont("font1.fnt");
 		fnt->SetColor(ARGB(255,0,0,0));
 		quad.blend=BLEND_DEFAULT_Z;
 		backQuad.blend=BLEND_DEFAULT_Z;
 		
 		backQuad.v[0].x=0; backQuad.v[0].y=0; 
  		backQuad.v[1].x=0; backQuad.v[1].y=HIGHT; 
  		backQuad.v[2].x=WIDTH; backQuad.v[2].y=HIGHT; 
  		backQuad.v[3].x=WIDTH; backQuad.v[3].y=0; 
  		for (int i = 0; i < 4; i++)
  			backQuad.v[i].col = ARGB(255,255,255,255), backQuad.v[i].z=0.5;
  			
		hge->System_Start();
		delete fnt;
  	}	

  	hge->System_Shutdown();
	hge->Release();

	return 0;
}
