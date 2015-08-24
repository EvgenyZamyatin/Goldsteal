#include "FlashLight.h"
#include "Environment.h"

void FlashLight::frame() {
	pos = owner->getPosition() + shift;
	Ring tmp(env->calcVisible(pos));
	
	#ifdef DEBUG
		assert(geo::orientation(tmp) == geo::Orientation::CLOCKWISE);
	#endif

	Ring pts;
	pts.push_back(pos);
	Vector dirA(owner->getDir());
	dirA *= 10;
	geo::rotate(dirA, sn, cs);
	Ray a(pos, pos+dirA);
	
	Vector dirB(owner->getDir());
	dirB *= 10;
	geo::rotate(dirB, -sn, cs);
	Ray b(pos, pos+dirB);
	
	int i = 0;
	Vector pt;
	for (; ; (i==tmp.size() - 2) ? i=0 : ++i) {
		Segment s(tmp[i], tmp[i+1]);
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
		Segment s(tmp[i], tmp[i+1]);
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


void FlashLight::setEnvironment(Environment* env) {
	this->env = env;
}