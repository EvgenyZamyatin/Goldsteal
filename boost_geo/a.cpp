#include "Geometry.h"
#include <cassert>
#include <iostream>

int main() {   
    Polygon a, b, c;
    bg::read_wkt("POLYGON((1 2, 2 3, 3 3, 4 2, 3 1, 2 1))", a);
    bg::read_wkt("POLYGON((2 2, 1 3, 1 4, 4 4, 4 3, 3 2))", b);
    bg::read_wkt("POLYGON((0 0, 0 10, 10 10, 10 0), (1 1, 1 2, 2 2, 2 1))", c);
    bg::correct(a);
    bg::correct(b);
    std::vector<Polygon> points;
    bg::union_(a, b, points);
    for (auto i : points) {
        std::cout << bg::dsv(i) << "\n";
    }
    for (auto i = boost::begin(bg::interior_rings(c)); i != boost::end(bg::interior_rings(c)); i++)
       std::cout << bg::dsv(*i) << "\n"; 
    return 0;
}
