#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <iostream>
#include <algorithm>


namespace geo {
    enum Orientation {
        CLOCKWISE = -1,
        COUNTERCLOCKWISE = 1,
        COLLINEAR = 0
    };

    template<typename EXACT_PREDICATE_TYPE> 
    struct Vector {
        
        Vector();
        Vector(EXACT_PREDICATE_TYPE const& x, EXACT_PREDICATE_TYPE const& y);

        Vector<EXACT_PREDICATE_TYPE> operator+ (Vector<EXACT_PREDICATE_TYPE> const& other) const;
        Vector<EXACT_PREDICATE_TYPE>& operator+= (Vector<EXACT_PREDICATE_TYPE> const & other);

        Vector<EXACT_PREDICATE_TYPE> operator- (Vector<EXACT_PREDICATE_TYPE> const & other) const;
        Vector<EXACT_PREDICATE_TYPE>& operator-= (Vector<EXACT_PREDICATE_TYPE> const & other);
 
        template<typename T>
        Vector<EXACT_PREDICATE_TYPE> operator* (T const& c) const;

        template<typename T>
        Vector<EXACT_PREDICATE_TYPE>& operator*= (T const& c);
        
        template<typename T>
        Vector<EXACT_PREDICATE_TYPE> operator/ (T const& c) const;

        template<typename T>
        Vector<EXACT_PREDICATE_TYPE>& operator/= (T const& c);
                                                                   
        bool operator==(Vector<EXACT_PREDICATE_TYPE> const& other) const;
        
        bool operator!=(Vector<EXACT_PREDICATE_TYPE> const& other) const;
                                                                       
        EXACT_PREDICATE_TYPE operator*(Vector<EXACT_PREDICATE_TYPE> const& other) const;
        EXACT_PREDICATE_TYPE operator^(Vector<EXACT_PREDICATE_TYPE> const& other) const;
        
        EXACT_PREDICATE_TYPE x;
        EXACT_PREDICATE_TYPE y;

        typedef EXACT_PREDICATE_TYPE coordinate_type;                
    };  

    template<typename VECTOR_TYPE>
    struct Segment {
        Segment();
        Segment(VECTOR_TYPE const& a, VECTOR_TYPE const& b);
        bool operator==(Segment<VECTOR_TYPE> const& other) const;

        VECTOR_TYPE a;
        VECTOR_TYPE b;
        
        typedef VECTOR_TYPE vector_type;
    };

    template<typename VECTOR_TYPE>
    struct Line {
        Line();
        Line(VECTOR_TYPE const& a, VECTOR_TYPE const& b);
        typename VECTOR_TYPE::coordinate_type a;
        typename VECTOR_TYPE::coordinate_type b;
        typename VECTOR_TYPE::coordinate_type c;

        typedef VECTOR_TYPE vector_type;
    };

    template<typename VECTOR_TYPE>
    struct Ray : Line<VECTOR_TYPE> {
        Ray();
        Ray(VECTOR_TYPE const& a, VECTOR_TYPE const& b);//dir from a to b
        VECTOR_TYPE dir;
        VECTOR_TYPE start;
        
        typedef VECTOR_TYPE vector_type;
    };

    
    template<typename VECTOR_TYPE>
    struct Box {
        Box();
        Box(VECTOR_TYPE const& minCor, VECTOR_TYPE const& maxCor);
        VECTOR_TYPE minCor;
        VECTOR_TYPE maxCor;
    };

    template<typename VECTOR_TYPE>
    struct Ring : std::vector<VECTOR_TYPE> {
    	using std::vector<VECTOR_TYPE>::vector;
   		typedef VECTOR_TYPE vector_type;   		    
    };

    template<typename VECTOR_TYPE>
    struct Polygon {
    	Polygon(Ring<VECTOR_TYPE> ering, std::vector<Ring<VECTOR_TYPE>> irings) : ering(ering), irings(irings) {}
    	Polygon(Ring<VECTOR_TYPE> ring) : ering(ring) {}
    	Ring<VECTOR_TYPE> ering;
    	std::vector<Ring<VECTOR_TYPE>> irings;
    	typedef Ring<VECTOR_TYPE> ring_type;
    };


