#include "Geometry.h"
#include <cmath>
#include <algorithm>
#include <cassert>
#include <set>

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
    if (collinear(a, b) && (onLine(b.a, a) || onLine(b.b, a))) {
        if (Geo::less((a.u-b.a).len2(), (a.u-b.b).len2()))
        	res = b.a;
        else 
        	res = b.b;
        return true;
    }
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
	rotate(std::sin(angle), std::cos(angle));
}

void Geo::Vector::rotate(double sn, double cs) {
	double nx = cs*x-sn*y;
	double ny = sn*x+cs*y;
	x = nx;
	y = ny;
}

Geo::Vector Geo::Vector::normal() {
	Vector ans(-y, x);
	ans.norm();
	return ans;
}

int Geo::Polygon::order() {
	for (int i = 0; i < points.size(); ++i) {
    	if (orientation(points[i], points[(i+1)%points.size()], points[(i+2)%points.size()]) == LEFT)
    		return Geo::COUNTERCLOCKWISE;    	
		if (orientation(points[i], points[(i+1)%points.size()], points[(i+2)%points.size()]) == RIGHT)
    		return Geo::CLOCKWISE;    	
    }
    return Geo::CLOCKWISE;
}

void Geo::Polygon::makeCW() {
	for (int i = 0; i < points.size(); ++i) {
    	if (orientation(points[i], points[(i+1)%points.size()], points[(i+2)%points.size()]) == LEFT) {
    		std::reverse(points.begin(), points.end());
    		return;
    	}    	
    	if (orientation(points[i], points[(i+1)%points.size()], points[(i+2)%points.size()]) == RIGHT)
    		return;
    }
}

void Geo::Polygon::makeCCW() {
	for (int i = 0; i < points.size(); ++i) {
    	if (orientation(points[i], points[(i+1)%points.size()], points[(i+2)%points.size()]) == RIGHT) {
    		std::reverse(points.begin(), points.end());
    		return;
    	}
    	if (orientation(points[i], points[(i+1)%points.size()], points[(i+2)%points.size()]) == LEFT)
    		return;	
    }
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
	for (int i = 0; i < size(); ++i) {
		if (i == v)
			continue;
		points[i] -= o;
		points[i].rotate(angle);
		points[i] += o;
	}
}

void Geo::Polygon::rotate(double angle) {
	Vector& o = points[0];
	for (int i = 1; i < size(); ++i) {
		points[i] -= o;
		points[i].rotate(angle);
		points[i] += o;
	}
}

Vector Geo::Polygon::center() {
	Vector ans(0,0);
	for (Vector v : points)
		ans += v;
	ans /= points.size();
	return ans;
}

Box::Box(Polygon const& p) {
	a.x = a.y = 1000000000;
	b.x = b.y = -1000000000;
	for (Vector const& v : p.points) {
		a.x = std::min(a.x, v.x);
		a.y = std::min(a.y, v.y);
		
		b.x = std::max(b.x, v.x);
		b.y = std::max(b.y, v.y);
	}
}

