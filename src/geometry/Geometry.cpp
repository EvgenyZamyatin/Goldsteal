#include "Geometry.h"
#include <boost/math/special_functions/round.hpp>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <set>
#include <limits>

using namespace Geo;


bool Geo::Vector::operator== (const Vector& other) const {
    return x==other.x && y==other.y;   
}

bool Geo::Vector::operator< (const Vector& other) const {
    return x < other.x || 
        (x == other.x && y < other.y);
}

Geo::Vector::Vector (double x, double y) {        
	this->x = boost::math::iround(x);
	this->y = boost::math::iround(y);
}

Vector Geo::Vector::operator* (double c) const {return Vector(c*x, c*y);}

Vector& Geo::Vector::operator*= (double c) {
	x=boost::math::iround(c*x); 
	y=boost::math::iround(c*y); 
	return *this;
}              
        
Vector Geo::Vector::operator/ (double c) const {return Vector(x*1.0/c, y*1.0/c);}

Vector& Geo::Vector::operator/= (double c) {
	x=boost::math::iround(x*1.0/c); 
	y=boost::math::iround(y*1.0/c);
	return *this;
}

Vector Geo::Vector::operator/ (int c) const {return Vector((x+(c>>1))/c, (y+(c>>1))/c);}

Vector& Geo::Vector::operator/= (int c) {
	x=((x+(c>>1))/c); 
	y=((y+(c>>1))/c);
	return *this;
}

int Geo::Vector::len2() const {
	return x*x + y*y;
}

int Geo::orientation (Vector a, Vector c, Vector b) {
    a -= c;
    b -= c;
    int tmp = a*b;
    if (tmp == 0)
        return Geo::COLLINEAR;
    if (tmp < 0)
        return Geo::LEFT;
    return Geo::RIGHT;
}

Line::Line(Vector const& p1, Vector const& p2) {
	a = p1.y - p2.y;
	b = p2.x - p1.x;
	c = -a*p1.x - b*p1.y;
}

bool Geo::Line::contains(Vector const& v) const {
    return a*v.x + b*v.y + c == 0;
}

long long det (int a, int b, int c, int d) {
	return ((long long)a) * d - ((long long)b) * c;
}

bool Geo::collinear (const Line& m, const Line& n) {
	long long zn = det(m.a, m.b, n.a, n.b);
    if (zn == 0)
    	return true;
    return false;
}

bool Geo::collinear (const Line& a, const Segment& b) {
    return collinear(a, Line(b.a, b.b));
}

bool Geo::collinear (const Segment& a, const Segment& b) {
    return (a.a-a.b)*(b.a-b.b) == 0;
}

bool Geo::intersect (const Line& m, const Line& n, Vector& res) {
    long long zn = det(m.a, m.b, n.a, n.b);
    if (zn == 0)
    	return false;
    //res.x = boost::math::iround(-det(m.c, m.b, n.c, n.b)*1.0 / zn);//!!!may be cast to double not necessary.
	//res.y = boost::math::iround(-det(m.a, m.c, n.a, n.c)*1.0 / zn);
	res.x = (-det(m.c, m.b, n.c, n.b) + (zn>>1)) / zn;
	res.y = (-det(m.a, m.c, n.a, n.c) + (zn>>1)) / zn;
    return true;    
}

bool Geo::between(int a, int b, int c) {
	return (a<=c)? (a<=b && b<=c) : (c<=b && b<=a);
}

bool Geo::intersect (const Line& l, const Segment& s, Vector& res, 
        bool consider_touch) {
    if (!intersect(l, Line(s.a, s.b), res))
    	return false;
  	if (res == s.a || res == s.b)	
    	return consider_touch;
    if (!between(s.a.x, res.x, s.b.x) || !between(s.a.y, res.y, s.b.y))
    	return false;
    return true;
}

void Geo::Vector::rotate(double angle) {
	rotate(std::sin(angle), std::cos(angle));
}

void Geo::Vector::rotate(double sn, double cs) {
	int nx = boost::math::iround(cs*x-sn*y);
	int ny = boost::math::iround(sn*x+cs*y);
	x = nx;
	y = ny;
}

Geo::Vector Geo::Vector::normal() const {
	Vector ans(-y, x);
	return ans;
}

