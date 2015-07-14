#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <vector>
#include <iostream>
#include <string>

namespace Geo {
    
    static double EPSILON = 1e-5;
    static const int LEFT = -1;
    static const int RIGHT = 1;
    static const int COLLINEAR = 0;
    static const int CLOCKWISE = -1;
    static const int COUNTERCLOCKWISE = 1;
    
    bool equals(double a, double b);
    bool greater(double a, double b);
    bool less(double a, double b);
    
    struct Vector {
        double x, y;
        Vector () {}
        Vector (double x, double y) : x(x), y(y) {};
        Vector operator+ (const Vector& other) const {return Vector(x+other.x, y+other.y);}
        Vector& operator+= (const Vector& other){x+=other.x, y+=other.y; return *this;}
        Vector operator- (const Vector& other) const {return Vector(x-other.x, y-other.y);}
        Vector& operator-= (const Vector& other){x-=other.x, y-=other.y; return *this;}
        Vector operator* (double c) const {return Vector(x*c, y*c);}
        Vector& operator*= (double c) {x*=c, y*=c;return *this;}
        Vector operator/ (double c) const {return Vector(x/c, y/c);}
        Vector& operator/= (double c) {x/=c, y/=c;return *this;}
        double len () const;
        double len2 () const;
        void rotate(double angle);
        void norm() {*this/=len();}
        double operator* (const Vector& other) const {return x*other.y-y*other.x;}
        double operator^ (const Vector& other) const {return x*other.x+y*other.y;}
        bool operator==(const Vector& other) const;
        bool operator!=(const Vector& other) const {return !((*this) == other);}
        bool operator< (const Vector& other) const;
        std::string to_string() {
            return "(" + std::to_string(x) + " " + ", " + std::to_string(y) + ")";
        }
        
    };
    
    std::ostream& operator<<(std::ostream&, Vector);
    
    struct Line {
        Vector u, v;
        Line () {}
        Line (const Vector& u, const Vector& v) : u(u), v(v) {}
        Line (const Line& other) : u(other.u), v(other.v) {}
        double k() const {return v.y/v.x;}
        double b() const {return u.y-k()*u.x;}
    };

    struct Segment {
        Vector a, b;
        Segment () {}
        Segment (const Vector& a, const Vector& b) : a(a), b(b) {}
        double len() const {return (a-b).len();}
        double len2() const {return (a-b).len2();}
    };

    struct Polygon {
        std::vector<Vector> points;
        Polygon () {}
        Polygon (const std::vector<Vector>& pts) : points(pts) {}
        void addPoint(const Vector& pt) {points.push_back(pt);}
        int size() const {return points.size();}
        Vector& operator[] (int i) {return points[i];}
        Vector operator[] (int i) const {return points[i];}
        std::string to_string() const;
        void makeCW();
        void makeNCW();
        int order();
    };
    
    std::ostream& operator<<(std::ostream&, Polygon);
    
    int orientation (Vector a, Vector c, Vector b);
    bool onLine(const Vector& v, const Line& l);
    bool collinear (const Line& a, const Line& b);
    bool collinear (const Line& a, const Segment& b);
    bool collinear (const Segment& a, const Segment& b);

    bool intersect (const Line& a, const Line& b, Vector& res);
    bool intersect (const Line& a, const Segment& s, Vector& res, bool consider_touch=true);
    bool intersect (const Segment& a, const Segment& b, Vector& res, bool consider_touch=true);
    bool intersect (const Line& line, const Polygon& p, std::vector<Vector>& res, bool consider_touch=true);
    bool intersect (Polygon p1, Polygon p2, std::vector<Polygon>& out);

    Polygon visibilityPolygon(Vector o, std::vector<Polygon> polygons, int w, int h);
    Polygon visibilityPolygon(Vector o, std::vector<Polygon> polygons);
    
    double distance(Polygon a, Polygon b);
	double distance(Segment a, Polygon b);

    
}

#endif
