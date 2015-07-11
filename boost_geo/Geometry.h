#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<double> Point;
typedef bg::model::polygon<Point> Polygon;
typedef bg::model::linestring<Point> Linestring;
typedef bg::model::segment<Point> Segment;
#endif
