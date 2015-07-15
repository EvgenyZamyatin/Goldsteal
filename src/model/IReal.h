#ifndef IREAL_H
#define IREAL_H
#include "../geometry/Geometry.h"

struct IReal {
public:
	virtual ~IReal(){};	
	virtual Geo::Polygon getPosition() = 0;
	virtual Geo::Vector getOrientation() = 0;
};

#endif