    template<typename VECTOR_TYPE>
    Orientation orientation (VECTOR_TYPE const& a, VECTOR_TYPE const& b, VECTOR_TYPE const& c);// orientation b of a and c.
    
    template<typename VECTOR_TYPE>
    Orientation orientation (Ring<VECTOR_TYPE> const& ring);// orientation b of a and c.
    
    template<typename EXACT_PREDICATE_TYPE>
    EXACT_PREDICATE_TYPE det (EXACT_PREDICATE_TYPE a, EXACT_PREDICATE_TYPE b, EXACT_PREDICATE_TYPE c, EXACT_PREDICATE_TYPE d);



    template<typename LINEAR_TYPE1, typename LINEAR_TYPE2>
    bool parallel (LINEAR_TYPE1 const& a, LINEAR_TYPE2 const& b);
    
    template<typename GEOMETRY1, typename GEOMETRY2>
    bool intersects (GEOMETRY1 const& a, GEOMETRY2 const& b);
    
    template<typename GEOMETRY1, typename GEOMETRY2, typename OUT>
    OUT distance2 (GEOMETRY1 const& a, GEOMETRY2 const& b);
    
    template<typename LINEAR_TYPE1, typename LINEAR_TYPE2, typename OUT>
    bool intersection (LINEAR_TYPE1 const& a, LINEAR_TYPE2 const& b, OUT& res, bool consider_touch);
    
    template<typename VECTOR_TYPE>
    void visibilityPolygon(VECTOR_TYPE const& o, Polygon<VECTOR_TYPE> const& polygon, Ring<VECTOR_TYPE>& res);
                                                                    
//======Implementation=======    
    template <typename T>
    Vector<T>::Vector() {}

    template <typename T>
    Vector<T>::Vector(T const& x, T const& y) : x(x), y(y) {}

    template <typename T>
    Segment<T>::Segment() {}

    template <typename T>
    Segment<T>::Segment(T const& a, T const& b) : a(a), b(b) {}

	template <typename T>
    bool Segment<T>::operator==(Segment<T> const& other) const {
    	return a==other.a && b == other.b;
    }

    template <typename T>
    Line<T>::Line() {}

    template <typename T>
    Line<T>::Line(T const& p1, T const& p2) {
        this->a = p1.y - p2.y;
        this->b = p2.x - p1.x;
        this->c = -a*p1.x - b*p1.y;
    }

    template <typename T>
    Ray<T>::Ray() : Line<T>() {}

    template <typename T>
    Ray<T>::Ray(T const& p1, T const& p2) : Line<T>(p1, p2) {
        dir = p2 - p1;
        start = p1;
    }

    template <typename T>
    Box<T>::Box() {}

    template <typename T>
    Box<T>::Box(T const& a, T const& b) {
        this->minCor = a;
        this->maxCor = b;
    }

    template <typename T>
    Vector<T> Vector<T>::operator+(Vector<T> const& other) const {
        return Vector<T>(x+other.x, y+other.y);
    }

