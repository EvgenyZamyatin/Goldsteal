#include "Geometry.h"

using namespace Geo;

struct Segment1 : Segment {
	Segment1(Vector const& a, Vector const& b, Vector const& d) : Segment(a, b), d(d) {}
	Segment1(Vector const& a, Vector const& b) : Segment(a, b) {}
	Vector d; // point before a
}; 

struct Ray : Line {
	Ray(Vector a, Vector b) : Line(a, b), v(b-a) {}
	Vector v;
};

void add(std::set<Segment1, std::function<bool(Segment1 const&, Segment1 const&)>>& set, Vector const& o, Segment1 const& s) {
	if (orientation(s.a, o, s.b) == LEFT)
		set.insert(s);
} 

/*bool touch (Vector const& o, Segment1 const& s) {
	return !(orientation(o, s.b, s.a) * orientation(o, s.b, s.c) == -1);	
}*/ 

void remove(std::set<Segment1, std::function<bool(Segment1 const&, Segment1 const&)>>& set, Vector const& o, Segment1 const& s) {
	if (orientation(s.d, o, s.a) != LEFT)
		return;
	Segment1 s1(s.d, s.a);
	auto it = set.lower_bound(s1);
	if ((*it).a==s1.a && (*it).b==s1.b) {
		s1=*it;
		set.erase(it);
	}
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
    	    Vector const& d = (i == 0) ? p[p.size()-1] : p[i-1];
    	    vertices.push_back(Segment1(a, b, d));
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
    			Segment1 s(p[i], i == p.size() - 1 ? p[0] : p[i+1], i == 0 ? p[p.size()-1] : p[i-1]);
    			if (!intersect(l, s, pt, false)) 
    				continue;
    			if ((l.v^(pt-o)) > 0) 
    				add(set, o, s);
    		}
    	}
    }
    
    for (int i = 0; i < (int)vertices.size(); ++i) {
 		if (i != 0 && orientation(vertices[i-1].a, o, vertices[i].a) == COLLINEAR)
 			continue;

 		Segment1 const& v = vertices[i];
 		dirV = v.a;//!!Important for comparator!!

 		int j = i;
 		while (j < vertices.size() && orientation(vertices[j].a, o, vertices[i].a) == COLLINEAR) {
 			remove(set, o, vertices[j]);
 			j++;
 		}
 		Vector nv1;
 		bool ok1=false;
 		if (set.size() > 0) {
 			Segment const& s = *(set.begin());
 			Ray l(o, v.a);            		            		
    		#ifdef DEBUG
 				assert(intersect(l, s, nv1));
 			#else
 				intersect(l, s, nv1)
 			#endif		
 			ok1 = true;
 		}

 		Vector nv2;
 		bool ok2=false;
 		j = i; 		
 		while (j < vertices.size() && orientation(vertices[j].a, o, vertices[i].a) == COLLINEAR) {
 			add(set, o, vertices[j]);
 			if (!ok2) {
 				int a = orientation(vertices[j].a, o, vertices[j].d);
 				int b = orientation(vertices[j].a, o, vertices[j].b);
 				if (a*b==-1 || 
 					(a==COLLINEAR && b == LEFT && ((vertices[j].a-vertices[j].d)^(o-vertices[j].d)) < 0) || 
 					(b==COLLINEAR && a == RIGHT && ((vertices[j].a-vertices[j].b)^(o-vertices[j].b)) < 0)) {
 					ok2 = true;
 					nv2 = vertices[j].a;
 				}
 					
 			}
 			j++;
 		}
 		assert(ok1 || ok2);
 		Geo::Vector nearest;
 		if (ok1 && ok2) {
 			nearest = (o-nv1).len2() < (o-nv2).len2() ? nv1 : nv2;
 		} else if (ok1) {
 			nearest = nv1;
 		} else {
 		    nearest = nv2;
 		}
 		
 		if ((nearest-o).len2() >= (v.a-o).len2()) {
        	if (nearest == v.a) {
       	 		candidates.push_back(v.a);
       	 	} else if (orientation(v.a, o, v.b) == LEFT) {
            	candidates.push_back(nearest); 
            	candidates.push_back(v.a);
           	} else {
           		candidates.push_back(v.a); 
           		candidates.push_back(nearest); 
           	}			
        }
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
