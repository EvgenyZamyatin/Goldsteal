#include "newGeometry.h"



int main() {
	geo::Polygon<geo::Ring<geo::Vector<int>>> poly(
		{{0,0}, {10, 0}, {10,10}, {0, 10}},
		{
			{{1,1}, {1,2}, {2,2}, {2,1}}
	   	});
	geo::correct(poly);
	std::vector<geo::Vector<int>> out;
	geo::visibileVertices({2,1}, {2,2}, {1,2}, poly, out);
	for (geo::Vector<int>& v : out)
		std::cerr << v << "\n";
	return 0;
}