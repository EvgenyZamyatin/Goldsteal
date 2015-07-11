#include "Geometry.h"
#include "Boosting.h"
#include <cassert>
#include <algorithm>

using namespace Geo;
void orientation_test() {
    assert(orientation(Vector(1, 6), Vector(3, 1), Vector(6, 6)) == LEFT);
    assert(orientation(Vector(6, 6), Vector(3, 1), Vector(1, 6)) == RIGHT);
    assert(orientation(Vector(4, 1), Vector(5, 3), Vector(6, 5)) == COLLINEAR);
}

void collinear_test() {

}

void onLine_test() {
    Line l(Vector(7,5), Vector(1,2));
    Vector v1(6, 3);
    Vector v2(5, 1);
    assert(onLine(v1, l));
    assert(onLine(v2, l));

}

void intersect_test() {
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
        return false;
    };

    Polygon poly(std::vector<Vector>({Vector(1,1),Vector(2,1), Vector(2,2),Vector(1,2)}));
    Vector o(3,3);
    Polygon myPoly = visibilityPolygon(o, std::vector<Polygon>({poly}), 5, 5);
    Polygon truePoly = Polygon(std::vector<Vector>({Vector(1.5,0), Vector(5,0), Vector(5, 5), Vector(0, 5), Vector(0, 1.5), Vector(1, 2), Vector(2,2), Vector(2, 1)}));
    assert(check(myPoly, truePoly));
    std::vector<Polygon> polys(std::vector<Polygon>({
            Polygon(std::vector<Vector>({Vector(1,2), Vector(1,4), Vector(2,4), Vector(2,2)})),
            Polygon(std::vector<Vector>({Vector(3,5), Vector(3,6), Vector(4,6), Vector(4,5)})),
            Polygon(std::vector<Vector>({Vector(4,1), Vector(4,3), Vector(5,3), Vector(5,1)})),
            Polygon(std::vector<Vector>({Vector(5,6), Vector(5,7), Vector(6,7), Vector(6,6)})),
            Polygon(std::vector<Vector>({Vector(4,8), Vector(4,9), Vector(8,9), Vector(8,8)})),
            Polygon(std::vector<Vector>({Vector(8,2), Vector(9,2), Vector(9,7), Vector(8,7)}))
            }));
    Vector o1(5,5);
    myPoly = visibilityPolygon(o1, polys, 10, 10);
    truePoly = Polygon(std::vector<Vector>({
            Vector(0,0),
            Vector(2.5,0),
            Vector(4,3),
            Vector(5,3),
            Vector(5,0),
            Vector(10,0),
            Vector(8,2),
            Vector(8,7),
            Vector(10,7+4.0/3),
            Vector(10,10),
            Vector(6,6),
            Vector(5,6),
            Vector(5,8),
            Vector(4,8),
            Vector(3+1.0/3,10),
            Vector(0,10),
            Vector(4,6),
            Vector(4,5),
            Vector(0,5),
            Vector(0,3.75),
            Vector(1,4),
            Vector(2,4),
            Vector(2,2)
        }));
    assert(check(myPoly, truePoly));
}

int main() {
    orientation_test();
    collinear_test();
    onLine_test();
    intersect_test();
    visibilityPolygon_test();
    
    std::cout << "All tests had been passed!" << "\n";
    return 0;
}
