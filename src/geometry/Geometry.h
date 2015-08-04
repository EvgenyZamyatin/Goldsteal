#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <vector>
#include <iostream>
#include <string>

#include <boost/geometry.hpp>                                 
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/segment.hpp>
#include <boost/geometry/geometries/register/ring.hpp>
#include <boost/geometry/geometries/register/box.hpp>

namespace Geo {
    
    static const int LEFT = -1;
    static const int RIGHT = 1;
    static const int COLLINEAR = 0;
    static const int CLOCKWISE = -1;
    static const int COUNTERCLOCKWISE = 1;
    
    struct Vector {
        int x, y;
        Vector () {}
        Vector (int x, int y) : x(x), y(y) {};
        Vector (double x, double y) : x(x), y(y) {};

        Vector operator+ (const Vector& other) const {return Vector(x+other.x, y+other.y);}
        Vector& operator+= (const Vector& other){x+=other.x, y+=other.y; return *this;}
        Vector operator- (const Vector& other) const {return Vector(x-other.x, y-other.y);}
        Vector& operator-= (const Vector& other){x-=other.x, y-=other.y; return *this;}
        
        Vector operator* (int c) const {return Vector(x*c, y*c);}
        Vector& operator*= (int c) {x*=c, y*=c;return *this;}              
        
        Vector operator* (double c) const {return Vector(x*c, y*c);}
        Vector& operator*= (double c) {x*=c, y*=c;return *this;}              
        
        Vector operator/ (double c) const {return Vector(x*1.0/c, y*1.0/c);}
        Vector& operator/= (double c) {x=x*1.0/c, y=y*1.0/c;return *this;}
        
        //double len () const;
        int len2 () const;
        
        void rotate(double angle);
        void rotate(double sn, double cs);
        
        Geo::Vector normal() const;

        int operator* (const Vector& other) const {return x*other.y-y*other.x;}
        int operator^ (const Vector& other) const {return x*other.x+y*other.y;}
        
        bool operator==(const Vector& other) const;
        bool operator!=(const Vector& other) const {return !((*this) == other);}
        bool operator< (const Vector& other) const;
        std::string to_string() {
            return "(" + std::to_string(x) + " " + ", " + std::to_string(y) + ")";
        }
    };
    
    std::ostream& operator<<(std::ostream&, Vector);
    
    struct Line {
        Line() {}
        Line(Vector const& a, Vector const& b);
        bool contains(Vector const& v) const;
        int a, b, c;                 
    };

    struct Segment {
        Vector a, b;
        Segment () {}
        Segment (const Vector& a, const Vector& b) : a(a), b(b) {}
        int len2() const {return (a-b).len2();}
        bool operator==(Segment const& other) {return a==other.a && b==other.b;}
    };

    struct Polygon {
        std::vector<Vector> points;

        Polygon () {}
        Polygon (const std::vector<Vector>& pts) {points = pts; points.push_back(pts[0]);}
        int size() const {return points.size()-1;}
        Vector const& operator[] (int i) const {return points[i];}
        Vector operator[] (int i) {return points[i];}
        std::string to_string() const;
        void makeCW();
        void makeCCW();
        int order() const;                             
        void rotate(const Vector&, double angle);
        void rotate(int v, double angle);
        void rotate(const Vector&, double sn, double cs);
        //around first point.
        void rotate(double angle);
        Vector center() const;
    };
    
    std::ostream& operator<<(std::ostream&, Polygon);
    
    struct Box {
    	Vector a, b;
        Box() {}
    	Box(Vector const& a, Vector const& b) : a(a), b(b) {}
    	Box(int x, int y, int width, int hight) : a(x,y), b(x+width, y+hight) {}
    	Box(Polygon const& p);
    };
    
    
    bool between(int a, int b, int c);
    int orientation (Vector a, Vector c, Vector b);
    
    bool collinear (const Line& a, const Line& b);
    bool collinear (const Line& a, const Segment& b);
    bool collinear (const Segment& a, const Segment& b);

    bool intersect (const Line& a, const Line& b, Vector& res);
    bool intersect (const Line& a, const Segment& s, Vector& res, bool consider_touch=true);
    
    //bool intersect (const Segment& a, const Segment& b, Vector& res, bool consider_touch=true);
    //bool intersect (const Line& line, const Polygon& p, std::vector<Vector>& res, bool consider_touch=true);

    //bool intersect (Polygon p1, Polygon p2, std::vector<Polygon>& out);
	//bool intersect (Polygon p1, Polygon p2, Polygon& out);

	bool intersects(Box const& a, Box const& b);

    Polygon visibilityPolygon(Vector o, std::vector<Polygon> polygons, int w, int h);//for testing
    Polygon visibilityPolygon(Vector const& o, std::vector<Polygon> const& polygons);
    Polygon visibilityPolygonFast(Vector const& o, std::vector<Polygon> const& polygons);
    
    int distance2(Vector const& a, Polygon const& b);
    int distance2(Vector const& a, Segment const& b);
    int distance2(Polygon const& a, Polygon const& b);
	int distance2(Segment const& a, Polygon const& b);  
}

BOOST_GEOMETRY_REGISTER_POINT_2D(Geo::Vector, double, boost::geometry::cs::cartesian, x, y);
BOOST_GEOMETRY_REGISTER_RING(std::vector<Geo::Vector>);
BOOST_GEOMETRY_REGISTER_SEGMENT(Geo::Segment, Geo::Vector, a, b);
BOOST_GEOMETRY_REGISTER_BOX(Geo::Box, Geo::Vector, a, b);

#endif
