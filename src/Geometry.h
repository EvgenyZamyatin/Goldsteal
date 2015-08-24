#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "geometry/newGeometry.h"

typedef geo::Vector<long long> Vector;
typedef geo::Ring<Vector> Ring;
typedef geo::Polygon<Vector> Poly;
typedef geo::Segment<Vector> Segment;
typedef geo::Line<Vector> Line;
typedef geo::Ray<Vector> Ray;
typedef geo::Box<Vector> Box;

#endif