/*
struct Segment1 : Segment {
	Segment1(Vector const& a, Vector const& b) : Segment(a, b) {}
	Vector next;
	double val;
};

bool intersect1 (const Vector& o, const Line& l, const Polygon& p, Vector& rv, Vector& nxt, 
        bool consider_touch) {
    double mn = 0;
    bool was = false;
    for (int i = 0, cnt = p.size(); cnt > 0; i++, cnt--) {
        Segment s(p[i], p[(i+1)%p.size()]);
        Vector pt;
        if (!intersect (l, s, pt))
            continue;
        if (collinear(l, s)) {
            if (consider_touch) {
                if (greater(l.v^(p[i]-o), 0) && (!was || (less((p[i]-o).len2(), mn)))) {
                	mn = (p[i]-o).len2();
                	rv = p[i];
                	Vector const& a = (i==p.size()-1) ? p[0] : p[i+1];
					Vector const& b = (i==0) ? p[p.size()-1] : p[i-1];
					if (orientation(p[i], o, a) == LEFT)
						nxt = a;
					else
						nxt = b;	
				   	was = true;
                }
            }
            continue;
        }
        if (pt != p[i] && pt != p[(i+1)%p.size()]) {
            if (greater(l.v^(pt-o), 0) && (!was || (less((pt-o).len2(), mn)))) {
                mn = (pt-o).len2();
                rv = pt;
                Vector const& a = p[i];
				Vector const& b = p[(i+1)%p.size()];
				if (orientation(pt, o, a) == LEFT)
					nxt = a;
				else
					nxt = b;	
				was = true;
            }
            continue;
        }
        if (pt == p[i]) {
            Vector a, b, c;
            a = p[(i-1+p.size())%p.size()], b = p[i], c = p[(i+1)%p.size()];
            if (consider_touch || orientation(a, b, b+l.v) * orientation(c, b, b+l.v) == -1) {
                if (greater(l.v^(p[i]-o), 0) && (!was || (less((pt-o).len2(), mn)))) {
                    mn = (pt-o).len2();
                    rv = pt;
                    if (orientation(pt, o, a) == LEFT)
    					nxt = a;
    				else
    					nxt = c;	
    				was = true;
                }    
            }
        }
    }

    return was;
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
	std::vector<Segment1> vertices;                  
	for (Polygon& p : polygons) {
    	for (int i = 0; i < p.size(); ++i) {
    	    Vector& a = p[i]; 
    	    Vector& b = (i == p.size() - 1) ? p[0] : p[i+1]; 
    	    Vector& c = (i == 0) ? p[p.size()-1] : p[i-1];
    	    vertices.push_back(Segment1(a, b));
    	    if (orientation(a, o, b) == LEFT)
    	    	vertices.back().next = b;
    	    else
	    	    vertices.back().next = c;
    	   	vertices.back().val = std::max((a-o).len2(), (vertices.back().next-o).len2());
    	}
    }    
    int cnt = 0;
    for (int i = 0; i < vertices.size(); ++i) { 
        if (less(vertices[i].a.y, o.y))
            std::swap(vertices[i], vertices[cnt++]);
    }
    std::sort(vertices.begin(), vertices.begin() + cnt, 
            [o](const Segment1& a, const Segment1& b) {
                return orientation(a.a, o, b.a) == LEFT || 
                    (orientation(a.a, o, b.a) == COLLINEAR && (a.a-o).len2() < (b.a-o).len2());    
            });
    std::sort(vertices.begin() + cnt, vertices.end(), 
            [o](const Segment1& a, const Segment1& b) {
                return orientation(a.a, o, b.a) == LEFT || 
                    (orientation(a.a, o, b.a) == COLLINEAR && (a.a-o).len2() < (b.a-o).len2());
            });
    std::vector<Vector> candidates;
    std::vector<std::pair<Vector, double>> stack;
    for (int i = 0; i < (int)vertices.size(); ++i) {
    	Segment1& v = vertices[i];
    	while (stack.size() > 0 && (orientation(stack.back().first, o, v.a) == LEFT))
    		stack.pop_back();
    	if (i > 0) {
    		if (orientation(vertices[i-1].a, o, vertices[i].a) == COLLINEAR || 
    				(stack.size() > 0 && less(stack.back().second, (o-v.a).len2())))
    			continue;
    	}
    	Line l(o, v.a-o);
   		Vector nearest, nxt;
   		double d = 0;
   		bool was = false;
   		for (Polygon& p : polygons) {
   			Vector n1, nxt1;
   			if (intersect1(o, l, p, n1, nxt1, false)) {
   				if (!was || less((o-n1).len2(), d)) {
   					d = (o-n1).len2();
   					nearest = n1;
   					nxt = nxt1; 
   			   	}
   				was = true;
   			}
   		}
   	    double val = std::max((o-nxt).len2(), (o-nearest).len2());
   	    
   	    if (less((nearest-o).len2(), (v.a-o).len2()))
   	 		continue;
   	 	if (nearest == v.a) {
   	 		candidates.push_back(nearest);
   	 		stack.push_back(std::make_pair(nxt, val));
   	 		continue;
   		}	
   		if (orientation(nearest, v.a, v.b) == LEFT) {
    		candidates.push_back(nearest); 
    		candidates.push_back(v.a);
    		stack.push_back(std::make_pair(v.next, v.val)); 
    	} else {
    		candidates.push_back(v.a); 
    		candidates.push_back(nearest); 
    		stack.push_back(std::make_pair(nxt, val));
    	}
   	}
   	return Geo::Polygon(candidates);
}
*/

