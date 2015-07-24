#ifndef BODYDATA_H
#define BODYDATA_H
#include <vector>
#include <hge.h>
#include <hgeanim.h>

namespace Render {
	struct BodyData {
		std::vector<hgeAnimation> legAnims;
		std::vector<hgeAnimation> bodyAnims;

		int lastLegState;
		int lastBodyState;

		hgeAnimation& getLeg(int state) {
			return legAnims[state];
		}

		hgeAnimation& getBody(int state) {
			return bodyAnims[state];
		}

	};
}
#endif