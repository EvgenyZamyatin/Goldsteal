#include "Geometry.h"

using namespace Geo;

struct Segment1 : Segment {
	Segment1(Vector const& a, Vector const& b, Vector const& c, Vector const& d) : Segment(a, b), c(c), d(d) {}
	Segment1(Vector const& a, Vector const& b) : Segment(a, b) {}
	Vector c; // point after b
	Vector d; // point before a
}; 

struct Ray : Line {
	Ray(Vector a, Vector b) : Line(a, b), v(b-a) {}
	Vector v;
};

void add(std::set<Segment1, std::function<bool(Segment1 const&, Segment1 const&)>>& set, Vector const& o, Segment1 const& s) {
	if (orientation(s.a, o, s.b) == LEFT) {
		set.insert(s);
	}
} 

bool touch (Vector const& o, Segment1 const& s) {
	return !(orientation(o, s.b, s.a) * orientation(o, s.b, s.c) == -1);	
} 

bool remove(std::set<Segment1, std::function<bool(Segment1 const&, Segment1 const&)>>& set, Vector const& o, Segment1 const& s) {
	if (orientation(s.d, o, s.a) >= 0)
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

void check(Vector const& o, std::vector<Segment1> const& v) {
	for (int i = 0; i < (int)v.size() - 1; ++i) {
		assert(orientation(v[i].a, o, v[i+1].a) <= 0);
	}
}

Polygon Geo::visibilityPolygonFast (Vector const& o, std::vector<Polygon> const& polygons) {
	std::vector<Segment1> vertices;                  
	for (Polygon const& p : polygons) {
    	for (int i = 0; i < p.size(); ++i) {
    	    Vector const& a = p[i]; 
    	    Vector const& b = p[i+1]; 
    	    Vector const& c = (i == p.size() - 1) ? p[1] : (i == p.size() - 2 ? p[0] : p[i+2]);
    	    Vector const& d = (i == 0) ? p[p.size()-1] : p[i-1];
    	    vertices.push_back(Segment1(a, b, c, d));
    	}
    }    
    int cnt = 0;
    for (int i = 0; i < vertices.size(); ++i) { 
        if (vertices[i].a.y < o.y || (vertices[i].a.y == o.y && vertices[i].a.x < o.x))
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

    #ifdef DEBUG
    	check(o, vertices);
   	#endif

    Geo::Vector dirV;
 	std::set<Segment1, std::function<bool(Segment1 const&, Segment1 const&)>> set([o, &dirV](Segment1 const& s1, Segment1 const& s2) {
 	                            Geo::Vector pt;
 	                            Ray l(o, dirV);
 	                            #ifdef DEBUG
 	                            	assert(intersect(l, s1, pt));
 	                            #else 	                            	
 	                            	intersect(l, s1, pt)
 	                            #endif
 	                            int a = (o-pt).len2();
 	                            #ifdef DEBUG
 	                            	assert(intersect(l, s2, pt));
 	                            #else 
 	                            	intersect(l, s2, pt)
 	                            #endif
 	                            int b = (o-pt).len2();
 	                            #ifdef DEBUG
 	                            	assert((Segment)s1 == (Segment)s2 || a != b);
 	                            #endif
 	                            return a < b;
 							});
 	
 	std::vector<Vector> candidates;
   	
   	{//init st.
    	Vector const& st = vertices[0].a;
    	dirV = st;//!!Important for comparator!!
    	Ray l(o, st);
    	Vector pt;
    	for (Polygon const& p : polygons) {
    		for (int i = 0; i < p.size(); ++i) {
    			Segment1 s(p[i], i == p.size() - 1 ? p[0] : p[i+1], (i == p.size() - 1) ? p[1] : (i == p.size() - 2 ? p[0] : p[i+2]), i == 0 ? p[p.size()-1] : p[i-1]);
    			if (!intersect(l, s, pt)) 
    				continue;
    			if (s.a != pt && (l.v^(pt-o)) > 0) {
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
         			#ifdef DEBUG
         				assert(set.size() > 0);
         			#endif
             		Segment const& nearestSeg = *(set.begin());
             		double val = std::max((nearestSeg.a-o).len2(), (nearestSeg.b-o).len2());
             		if (val == (o-v.a).len2() || val > (o-v.a).len2()) {
             			Ray l(o, v.a);            		            		
                		#ifdef DEBUG
         					assert(intersect(l, nearestSeg, nearest));
         				#else
         					intersect(l, nearestSeg, nearest)
         				#endif		
                		have = true;
                	}
                } else nearest = ans;

            	if (have && ((nearest-o).len2() == (v.a-o).len2() || (nearest-o).len2() > (v.a-o).len2())) {
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

Polygon Geo::visibilityPolygon (Vector o, std::vector<Polygon> polygons, int w, int h) {
	for (Polygon& p : polygons) {
        p.makeCCW();
    }
    Polygon p({Vector(0, 0), Vector(0, h), Vector(w, h), Vector(w, 0)});
    polygons.push_back(p);
    return visibilityPolygonFast (o, polygons);
}

Polygon Geo::visibilityPolygon (Vector const& o, std::vector<Polygon> const& polygons) {
	#ifdef DEBUG
    	assert(polygons[0].order() == CLOCKWISE);
    	for (int i = 1; i < polygons.size(); ++i) {
        	assert(polygons[i].order() == COUNTERCLOCKWISE);
    	}
    #endif
	return visibilityPolygonFast (o, polygons);    
}