struct Segment1 : Segment {
	Segment1(Vector const& a, Vector const& b, Vector const& c, Vector const& d) : Segment(a, b), c(c), d(d) {}
	Segment1(Vector const& a, Vector const& b) : Segment(a, b) {}
	Vector c; // point after b
	Vector d; // point before a
}; 

void add(std::set<Segment1, std::function<bool(Segment1 const&, Segment1 const&)>>& set, Vector const& o, Segment1 const& s) {
	if (orientation(s.a, o, s.b) == LEFT)
		set.insert(s);
} 

bool touch (Vector const& o, Segment1 const& s) {
	return !(orientation(o, s.b, s.a) * orientation(o, s.b, s.c) == -1);	
} 


bool remove(std::set<Segment1, std::function<bool(Segment1 const&, Segment1 const&)>>& set, Vector const& o, Segment1 const& s) {
	if (orientation(s.d, o, s.a) == RIGHT)
		return false;
	Segment1 s1(s.d, s.a);
	auto it = set.lower_bound(s1);
	if ((*it).a==s1.a && (*it).b==s1.b) {
		s1=*it;
		bool ans = (it == set.begin()) && !touch(o, s1);
		set.erase(it);
		return ans;
	}
   	return false;
} 

Polygon Geo::visibilityPolygonFast (Vector o, std::vector<Polygon> polygons) {
	std::vector<Segment1> vertices;                  
	for (Polygon& p : polygons) {
    	for (int i = 0; i < p.size(); ++i) {
    	    Vector const& a = p[i]; 
    	    Vector const& b = (i == p.size() - 1) ? p[0] : p[i+1]; 
    	    Vector const& c = (i == p.size() - 1) ? p[1] : (i == p.size() - 2 ? p[0] : p[i+2]);
    	    Vector const& d = (i == 0) ? p[p.size()-1] : p[i-1];
    	    vertices.push_back(Segment1(a, b, c, d));
    	}
    }    
    int cnt = 0;
    for (int i = 0; i < vertices.size(); ++i) { 
        if (less(vertices[i].a.y, o.y))
            std::swap(vertices[i], vertices[cnt++]);
    }
    std::sort(vertices.begin(), vertices.begin() + cnt, 
            [o](const Segment1& a, const Segment1& b) {
                return orientation(a.a, o, b.a) == LEFT || 
                    (orientation(a.a, o, b.a) == COLLINEAR && (a.a-o).len2() < (b.a-o).len2());    
            });
    std::sort(vertices.begin() + cnt, vertices.end(), 
            [o](const Segment1& a, const Segment1& b) {
                return orientation(a.a, o, b.a) == LEFT || 
                    (orientation(a.a, o, b.a) == COLLINEAR && (a.a-o).len2() < (b.a-o).len2());
            });
    
    Geo::Vector dirV;
 	std::set<Segment1, std::function<bool(Segment1 const&, Segment1 const&)>> set([o, &dirV](Segment1 const& s1, Segment1 const& s2) {
 	                            Geo::Vector pt;
 	                            Line l(o, dirV-o);
 	                            assert(intersect(l, s1, pt));
 	                            double a = (o-pt).len2();
 	                            assert(intersect(l, s2, pt));
 	                            double b = (o-pt).len2();
 	                            assert((Segment)s1 == (Segment)s2 || !Geo::equals(a, b));
 	                            return Geo::less(a, b);
 							});
 	
 	std::vector<Vector> candidates;
   	
   	{//init st.
    	Vector const& st = vertices[0].a;
    	dirV = st;//!!Important for comparator!!
    	Line l(o, (st-o));
    	Vector pt;
    	for (Polygon const& p : polygons) {
    		for (int i = 0; i < p.size(); ++i) {
    			Segment1 s(p[i], i == p.size() - 1 ? p[0] : p[i+1], (i == p.size() - 1) ? p[1] : (i == p.size() - 2 ? p[0] : p[i+2]), i == 0 ? p[p.size()-1] : p[i-1]);
    			if (!intersect(l, s, pt))
    				continue;
    			if (s.a != pt && greater(l.v^(pt-o), 0)) {
    				add(set, o, s);
    			}    			
    		}
    	}
    }

    for (int i = 0; i < (int)vertices.size(); ++i) {
 		Segment1 const& v = vertices[i];
 		dirV = v.a;//!!Important for comparator!!
 		int j = i;
 		bool isAns = false;
 		Vector ans;
 		while (j < vertices.size() && orientation(vertices[j].a, o, vertices[i].a) == COLLINEAR) {
 			bool now = remove(set, o, vertices[j]);
 			if (!isAns && now) {
 				isAns = true;
 				ans = vertices[j].a;
 			}
 			j++;
 		}
 		if ((i == 0 || orientation(vertices[i-1].a, o, vertices[i].a) != COLLINEAR)) {
     			Vector nearest;
            	bool have = isAns;
     			if (!isAns) {
         			assert(set.size() > 0);
             		Segment const& nearestSeg = *(set.begin());
             		double val = std::max((nearestSeg.a-o).len2(), (nearestSeg.b-o).len2());
             		if (!less(val, (o-v.a).len2())) {
             			Line l(o, (v.a-o));            		            		
                		assert(intersect(l, nearestSeg, nearest));
                		have = true;
                	}
                } else nearest = ans;

            	if (have && !less((nearest-o).len2(), (v.a-o).len2())) {
                	if (nearest == v.a) {
               	 		candidates.push_back(v.a);
               	 	} else if (orientation(nearest, v.a, v.b) == LEFT) {
                    	candidates.push_back(nearest); 
                    	candidates.push_back(v.a);
                   	} else {
                   		candidates.push_back(v.a); 
                   		candidates.push_back(nearest); 
                   	}			
                }
    	}
    	add(set, o, v);
    }
    return Polygon(candidates);
}

