#include "Geometry.h"
#include <bits/stdc++.h>
#include <vector>
using namespace std;
using namespace Geo;

int main() {
    
    /*Vector pts[] = {
        Vector (2, 1),
        Vector (3, 2),
        Vector (5, 1),
        Vector (6, 3),
        Vector (4, 4),
        Vector (3, 5),
        Vector (1, 5),
        Vector (1, 3)
    };*/

    Vector pts[] = {
        Vector(1, 1),
        Vector(2, 1),
        Vector(2, 2),
        Vector(1, 2)
    };

    std::vector<Vector> ps;
    ps.insert(ps.begin(), pts, pts+4);
    Polygon p(ps);
    vector<Polygon> polygons;
    polygons.push_back(p);
    for (Vector i : visibilityPolygon(Vector(3, 3), polygons, 5, 5).points)
        std::cerr << i << "\n";

    /*
    Line l(Vector(0, 1), Vector(1, 0));
    ps.clear();
    bool i = intersect (l, p, ps, true);
    for (auto i : ps) {
        std::cout << i.x << " " << i.y << "\n";
    }
    */


    return 0;
}
