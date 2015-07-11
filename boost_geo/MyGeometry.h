#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <boost/polygon/polygon.hpp>
#include <vector>
namespace gtl = boost::polygon;
using namespace boost::polygon::operators;

struct Point {
    double x;
    double y;
};

namespace boost {
    namespace polygon {
        template<>
        struct geometry_concept<Point> {typedef point_concept type;};
        
        template <>
        struct point_traits<Point> {
            typedef int coordinate_type;
            static inline coordinate_type get(const Point& point, 
                    orientation_2d orient) {
                if(orient == HORIZONTAL)
                    return point.x;
                return point.y;
            }
        };

        template <>
        struct point_mutable_traits<Point> {
            typedef int coordinate_type;
            static inline void set(Point& point, orientation_2d orient, int value) {
                if(orient == HORIZONTAL)
                    point.x = value;
                else
                    point.y = value;
            }
            static inline Point construct(int x_value, int y_value) {
                Point retval;
                retval.x = x_value;
                retval.y = y_value; 
                return retval;
            }
        };
    }
}

typedef std::vector<Point> SPolygon;
namespace boost {
    namespace polygon {
        template <>
        struct geometry_concept<SPolygon>{ typedef polygon_concept type; };
        
        template <>
        struct polygon_traits<SPolygon> {
            typedef int coordinate_type;
            typedef SPolygon::const_iterator iterator_type;
            typedef Point point_type;
            static inline iterator_type begin_points(const SPolygon& t) {
                return t.begin();
            }
            static inline iterator_type end_points(const SPolygon& t) {
                return t.end();
            }
            static inline std::size_t size(const SPolygon& t) {
                return t.size();
            }
            static inline winding_direction winding(const SPolygon& t) {
                return unknown_winding;
            }
        };
        template <>
        struct polygon_mutable_traits<SPolygon> {
            template <typename iT>
            static inline SPolygon& set_points(SPolygon& t, 
                    iT input_begin, iT input_end) {
                t.clear();
                t.insert(t.end(), input_begin, input_end);
                return t;
            }
        };
    }
}

typedef std::deque<SPolygon> PolygonSet;
namespace boost { 
    namespace polygon {
        template <>
        struct geometry_concept<PolygonSet> { typedef polygon_set_concept type; };

        template <>
        struct polygon_set_traits<PolygonSet> {
            typedef int coordinate_type;
            typedef PolygonSet::const_iterator iterator_type;
            typedef PolygonSet operator_arg_type;

            static inline iterator_type begin(const PolygonSet& polygon_set) {
                return polygon_set.begin();
            }

            static inline iterator_type end(const PolygonSet& polygon_set) {
                return polygon_set.end();
            }

            static inline bool clean(const PolygonSet& polygon_set) { return false; }
            static inline bool sorted(const PolygonSet& polygon_set) { return false; }
        };

        template <>
        struct polygon_set_mutable_traits<PolygonSet> {
            template <typename input_iterator_type>
            static inline void set(PolygonSet& polygon_set, 
                    input_iterator_type input_begin, input_iterator_type input_end) {
                polygon_set.clear();
                polygon_set_data<int> ps;
                ps.insert(input_begin, input_end);
                ps.get(polygon_set);
            }
        };
    } 
}

/*
struct Polygon {
    SPolygon bound;
    std::vector<SPolygon> holes;
};

namespace boost {
    namespace polygon {
        template <>
        struct polygon_with_holes_traits<Polygon> {
            typedef std::vector<SPolygon>::const_iterator iterator_holes_type;
            typedef SPolygon hole_type;
            static inline iterator_holes_type begin_holes(const Polygon& t) {
                return t.holes.begin();
            }
            static inline iterator_holes_type end_holes(const Polygon& t) {
                return t.holes.end();
            }
            static inline unsigned int size_holes(const Polygon& t) {
                return t.holes.size();
            }
        };

        template <>
        struct polygon_with_holes_mutable_traits<Polygon> {
            template <typename iT>
            static inline Polygon& set_holes(Polygon& t, iT inputBegin, iT inputEnd) {
                t.holes.clear();
                t.holes.insert(inputBegin, inputEnd);                
                return t;
            }
        };
    }
}
*/




#endif
