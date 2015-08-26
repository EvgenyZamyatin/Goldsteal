#include "../FlashLight.h"
#include "../Environment.h"
#include "../Vertex.h"

void FlashLight::frame() {
	pos = owner->getPosition();
	geo::Ring<Vertex> tmp;
	
	env->findVisibility(pos, tmp);
	
	#ifdef DEBUG
		assert(geo::orientation(tmp) == geo::Orientation::CLOCKWISE);
	#endif

	geo::Ring<Vertex> pts;
	pts.push_back(pos);
	Vertex dirA(owner->getDir());
	dirA *= 10;
	geo::rotate(dirA, sn, cs);
	geo::Ray<Vertex> a(pos, pos+dirA);
	
	Vertex dirB(owner->getDir());
	dirB *= 10;
	geo::rotate(dirB, -sn, cs);
	geo::Ray<Vertex> b(pos, pos+dirB);
	
	int i = 0;
	Vertex pt;
	for (; ; (i==tmp.size() - 2) ? i=0 : ++i) {
		geo::Segment<Vertex> s(tmp[i], tmp[i+1]);
		bool ok = geo::intersection(a, s, pt, false);
		if (!ok) {
			if (geo::contains(a, tmp[i+1])) {
				ok = true;
				pt = tmp[i+1];	
			}
		}
		if (ok) {
			pts.push_back(pt);
			if (geo::contains(a, tmp[i+1]))
				++i;
			break;
		}
	}
	
	for (; ; (i==tmp.size() - 2) ? i=0 : ++i) {
		geo::Segment<Vertex> s(tmp[i], tmp[i+1]);
		bool ok = geo::intersection(b, s, pt, false);
		if (!ok) {
			if (geo::contains(b, tmp[i+1])) {
				ok = true;
				pt = tmp[i+1];	
			}
		}
		if (ok) {
			pts.push_back(pt);
			break;
		}
		pts.push_back(tmp[i+1]);
	}

	bounds = pts;
	geo::correct(bounds);
}


void FlashLight::setEnvironment(Environment const* env) {
	this->env = env;
}