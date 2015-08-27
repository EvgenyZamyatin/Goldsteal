#include "Vertex.h"
#include "../Geometry.h"
#include <clipper.hpp>



/*int main() {
	
	int inflateRadius = 5;
	Ring ring({{0, 0}, {1, 0}, {1, 1}, {0, 1}});
	geo::correct(ring);

	ClipperLib::Path subj;
  	ClipperLib::Paths solution;
  	for (Vector const& v : ring)
  		subj << ClipperLib::IntPoint((int)v.x, (int)v.y);
    ClipperLib::ClipperOffset co;
	co.AddPath(subj, ClipperLib::jtMiter, ClipperLib::etClosedPolygon);
	co.Execute(solution, inflateRadius);
	#ifdef DEBUG
		assert(solution.size() == 1);
	#endif
	Ring ans;
	for (ClipperLib::IntPoint const& v : solution[0])
		ans.push_back(Vector(v.X, v.Y));
	geo::correct(ans);
	std::cerr << ans;
	return 0;
}*/
geo::Polygon<geo::Ring<Vector>> subtract(geo::Polygon<geo::Ring<Vector>> const& poly, geo::Ring<Vector> const& ring) {
	
	ClipperLib::Path subj;
  	ClipperLib::Paths solution;
  	
  	ClipperLib::Clipper c;
	
  	for (Vector const& v : poly.ering)
  		subj.push_back(ClipperLib::IntPoint((int)v.x, (int)v.y));
  	c.AddPath(subj, ClipperLib::ptSubject, true);

  	for (Ring const& ring : poly.irings) {
		subj.clear();
		for (Vector const& v : ring)
  			subj.push_back(ClipperLib::IntPoint((int)v.x, (int)v.y));
  	   	std::reverse(subj.begin(), subj.end());
  		c.AddPath(subj, ClipperLib::ptSubject, true);
  	}
	

	subj.clear();
  	for (Vector const& v : ring)
  		subj.push_back(ClipperLib::IntPoint((int)v.x, (int)v.y));
  	c.AddPath(subj, ClipperLib::ptClip, true);

    c.Execute(ClipperLib::ctDifference, solution);
    geo::Polygon<geo::Ring<Vector>> ans;
    for (ClipperLib::IntPoint const& pt : solution[0]) {
    	ans.ering.push_back({pt.X, pt.Y});
    }
    for (int i = 1; i < solution.size(); ++i) {
    	ClipperLib::Path const& path = solution[i];
    	geo::Ring<Vector> ring;
    	for (ClipperLib::IntPoint const& pt : path)
    		ring.push_back({pt.X, pt.Y});
    	ans.irings.push_back(ring);
    }
    geo::correct(ans);
	return ans;

}

void print(Poly const& ans) {
	std::cerr << ans.ering << "\n";
	for (Ring const& ring : ans.irings)
		std::cerr << ring << "\n";	                               
}

int main() {
	Poly poly({{0,0}, {10, 0}, {10, 10}, {0, 10}});
	geo::correct(poly);
	Ring ring({{0,0}, {0,2}, {2,2}, {2,0}});
	geo::correct(ring);
	Poly ans = subtract(poly, ring);
	ans = subtract(ans, ring);
	ans = subtract(ans, {{1, 1}, {3, 1}, {3, 3}, {1, 3}, {1,1}});

	print(ans);
	return 0;

}