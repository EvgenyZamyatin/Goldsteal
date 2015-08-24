#include "newGeometry.h"
#include <set>
#include <functional>
#include <algorithm>
#include <assert.h>

namespace geo {

template<typename T>
struct Segment1 : Segment<T> {
    Segment1(T const& a, T const& b, T const& d) : Segment<T>(a, b), d(d) {}
    Segment1(T const& a, T const& b) : Segment<T>(a, b) {}
    T d; // point before a
}; 

template<typename T>
void add(std::set<Segment1<T>, std::function<bool(Segment1<T> const&, Segment1<T> const&)>>& set, T const& o, Segment1<T> const& s) {
    if (orientation(s.a, o, s.b) == Orientation::CLOCKWISE)
        set.insert(s);
} 

template<typename T>
void remove(std::set<Segment1<T>, std::function<bool(Segment1<T> const&, Segment1<T> const&)>>& set, T const& o, Segment1<T> const& s) {
    if (orientation(s.d, o, s.a) != Orientation::CLOCKWISE)
        return;
    Segment1<T> s1(s.d, s.a);
    auto it = set.lower_bound(s1);
    if ((*it).a==s1.a && (*it).b==s1.b) {
        s1=*it;
        set.erase(it);
    }
} 

template<typename T>
void check(T const& o, std::vector<Segment1<T>> const& v) {
    for (int i = 0; i < (int)v.size() - 1; ++i) {
        assert(orientation(v[i].a, o, v[i+1].a) != Orientation::COUNTERCLOCKWISE);
    }
}


template<typename T>
void visibilityPolygon (T const& o, Polygon<T> const& polygon, Ring<T>& out) {
	#ifdef DEBUG
		assert(orientation(polygon.ering) == Orientation::COUNTERCLOCKWISE);
		assert(polygon.ering[0] == polygon.ering.back());
		for (auto ring : polygon.irings) {
			assert(orientation(ring) == Orientation::COUNTERCLOCKWISE);
			assert(ring[0] == ring.back());
		}
	#endif
		
    std::vector<Segment1<T>> vertices;                  

    for (Ring<T> const& ring : polygon.irings) {
        for (int i = 0; i < ring.size() - 1; ++i) {
            T const& a = ring[i]; 
            T const& b = ring[i+1]; 
            T const& d = (i == 0) ? ring[ring.size() - 2] : ring[i-1];
            vertices.push_back(Segment1<T>(a, b, d));
        }
    }    
    
   	{
		Ring<T> const& ring = polygon.ering;
		for (int i = 0; i < ring.size() - 1; ++i) {
            T const& a = ring[i]; 
            T const& d = ring[i+1]; 
            T const& b = (i == 0) ? ring[ring.size() - 2] : ring[i-1];
            vertices.push_back(Segment1<T>(a, b, d));
        }
    }    
    

    
    int cnt = 0;
    for (int i = 0; i < vertices.size(); ++i) { 
        if (vertices[i].a.y < o.y || (vertices[i].a.y == o.y && vertices[i].a.x < o.x))
            std::swap(vertices[i], vertices[cnt++]);
    }
    std::sort(vertices.begin(), vertices.begin() + cnt, 
            [o](Segment1<T> const& a, Segment1<T> const& b) {
                return orientation(a.a, o, b.a) == Orientation::CLOCKWISE || 
                    (orientation(a.a, o, b.a) == Orientation::COLLINEAR && distance2(a.a, o) < distance2(b.a, o));    
            });
    std::sort(vertices.begin() + cnt, vertices.end(), 
            [o](Segment1<T> const& a, Segment1<T> const& b) {
                return orientation(a.a, o, b.a) == Orientation::CLOCKWISE || 
                    (orientation(a.a, o, b.a) == Orientation::COLLINEAR && distance2(a.a, o) < distance2(b.a, o));    
            });

    #ifdef DEBUG
        check(o, vertices);
    #endif

    T dirV;
    std::set<Segment1<T>, std::function<bool(Segment1<T> const&, Segment1<T> const&)>> set([o, &dirV](Segment1<T> const& s1, Segment1<T> const& s2) {
                                T pt;
                                Ray<T> l(o, dirV);
                                #ifdef DEBUG
                                    assert(intersection(l, (Segment<T>)s1, pt));
                                #else                                   
                                    intersection(l, (Segment<T>)s1, pt);
                                #endif
                                typename T::coordinate_type a = distance2(o, pt);
                                #ifdef DEBUG
                                    assert(intersection(l, (Segment<T>)s2, pt));
                                #else 
                                    intersection(l, (Segment<T>)s2, pt);
                                #endif
                                typename T::coordinate_type b = distance2(o, pt);
                                #ifdef DEBUG
                                    assert((Segment<T>)s1 == (Segment<T>)s2 || a != b);
                                #endif
                                return a < b;
                            });
    
    {//init st.
        T const& st = vertices[0].a;
        dirV = st;//!!Important for comparator!!
        Ray<T> l(o, st);
        T pt;
        for (Ring<T> const& p : polygon.irings) {
            for (int i = 0; i < p.size() - 1; ++i) {
                Segment1<T> s(p[i], p[i+1], i == 0 ? p[p.size() - 2] : p[i-1]);
                if (!intersection(l, (Segment<T>)s, pt, false)) 
                    continue;
                add(set, o, s);
            }
        }

        Ring<T> const& p = polygon.ering;
        for (int i = 0; i < p.size() - 1; ++i) {
            Segment1<T> s(p[i], i == 0 ? p[p.size() - 2] : p[i-1], p[i+1]);
            if (!intersection(l, (Segment<T>)s, pt, false)) 
                continue;
            add(set, o, s);
        }

    }
    
    for (int i = 0; i < (int)vertices.size(); ++i) {
        if (i != 0 && orientation(vertices[i-1].a, o, vertices[i].a) == Orientation::COLLINEAR)
            continue;

        Segment1<T> const& v = vertices[i];
        dirV = v.a;//!!Important for comparator!!

        int j = i;
        while (j < vertices.size() && orientation(vertices[j].a, o, vertices[i].a) == Orientation::COLLINEAR) {
            remove(set, o, vertices[j]);
            j++;
        }
        T nv1;
        bool ok1=false;
        if (set.size() > 0) {
            Segment<T> const& s = *(set.begin());
            Ray<T> l(o, v.a);                                      
            #ifdef DEBUG
                assert(intersection(l, s, nv1));
            #else
                intersection(l, s, nv1);
            #endif      
            ok1 = true;
        }

        T nv2;
        bool ok2=false;
        j = i;      
        while (j < vertices.size() && orientation(vertices[j].a, o, vertices[i].a) == Orientation::COLLINEAR) {
            add(set, o, vertices[j]);
            if (!ok2) {
                int a = orientation(vertices[j].a, o, vertices[j].d);
                int b = orientation(vertices[j].a, o, vertices[j].b);
                if (a*b==-1 || 
                    (a==COLLINEAR && b == Orientation::CLOCKWISE && ((vertices[j].a-vertices[j].d)^(o-vertices[j].d)) < 0) || 
                    (b==COLLINEAR && a == Orientation::COUNTERCLOCKWISE && ((vertices[j].a-vertices[j].b)^(o-vertices[j].b)) < 0)) {
                    ok2 = true;
                    nv2 = vertices[j].a;
                }
                    
            }
            j++;
        }
        #ifdef DEBUG
        	assert(ok1 || ok2);
        #endif
        T nearest;
        if (ok1 && ok2) {
            nearest = distance2(o, nv1) < distance2(o, nv2) ? nv1 : nv2;
        } else if (ok1) {
            nearest = nv1;
        } else {
            nearest = nv2;
        }
        
        if (distance2(o, nearest) >= distance2(o, v.a)) {
            if (nearest == v.a) {
                out.push_back(v.a);
            } else if (orientation(v.a, o, v.b) == Orientation::CLOCKWISE) {
                out.push_back(nearest); 
                out.push_back(v.a);
            } else {
                out.push_back(v.a); 
                out.push_back(nearest); 
            }           
        }
    }
    out.push_back(out[0]);
}

}
