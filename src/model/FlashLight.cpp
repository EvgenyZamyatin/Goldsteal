#include "FlashLight.h"
#include "Environment.h"

void FlashLight::frame() {
	pos = owner->getPosition() + shift;
	Geo::Polygon tmp(env->calcVisible(pos));
	
	#ifdef DEBUG
		assert(tmp.order() == Geo::CLOCKWISE);
	#endif
	std::vector<Geo::Vector> pts;
	pts.push_back(pos);
	Geo::Vector dirA(owner->getDir());
	dirA *= 10;
	dirA.rotate(sn, cs);
	Geo::Line a(pos, pos+dirA);
	
	Geo::Vector dirB(owner->getDir());
	dirB *= 10;
	dirB.rotate(-sn, cs);
	Geo::Line b(pos, pos+dirB);
	
	int i = 0;
	Geo::Vector pt;
	for (; ; (i==tmp.size() - 1) ? i=0 : ++i) {
		Geo::Segment s(tmp[i], tmp[i+1]);
		bool ok = intersect(a, s, pt, false);
		if (!ok) {
			if (a.contains(tmp[i+1])) {
				ok = true;
				pt = tmp[i+1];	
			}
		}
		if (ok && ((pt-pos)^dirA) > 0) {
			pts.push_back(pt);
			if (a.contains(tmp[i+1]))
				++i;
			break;
		}
	}
	
	for (; ; (i==tmp.size() - 1) ? i=0 : ++i) {
		Geo::Segment s(tmp[i], tmp[i+1]);
		bool ok = intersect(b, s, pt, false);
		if (!ok) {
			if (b.contains(tmp[i+1])) {
				ok = true;
				pt = tmp[i+1];	
			}
		}
		if (ok && ((pt-pos)^dirB) > 0) {
			pts.push_back(pt);
			break;
		}
		pts.push_back(tmp[i+1]);
	}

	bounds = Geo::Polygon(pts);
}


void FlashLight::setEnvironment(Environment* env) {
	this->env = env;
}