int Geo::Polygon::order() const {
	for (int i = 0; i < points.size() - 2; ++i) {
    	if (orientation(points[i], points[i+1], points[i+2]) == LEFT)
    		return Geo::COUNTERCLOCKWISE;    	
		if (orientation(points[i], points[i+1], points[i+2]) == RIGHT)
    		return Geo::CLOCKWISE;    	
    }
    #ifdef DEBUG
    	assert(false && "order(): All points in polygon at the same line");
    #endif
}

void Geo::Polygon::makeCW() {
	for (int i = 0; i < points.size() - 2; ++i) {
    	if (orientation(points[i], points[i+1], points[i+2]) == LEFT) {
    		std::reverse(points.begin(), points.end());
    		return;
    	}    	
    	if (orientation(points[i], points[i+1], points[i+2]) == RIGHT)
    		return;
    }
    #ifdef DEBUG
    	assert(false && "makeCW(): All points in polygon at the same line");
    #endif
}

void Geo::Polygon::makeCCW() {
	for (int i = 0; i < points.size() - 2; ++i) {
    	if (orientation(points[i], points[i+1], points[i+2]) == RIGHT) {
    		std::reverse(points.begin(), points.end());
    		return;
    	}    	
    	if (orientation(points[i], points[i+1], points[i+2]) == LEFT)
    		return;
    }
    #ifdef DEBUG
    	assert(false && "makeCW(): All points in polygon at the same line");
    #endif
}



void Geo::Polygon::rotate(const Vector& o, double angle) {
	for (Vector& v : points) {
		v -= o;
		v.rotate(angle);
		v += o;
	}
}

void Geo::Polygon::rotate(const Vector& o, double sn, double cs) {
	for (Vector& v : points) {
		v -= o;
		v.rotate(sn, cs);
		v += o;
	}
}

void Geo::Polygon::rotate(int v, double angle) {
	Vector& o = points[v];
	for (int i = 0; i < size()+1; ++i) {
		if (i == v)
			continue;
		points[i] -= o;
		points[i].rotate(angle);
		points[i] += o;
	}
}

void Geo::Polygon::rotate(double angle) {
	Vector& o = points[0];
	for (int i = 1; i < size()+1; ++i) {
		points[i] -= o;
		points[i].rotate(angle);
		points[i] += o;
	}
}

Vector Geo::Polygon::center() const {
	Vector ans(0,0);
	for (int i = 0; i < size(); ++i) {
		ans += points[i];
	}
	ans /= size();
	return ans;
}

Box::Box(Polygon const& p) {
	a.x = a.y = std::numeric_limits<int>::max();
	b.x = b.y = std::numeric_limits<int>::min();
	for (Vector const& v : p.points) {
		a.x = std::min(a.x, v.x);
		a.y = std::min(a.y, v.y);
		
		b.x = std::max(b.x, v.x);
		b.y = std::max(b.y, v.y);
	}
}

//!!!Polygons must be correct
int Geo::distance2(Polygon const& a, Polygon const& b) {
	#ifdef DEBUG                                
		assert(a.order() == COUNTERCLOCKWISE);
		assert(b.order() == COUNTERCLOCKWISE);
	#endif
	return boost::geometry::comparable_distance(a.points, b.points);
}

int Geo::distance2(Segment const& a, Polygon const& b) {
	#ifdef DEBUG                                
		assert(b.order() == COUNTERCLOCKWISE);
	#endif
	return boost::geometry::comparable_distance(a, b.points);
}

int Geo::distance2(Vector const& a, Polygon const& b) {
	#ifdef DEBUG                                
		assert(b.order() == COUNTERCLOCKWISE);
	#endif
	return boost::geometry::comparable_distance(a, b.points);
}

int Geo::distance2(Vector const& a, Segment const& b) {
	return boost::geometry::comparable_distance(a, b);
}

bool Geo::intersects(Box const& a, Box const& b) {
	#ifdef DEBUG                                
		assert(a.a.x <= a.b.x && a.a.y <= a.b.y);
		assert(b.a.x <= b.b.x && b.a.y <= b.b.y);
	#endif
	return boost::geometry::intersects(a, b);
}

std::ostream& Geo::operator<<(std::ostream& strm, Vector v) {return strm << v.to_string();}
std::ostream& Geo::operator<<(std::ostream &strm, Polygon p) {
    return strm << p.to_string();
}

std::string Geo::Polygon::to_string() const {
    std::string ans = "";
    for (Vector p : points)
        ans += p.to_string() + " ";
    return ans;
}
