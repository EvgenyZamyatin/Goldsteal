#include "Geometry.h"
#include <cmath>
#include <algorithm>
#include <cassert>

using namespace Geo;

bool Geo::equals(double a, double b) {
    return fabs(a-b) < Geo::EPSILON;
}

bool Geo::greater(double a, double b) {
    return a > b+Geo::EPSILON;
}

bool Geo::less(double a, double b) {
    return a < b-Geo::EPSILON;
}

double Geo::Vector::len() const {
    return std::sqrt(len2());
}

double Geo::Vector::len2() const {
    return x*x+y*y;
}

bool Geo::Vector::operator== (const Vector& other) const {
    return fabs(x-other.x) < Geo::EPSILON && fabs(y-other.y) < EPSILON;   
}

bool Geo::Vector::operator< (const Vector& other) const {
    return less(x, other.x) || 
        (equals(x,other.x) && less(y, other.y));
}

std::string Geo::Polygon::to_string() const {
    std::string ans = "";
    for (Vector p : points)
        ans += p.to_string() + " ";
    return ans;
}

std::ostream& Geo::operator<<(std::ostream &strm, Polygon p) {
    return strm << p.to_string();
}

int Geo::orientation (Vector a, Vector c, Vector b) {
    a -= c;
    b -= c;
    //a.norm();
    //b.norm();
    double tmp = a*b;
    if (fabs(tmp) < Geo::EPSILON)
        return Geo::COLLINEAR;
    if (tmp < 0)
        return Geo::LEFT;
    return Geo::RIGHT;
}

bool Geo::onLine(const Vector& v, const Line& l) {
    if (equals(l.v.x, 0)) 
        return equals(v.x, l.u.x);
    return Geo::equals(l.k() * v.x+l.b(), v.y);
}

bool Geo::collinear (const Line& a, const Line& b) {
    return fabs(a.v*b.v) < Geo::EPSILON;
}

bool Geo::collinear (const Line& a, const Segment& b) {
    return fabs(a.v*(b.b-b.a)) < Geo::EPSILON;
}

bool Geo::collinear (const Segment& a, const Segment& b) {
    return fabs((a.a-a.b)*(b.a-b.b)) < Geo::EPSILON;
}

//Same line dont intersects.
bool Geo::intersect (const Line& a, const Line& b, Vector& res) {
    if (collinear(a, b))
        return false;
    double c = (b.u*b.v-a.u*b.v)/(a.v*b.v);
    res = a.u + a.v*c;
    return true;    
}

//If overlaps returns true.
bool Geo::intersect (const Line& a, const Segment& b, Vector& res, 
        bool consider_touch) {
    if (collinear(a, b) && (onLine(b.a, a) || onLine(b.b, a)))
        return true;
    if (collinear(a, b))
        return false;
    Vector u(b.a), v(b.b-b.a);
    double c = (a.u*a.v-u*a.v)/(v*a.v);
    if (c - 1 > Geo::EPSILON || c < - Geo::EPSILON ||
            ((fabs(c) < Geo::EPSILON || fabs(c-1) < Geo::EPSILON) && !consider_touch))
        return false;
    res = u + v * c;
    return true;
}

std::ostream& Geo::operator<<(std::ostream& strm, Vector v) {return strm << v.to_string();}

bool Geo::intersect (const Line& l, const Polygon& p, std::vector<Vector>& res, 
        bool consider_touch) {
    for (int i = 0, cnt = p.size(); cnt > 0; i++, cnt--) {
        Segment s(p[i], p[(i+1)%p.size()]);
        Vector pt;
        if (!intersect (l, s, pt))
            continue;
        if (collinear(l, s)) {
            if (consider_touch) {
                res.push_back(p[i]);
            }
            continue;
        }
        if (pt != p[i] && pt != p[(i+1)%p.size()]) {
            res.push_back(pt);
            continue;
        }
        if (pt == p[i]) {
            Vector a, b, c;
            a = p[(i-1+p.size())%p.size()], b = p[i], c = p[(i+1)%p.size()];
            if (consider_touch || orientation(a, b, b+l.v) * orientation(c, b, b+l.v) == -1) {
                res.push_back(pt);
            }
        }
    }
    return res.size() > 0;
}


