#ifndef BOOSTING_H
#define BOOSTING_H

#include <boost\geometry.hpp>
#include "Geometry.h"


//Point
namespace boost {
    namespace geometry {
        namespace traits {
            template<>
            struct tag<Geo::Vector> { typedef point_tag type; };

            template<>
            struct coordinate_type<Geo::Vector>{ typedef double type; };

            template<>
            struct coordinate_system<Geo::Vector>
            { typedef cs::cartesian type; };

            template<>
            struct dimension<Geo::Vector> : boost::mpl::int_<2> {};

            template<>
            struct access<Geo::Vector, 0> {
                static double get(Geo::Vector const& p) {
                    return p.x;
                }

                static void set(Geo::Vector& p, double const& value) {
                    p.x = value;
                }
            };

            template<>
            struct access<Geo::Vector, 1> {
                static double get(Geo::Vector const& p) {
                    return p.y;
                }

                static void set(Geo::Vector& p, double const& value) {
                    p.y = value;
                }
            };
        }
    }
}

//Segment
/*
namespace boost {
	namespace geometry {
		namespace traits {
			template<>
            struct tag<Geo::Segment> { typedef segment_tag type; };
            
            template<>
            struct point_type<Geo::Segment>{ typedef Geo::Vector type; };

            template<>
            struct indexed_access<Geo::Segment, 0> {
                static Geo::Vector get(Geo::Segment const& s) {
                    return s.a;
                }

                static Geo::Vector set(Geo::Segment& s, Geo::Vector const& value) {
                    s.a = value;
                }
            };

            template<>
            struct indexed_access<Geo::Segment, 1> {
                static Geo::Vector get(Geo::Segment const& s) {
                    return s.b;
                }

                static Geo::Vector set(Geo::Segment& s, Geo::Vector const& value) {
                    s.b = value;
                }
            };

		}
	}
}
*/

#endif
