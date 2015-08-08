#include "Geometry.h"
#include <cassert>
#include <algorithm>

using namespace Geo;

void orientation_test() {
	assert(orientation(Vector(9,6), Vector(2000, 3000), Vector(0,0)) != COLLINEAR);
    assert(orientation(Vector(1, 6), Vector(3, 1), Vector(6, 6)) == LEFT);
    assert(orientation(Vector(6, 6), Vector(3, 1), Vector(1, 6)) == RIGHT);
    assert(orientation(Vector(4, 1), Vector(5, 3), Vector(6, 5)) == COLLINEAR);
}

void collinear_test() {

}

/*void intersect_test() {
    //Line-Line
    {
        Line a(Vector(3, 1), Vector(1, 3));
        Line b(Vector(8, 4), Vector(2,-1));
        Vector pt;
        assert(intersect(a, b, pt));
        assert(pt == Vector(32.0/7, 3.0*(32.0/7)-8));
    }
    //Line-Seg
    {
        Line l(Vector(7,5), Vector(1,2));
        Segment s(Vector(5, 1), Vector(6, 3));
        Vector pt;
        assert(intersect(l, s, pt, true));
        l = Line(Vector(62,5350.12), Vector(2355,5350.1)-Vector(62,5350.12));
        s = Segment(Vector(2227,5350.1), Vector(2227, 5222.1));        
        assert(!intersect(l, s, pt, true));

    }
    //Seg-Seg
    {

    }
    //Line-Polygon
    {
        Vector pts[] = {
            Vector (2, 1),
            Vector (3, 2),
            Vector (5, 1),
            Vector (6, 3),
            Vector (4, 4),
            Vector (3, 5),
            Vector (1, 5),
            Vector (1, 3)
        };
        Polygon poly(std::vector<Vector>(pts, pts+8));

        auto check = [&poly](Line l, bool ct, std::vector<Vector> ans) {
            std::vector<Vector> res;
            intersect(l, poly, res, ct);
            std::sort(res.begin(), res.end());
            std::sort(ans.begin(), ans.end());
            return res == ans;
        };
        Line l(Vector(3, 0), Vector(0, 1));
        assert(check(l, true, std::vector<Vector>({Vector(3, 2), Vector(3, 5)})));
        assert(check(l, false, std::vector<Vector>({Vector(3, 2), Vector(3, 5)})));

        l = Line(Vector(7, 5), Vector(1, 2));
        assert(check(l, true, std::vector<Vector>({Vector(5, 1), Vector(6, 3)})));
        assert(check(l, false, std::vector<Vector>()));

        l = Line(Vector(7,0),Vector(-1,3));
        assert(check(l, true, std::vector<Vector>({Vector(6, 3)})));
        assert(check(l, false, std::vector<Vector>()));

        l = Line(Vector(0, 1.5), Vector(1, 0));
        assert(check(l, true, std::vector<Vector>({Vector(1.75, 1.5), Vector(2.5, 1.5), Vector(4, 1.5), Vector(5.25, 1.5)})));
        assert(check(l, false, std::vector<Vector>({Vector(1.75, 1.5), Vector(2.5, 1.5), Vector(4, 1.5), Vector(5.25, 1.5)})));

        l = Line(Vector(0, 1), Vector(1, 0));
        assert(check(l, true, std::vector<Vector>({Vector(2, 1), Vector(5, 1)})));
        assert(check(l, false, std::vector<Vector>()));
    }
}
*/

void write(Polygon p, std::string file) {
	freopen(file.c_str(), "w", stdout);
	printf("%s\n", p.to_string().c_str());
}