/*
Polygon Geo::visibilityPolygon (Vector o, std::vector<Polygon> polygons) {
    std::vector<Vector> candidates;
    std::vector<std::pair<Vector, int>> vertices; 
    for (int i = 0; (int)i < polygons.size(); ++i) {
        Polygon p = polygons[i];
        for (Vector v : p.points)
            vertices.push_back(std::make_pair(v, i));
    }

    int cnt = 0;
    for (int i = 0; i < vertices.size(); ++i) { 
        if (less(vertices[i].first.y, o.y))
            std::swap(vertices[i], vertices[cnt++]);
    }
    //for (std::pair<Vector,int> i : vertices)
    //	std::cerr << i.first << "\n";
    //std::cerr << "===\n";
    std::sort(vertices.begin(), vertices.begin() + cnt, 
            [o](const std::pair<Vector, int>& a, const std::pair<Vector, int>& b) {
                return orientation(a.first, o, b.first) == LEFT || 
                    (orientation(a.first, o, b.first) == COLLINEAR && (a.first-o).len2() < (b.first-o).len2());    
            });
    std::sort(vertices.begin() + cnt, vertices.end(), 
            [o](const std::pair<Vector, int>& a, const std::pair<Vector, int>& b) {
                return orientation(a.first, o, b.first) == LEFT || 
                    (orientation(a.first, o, b.first) == COLLINEAR && (a.first-o).len2() < (b.first-o).len2());
            });
    //for (std::pair<Vector,int> i : vertices)
    //	std::cerr << i.first << "\n";
    Vector last;
    bool vl = false;
    std::vector<std::pair<Vector, int>> tmp;
    std::vector<Vector> tmp1;
    int lastPolygon = -1;
    //int firstPolygon = -1;  
    for (std::pair<Vector, int> pr : vertices) {
        tmp.clear();
        Vector& v = pr.first;
        if (vl && orientation(v, o, last) == COLLINEAR)
            continue;
        last = v;
        vl = true;
        Line l(o, v-o);
        //std::cerr << v << " : ";
        for (int i = 0; i < (int)polygons.size(); ++i) {
            Polygon p = polygons[i];
            tmp1.clear();
            intersect(l, p, tmp1, false);
            for (Vector j : tmp1)
                tmp.push_back(std::make_pair(j, i));
        }
        if (tmp.size() == 0) {
            candidates.push_back(v);
            lastPolygon = pr.second;
            //if (firstPolygon==-1)
            //	firstPolygon=lastPolygon;
            continue;
        }
        std::pair<Vector, int> u;
        bool was = false;
        for (std::pair<Vector, int> t : tmp) {
            if (greater((t.first-o)^l.v, 0) && (!was || less((t.first-o).len2(), (u.first-o).len2()))) {
                u = t;
                was = true;
            }
        }
        //std::cerr << u.first << "\n";
        if (u.first == v) {
            candidates.push_back(v);
            lastPolygon = u.second;
            //if (firstPolygon==-1)
            //	firstPolygon=lastPolygon;
            continue;
        }
        if (greater((u.first-o).len2(), (v-o).len2())) {
            if (lastPolygon == pr.second)
                candidates.push_back(v), candidates.push_back(u.first), lastPolygon = u.second;
            else
                candidates.push_back(u.first), candidates.push_back(v), lastPolygon = pr.second; 
            //if (firstPolygon==-1)
            //	firstPolygon=lastPolygon;
            continue;
        } else {
            continue;
        }
    }
    //if (firstPolygon != lastPolygon)
    //	std::swap(candidates[0], candidates[1]);
    Polygon ans(candidates);
    return ans;
}
*/

void Geo::Polygon::makeCW() {
	for (int i = 0; i < points.size(); ++i) {
    	if (orientation(points[i], points[(i+1)%points.size()], points[(i+2)%points.size()]) == LEFT) {
    		std::reverse(points.begin(), points.end());
    		break;
    	}    	
    }
}

void Geo::Polygon::makeNCW() {
	for (int i = 0; i < points.size(); ++i) {
    	if (orientation(points[i], points[(i+1)%points.size()], points[(i+2)%points.size()]) == RIGHT) {
    		std::reverse(points.begin(), points.end());
    		break;
    	}    	
    }
}




Polygon Geo::visibilityPolygon (Vector o, std::vector<Polygon> polygons) {
	for (Polygon& p : polygons) {
    	assert(p.size() > 2);
    	p.makeNCW();    
	}                      
	std::vector<Segment> vertices; 
    for (Polygon& p : polygons) {
    	for (int i = 0; i < p.size(); ++i) {
    	    vertices.push_back(Segment(p[i], p[(i+1)%p.size()]));
    		//vertices.insert(vertices.end(), p.points.begin(), p.points.end());
    	}
    }    
    int cnt = 0;
    for (int i = 0; i < vertices.size(); ++i) { 
        if (less(vertices[i].a.y, o.y))
            std::swap(vertices[i], vertices[cnt++]);
    }
    std::sort(vertices.begin(), vertices.begin() + cnt, 
            [o](const Segment& a, const Segment& b) {
                return orientation(a.a, o, b.a) == LEFT || 
                    (orientation(a.a, o, b.a) == COLLINEAR && (a.a-o).len2() < (b.a-o).len2());    
            });
    std::sort(vertices.begin() + cnt, vertices.end(), 
            [o](const Segment& a, const Segment& b) {
                return orientation(a.a, o, b.a) == LEFT || 
                    (orientation(a.a, o, b.a) == COLLINEAR && (a.a-o).len2() < (b.a-o).len2());
            });
    //std::cerr << vertices << "\n";
    std::vector<Vector> candidates;
    std::vector<Vector> tmp;
    for (int i = 0; i < (int)vertices.size(); ++i) {
    	Segment& v = vertices[i];
    	if (i > 0) {
    		if (orientation(vertices[i-1].a, o, vertices[i].a) == COLLINEAR)
    			continue;
    	}
   		tmp.clear();
   		Line l(o, v.a-o);
   		for (Polygon p : polygons)
   			intersect(l, p, tmp, false);
   		Vector nearest;
   		bool was = false;
   		for (Vector i : tmp) {
   			if (greater((v.a-o)^(i-o), 0) && (!was || less((i-o).len2(), (nearest-o).len2())))
   				nearest = i, was = true;
   	 	}
   	 	if (less((nearest-o).len2(), (v.a-o).len2()))
   	 		continue;
   	 	if (nearest == v.a) {
   	 		candidates.push_back(nearest);
   	 		continue;
   		}	
   		//assert(orientation(nearest, candidates[i-1], v) != COLLINEAR)
   		if (orientation(nearest, v.a, v.b) == LEFT) {
    		candidates.push_back(nearest); 
    		candidates.push_back(v.a); 
    	} else {
    		candidates.push_back(v.a); 
    		candidates.push_back(nearest); 
    	}    	
    }
    return Polygon(candidates);        
}



Polygon Geo::visibilityPolygon (Vector o, std::vector<Polygon> polygons, int w, int h) {
    Polygon p;
    p.addPoint(Vector(0, 0));
    p.addPoint(Vector(w, 0));
    p.addPoint(Vector(w, h));
    p.addPoint(Vector(0, h));
    polygons.push_back(p);
    return visibilityPolygon (o, polygons);    
}