    template <typename T>
    Vector<T>& Vector<T>::operator+=(Vector<T> const& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    template <typename T>
    Vector<T> Vector<T>::operator-(Vector<T> const& other) const {
        return Vector<T>(x-other.x, y-other.y);
    }

    template <typename T>
    Vector<T>& Vector<T>::operator-=(Vector<T> const& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }


    template <typename T>
    T Vector<T>::operator*(Vector<T> const& other) const {
        return x*other.y - y*other.x;
    }

    template <typename T>
    T Vector<T>::operator^(Vector<T> const& other) const {
        return x*other.x + y*other.y;
    }

    template <typename T>
    template <typename E>
    Vector<T> Vector<T>::operator*(E const& k) const {
        return Vector<T>(x*k, y*k);
    }

    template <typename T>
    template <typename E>
    Vector<T>& Vector<T>::operator*=(E const& k) {
        x*=k;
        y*=k;
        return *this;
    }

    template <typename T>
    template <typename E>
    Vector<T> Vector<T>::operator/(E const& k) const {
        return Vector<T>(x/k, y/k);
    }

    template <typename T>
    template <typename E>
    Vector<T>& Vector<T>::operator/=(E const& k) {
        x/=k;
        y/=k;
        return *this;
    }

    template <typename T>
    bool Vector<T>::operator==(Vector<T> const& other) const {
        return other.x == x && other.y == y;
    }
    
    template <typename T>    
    bool Vector<T>::operator!=(Vector<T> const& other) const {
        return other.x != x || other.y != y;
    }
        
    //=======================

    template<typename T>
    Orientation orientation (T const& a, T const& b, T const& c) {
        typename T::coordinate_type res = (a-b)*(c-b);
        if (res == 0)
            return Orientation::COLLINEAR;
        if (res > 0)
            return Orientation::COUNTERCLOCKWISE;
        return Orientation::CLOCKWISE;
    }

    template<typename T> 
    Orientation orientation(Ring<T> const& ring) {
    	typename T::coordinate_type sum = 0;
    	for (int i = 0; i < ring.size() - 1; ++i) {
        	T const& a = ring[i];
        	T const& b = ring[i+1];
        	sum += (b.x-a.x)*(b.y+a.y);
    	}
    	if (sum < 0)    
        	return Orientation::COUNTERCLOCKWISE;
    	return Orientation::CLOCKWISE;
    }

    template<typename T>
    T det (T a, T b, T c, T d) {
        return a * d - b * c;
    }

    template<typename T>
    T distance2(Vector<T> const& a, Vector<T> const& b) {
    	return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
    }
    
    template<typename T>
    bool parallel (Line<Vector<T>> const& m, Line<Vector<T>> const& n) {
        T zn = det(m.a, m.b, n.a, n.b);
        if (zn == 0)
            return true;
        return false;
    }
    
    template<typename T>
    bool intersects (Box<Vector<T>> const& a, Box<Vector<T>> const& b) {
    	if (a.minCor.x > b.maxCor.x || a.minCor.y > b.maxCor.y 
    		|| a.maxCor.x < b.minCor.x || a.maxCor.y < b.maxCor.y)
    		return false;
    	return true; 
    }

    template<typename T>
    bool intersection (Line<Vector<T>> const& m, Line<Vector<T>> const& n, Vector<T>& res, bool consider_touch=true) {
        T zn = det(m.a, m.b, n.a, n.b);
        if (zn == 0)
            return false;
        res.x = -det(m.c, m.b, n.c, n.b) / zn;
        res.y = -det(m.a, m.c, n.a, n.c) / zn;
        return true;    
    }
    
    template<typename T>
    bool intersection (Line<Vector<T>> const& l, Segment<Vector<T>> const& s, Vector<T>& res, bool consider_touch=true) {
        if (!intersection(l, Line<Vector<T>>(s.a, s.b), res, true))
            return false;
        T a = (l.c+l.a*s.a.x) + s.a.y*l.b;
        a = (a > 0) ? 1 : (a == 0 ? 0 : -1);
        T b = (l.c+l.a*s.b.x) + s.b.y*l.b;
        b = (b > 0) ? 1 : (b == 0 ? 0 : -1);
        if (a*b > 0 || (a*b == 0 && !consider_touch))
            return false;
        return true;
    }
    
    template<typename T>
    bool intersection (Ray<Vector<T>> const& l, Segment<Vector<T>> const& s, Vector<T>& res, bool consider_touch=true) {
        if (!intersection((Line<Vector<T>>)l, s, res, consider_touch))
            return false;
        if ((l.dir^(res - l.start)) > 0)
        	return true;
        return false;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& str, Vector<T> const& v) {
    	return str << "{" << v.x << ", " << v.y << "}";
    }
    
    template<typename T>
    std::ostream& operator<<(std::ostream& str, Segment<T> const& v) {
    	return str << "{" << v.a << ", " << v.b << "}";
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& str, Ring<T> const& ring) {
    	str << "{ ";
    	for (Vector<T> const& v : ring)
    		str << v << ", ";
    	str << "}";    	
    	return str;
    }

    template<typename T>
    void correct(Ring<T>& ring) {
    	if (ring[0] != ring.back())
    		ring.push_back(ring[0]);
    	if (orientation(ring) == Orientation::CLOCKWISE) {
    		std::reverse(ring.begin(), ring.end());
    	}
    }
    
    template<typename T>
    void correct(Polygon<T>& poly) {
  		correct(poly.ering);
  		for (Ring<T>& iring : poly.irings)
  			correct(iring);
  	}
}

#include "newVisibilityPolygon.cpp"

#endif


