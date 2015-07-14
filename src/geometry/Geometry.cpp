#include "Geometry.h"
#include <cmath>
#include <algorithm>
#include <cassert>
#include "Boosting.h"

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

void Geo::Vector::rotate(double angle) {
	double nx = std::cos(angle)*x-std::sin(angle)*y;
	double ny = std::sin(angle)*x+std::cos(angle)*y;
	x = nx;
	y = ny;
}

int Geo::Polygon::order() {
	for (int i = 0; i < points.size(); ++i) {
    	if (orientation(points[i], points[(i+1)%points.size()], points[(i+2)%points.size()]) == LEFT)
    		return Geo::COUNTERCLOCKWISE;    	
    }
    return Geo::CLOCKWISE;
}

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
    	assert(p.size() > 1);
    	if (p.size() > 2)
    		p.makeNCW();
    	else {
    		if (orientation(p.points[0], o, p.points[1]) == RIGHT)
    			std::swap(p.points[0], p.points[1]);    
    	}
	}                      
	std::vector<Segment> vertices; 
    for (Polygon& p : polygons) {
    	for (int i = 0; i < p.size(); ++i) {
    	    vertices.push_back(Segment(p[i], p[(i+1)%p.size()]));
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


bool Geo::intersect(Polygon p1, Polygon p2, std::vector<Polygon>& out) {
	boost::geometry::correct(p1.points);
	boost::geometry::correct(p2.points);
	std::vector<std::vector<Geo::Vector>> tmp;
	boost::geometry::intersection(p1.points, p2.points, tmp);
	for (int i = 0; i < (int)tmp.size(); ++i) {
		tmp[i].pop_back();
		out.push_back(Polygon(tmp[i]));
   	}	
   	return tmp.size() > 0;
}

double Geo::distance(Polygon a, Polygon b) {
	boost::geometry::correct(a.points);
	boost::geometry::correct(b.points);
	return boost::geometry::distance(a.points, b.points);
}

double Geo::distance(Segment a, Polygon b) {
	boost::geometry::correct(a);
	boost::geometry::correct(b.points);
	return boost::geometry::distance(a, b.points);
}

