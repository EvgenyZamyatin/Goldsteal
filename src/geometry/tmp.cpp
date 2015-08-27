#include "newGeometry.h"


int main() {
	geo::Polygon<geo::Ring<geo::Vector<int>>> poly(
		{{0,0}, {10, 0}, {10,10}, {0, 10}},
		{
			{{1,1}, {1,2}, {2,2}, {2,1}}
	   	});
	geo::correct(poly);
	geo::Ring<geo::Vector<int>> out;
	geo::visibilityPolygon({2,2}, poly, out);
	std::cerr << out << "\n";
	return 0;
}