void visibilityPolygon_test() {
    auto check = [](Polygon a, Polygon b) {
        if (a.size() != b.size())
            return false;

        for (int i = 0; i < a.points.size(); ++i) {
            bool ok = true;
            for (int j = 0; j < b.points.size(); ++j) {
                ok &= a[(i+j)%a.size()] == b[j];
            }
            if (ok)
                return true;
        }
        std::reverse(a.points.begin(), a.points.end());
        for (int i = 0; i < a.points.size(); ++i) {
            bool ok = true;
            for (int j = 0; j < b.points.size(); ++j) {
                ok &= a[(i+j)%a.size()] == b[j];
            }
            if (ok)
                return true;
        }        
        return false;
    };
    Polygon myPoly;
    Polygon truePoly;
    std::vector<Polygon> polys;
    Vector o;
    int testNumber = 0;
    
    Polygon poly(std::vector<Vector>({Vector(1,1),Vector(2,1), Vector(2,2),Vector(1,2)}));
    o = Vector(3,3);
    myPoly = visibilityPolygon(o, std::vector<Polygon>({poly}), 5, 5);
    write(myPoly, std::string("test") + std::to_string(testNumber++) + ".ans");

    polys = (std::vector<Polygon>({
            	Polygon(std::vector<Vector>({Vector(269,257), Vector(269,385), Vector(397,385), Vector(397,257)}))
            }));
    o = Vector(267,370);
    myPoly = visibilityPolygon(o, polys, 640, 640);
    write(myPoly, std::string("test") + std::to_string(testNumber++) + ".ans");
    
    polys = (std::vector<Polygon>({
            Polygon(std::vector<Vector>({Vector(1,2), Vector(1,4), Vector(2,4), Vector(2,2)})),
            Polygon(std::vector<Vector>({Vector(3,5), Vector(3,6), Vector(4,6), Vector(4,5)})),
            Polygon(std::vector<Vector>({Vector(4,1), Vector(4,3), Vector(5,3), Vector(5,1)})),
            Polygon(std::vector<Vector>({Vector(5,6), Vector(5,7), Vector(6,7), Vector(6,6)})),
            Polygon(std::vector<Vector>({Vector(4,8), Vector(4,9), Vector(8,9), Vector(8,8)})),
            Polygon(std::vector<Vector>({Vector(8,2), Vector(9,2), Vector(9,7), Vector(8,7)}))
            }));
    o = Vector(5,5);
    myPoly = visibilityPolygon(o, polys, 10, 10);
    write(myPoly, std::string("test") + std::to_string(testNumber++) + ".ans");

    polys = (std::vector<Polygon>({
			Polygon(std::vector<Vector>({Vector(7,5), Vector(5,8), Vector(9,6)})),			
			Polygon(std::vector<Vector>({Vector(6,1), Vector(7,3), Vector(8,4), Vector(10,3)}))
		}));
	o = Vector(2,3);
	myPoly = visibilityPolygon(o, polys, 15, 10);
	write(myPoly, std::string("test") + std::to_string(testNumber++) + ".ans");
    
    polys = (std::vector<Polygon>({
			Polygon(std::vector<Vector>({Vector(7,5)*1000, Vector(5,8)*1000, Vector(9,6)*1000})),			
			Polygon(std::vector<Vector>({Vector(6,1)*1000, Vector(7,3)*1000, Vector(8,4)*1000, Vector(10,3)*1000}))
		}));
	o = Vector(2,3); o*=1000;
	myPoly = visibilityPolygon(o, polys, 15*1000, 10*1000);
	write(myPoly, std::string("test") + std::to_string(testNumber++) + ".ans");

	polys = (std::vector<Polygon>({
			Polygon(std::vector<Vector>({Vector(591, 171), Vector(591,329), Vector(607,329), Vector(607,171)}))
		}));
	o = Vector(528, 532);
	myPoly = visibilityPolygon(o, polys, 640, 640);
	write(myPoly, std::string("test") + std::to_string(testNumber++) + ".ans");
    
}

int main() {
    Line l({2008, 2303}, {0, 2304});
    Segment s({0, 2304},{1883, 2304});
    Vector pt;
    std::cerr << intersect(l, s, pt) << "\n";
    std::cerr << pt << "\n";
    return 0;
	orientation_test();
    collinear_test();
    visibilityPolygon_test();
    /*Polygon a(std::vector<Vector>({{0,0}, {0,2}, {2,2}, {2,0}}));
    Polygon b(std::vector<Vector>({{2,0}, {2,2}, {4,2}, {4,0}}));
	std::cerr << distance(a, b) << "\n";
	a = Polygon(std::vector<Vector>({{0,0}, {0,2}, {2,2}, {2,0}}));
    b = Polygon(std::vector<Vector>({{12,0}, {12,2}, {14,2}, {14,0}}));
	std::cerr << distance(a, b) << "\n";
	
	a = Polygon(std::vector<Vector>({{0,0}, {0,10}, {10,10}, {10,0}}));
    b = Polygon(std::vector<Vector>({{3,0}, {3,6}, {6,6}, {6,3}}));
	std::cerr << distance(a, b) << "\n";
	Polygon a(std::vector<Vector>({{0,0}, {0,2}, {2,2}, {2,0}});
	*/
	//Vector o(405.000000 , 305.000000);
	//std::vector<Polygon> polys({ Polygon({Vector(428.000000 , 246.000000), Vector(471.301270 , 271.000000), Vector(494.301270 , 231.162827), Vector(451.000000 , 206.162827)})});
	//visibilityPolygon(o, polys, 640, 512);
	//(640.000000 , 184.489487) (471.301270 , 271.000000) (0.000000 , 0.000000) (0.000000 , 512.000000) (640.000000 , 512.000000)
	
	std::cout << "All tests had been passed!" << "\n";
    return 0;
}
