#ifndef BODYDATA_H
#define BODYDATA_H
#include <vector>
#include <hge.h>
#include <hgeanim.h>

namespace Render {
	struct BodyData {
		std::vector<hgeAnimation> anims;
		int lastState;
		int width;
		int hight;
		hgeAnimation& get(int state) {
			return anims[state];
		}
	};
}
#endif