Polygon Geo::visibilityPolygon (Vector o, std::vector<Polygon> polygons, double w, double h) {
	for (Polygon& p : polygons) {
    	assert(p.size() > 1);
    	if (p.size() > 2)
    		p.makeCCW();
    	else {
    		if (orientation(p.points[0], o, p.points[1]) == RIGHT)
    			std::swap(p.points[0], p.points[1]);    
    	}
	}
	Polygon p;
    p.addPoint(Vector(0, 0));
    p.addPoint(Vector(0, h));
    p.addPoint(Vector(w, h));
    p.addPoint(Vector(w, 0));
    polygons.push_back(p);
    return visibilityPolygonFast (o, polygons);    
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

bool Geo::intersect(Polygon p1, Polygon p2, Polygon& out) {
	boost::geometry::correct(p1.points);
	boost::geometry::correct(p2.points);
	std::vector<std::vector<Geo::Vector>> tmp;
	boost::geometry::intersection(p1.points, p2.points, tmp);
	if (tmp.size() == 0)
		return false;
	out = Polygon(tmp[0]);	
	return true;
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

double Geo::distance(Vector a, Polygon b) {
	boost::geometry::correct(a);
	boost::geometry::correct(b.points);
	return boost::geometry::distance(a, b.points);
}

double Geo::distance(Vector a, Segment b) {
	boost::geometry::correct(a);
	boost::geometry::correct(b);
	return boost::geometry::distance(a, b);
}

bool Geo::intersects(Box const& a, Box const& b) {
	return boost::geometry::intersects(a, b);
}
