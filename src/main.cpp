#include "hge.h"
#include "hgefont.h"
#include "geometry/Geometry.h"
#include "geometry/Boosting.h"
#include <iostream>
#include <fstream>

#define WIDTH 800
#define HIGHT 600

const double dx = 5;
const double dy = 5;

HGE *hge = 0;
hgeTriple quad;
hgeQuad backQuad;
hgeFont* fnt;

std::vector<Geo::Polygon> polygons;
Geo::Vector viewPoint(WIDTH*1.0/2, HIGHT*1.0/2);
bool createPolyClicked=false;
Geo::Polygon buffer;

bool FrameFunc() {
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
	hgeInputEvent evt;
	while (hge->Input_GetEvent(&evt)) {
  		switch (evt.type ) {
      		case INPUT_MBUTTONDOWN:
        		if (createPolyClicked) {
        			float a, b;
					hge->Input_GetMousePos(&a,&b);
					buffer.addPoint(Geo::Vector(a,b));	
				}
        		break;
        	case INPUT_KEYDOWN:
        		if (evt.key==HGEK_SPACE) {
        			if (createPolyClicked) {
						if (buffer.size() > 1) {
							polygons.push_back(buffer);
						}
						buffer.points.clear();
						createPolyClicked=false;
					} else {
						createPolyClicked=true;
					}	
        		} else if (evt.key==HGEK_UP) {
        			viewPoint.y -= dx;
        		} else if (evt.key==HGEK_DOWN) {
        			viewPoint.y += dx;
        		} else if (evt.key==HGEK_RIGHT) {
        			viewPoint.x += dy;
        		} else if (evt.key==HGEK_LEFT) {
        			viewPoint.x -= dy;
        		} else if (evt.key==HGEK_CTRL) {
        			std::ofstream file;
        			file.open("wrong.txt");
        			file << "viewPoint: " << viewPoint << "\n";
        			file << "Polygons:\n";
        			for (Geo::Polygon p : polygons)
        				file << p << "\n";
        			file << "VisibilityPolygon:\n" << Geo::visibilityPolygon(viewPoint, polygons, WIDTH, HIGHT) << "\n";
        			file.close();        				 	
        		}

        		break;
   		}
	}
	
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
	for (int i = 0; i < 4; ++i)
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
	
	Geo::Polygon p = Geo::visibilityPolygon(viewPoint, polygons, WIDTH, HIGHT);
	drawPolygon(viewPoint,p,ARGB(100,0,100,0));
	
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
	
	//hge->System_SetState( HGE_ZBUFFER, true);
	if (hge->System_Initiate()) {
 		fnt=new hgeFont("font1.fnt");
 		fnt->SetColor(ARGB(255,0,0,0));
 		//quad.tex = backQuad.tex = 0;
 		
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
