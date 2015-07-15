#ifndef IStaticObject_H
#define IStaticObject_H
#include "IReal.h"

struct IStaticObject : IReal {
public:
	virtual ~IStaticObject(){};
	virtual void activate() = 0;
	virtual void deactivate() = 0;
	virtual void frame() = 0;                
};

#endif