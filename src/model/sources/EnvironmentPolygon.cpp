#include "../Vertex.h"
#include "../Environment.h"
#include "../../Geometry.h"
#include <clipper.hpp>

geo::Ring<Vector> Environment::inflate(geo::Ring<Vector> const& ring, int inflateRadius) {
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
	return ans;
}

geo::Polygon<geo::Ring<Vector>> Environment::subtract(geo::Polygon<geo::Ring<Vector>> const& poly, geo::Ring<Vector> const& ring) {
	
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