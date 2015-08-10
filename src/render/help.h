#include <hge.h>
#include <vector>
#include "../geometry/Geometry.h"
#include "../model/Camera.h"

const int dx[] = {0, 1, 1, 0};
const int dy[] = {0, 0, 1, 1};

inline void fillQuad(hgeQuad& quad, std::vector<hgeVertex> const& v) {
	for (int i = 0; i < 4; ++i)
		quad.v[i]=v[i];
}

inline void fillTriple(hgeTriple& trip, std::vector<hgeVertex> const& v) {
	for (int i = 0; i < 3; ++i)
		trip.v[i]=v[i];
}
