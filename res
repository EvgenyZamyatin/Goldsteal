src/geometry/Geometry.cpp: In function 'Geo::Polygon Geo::visibilityPolygonFast(Geo::Vector, std::vector<Geo::Polygon>)':
src/geometry/Geometry.cpp:441:23: error: 'veritces' was not declared in this scope
    Segment const& v = veritces[i];
                       ^
src/geometry/Geometry.cpp:442:18: error: expected initializer before '&' token
    Segment conset& s;
                  ^
src/geometry/Geometry.cpp:458:11: error: 'candidates' was not declared in this scope
           candidates.push_back(nearest); 
           ^
src/geometry/Geometry.cpp:461:11: error: 'candidates' was not declared in this scope
           candidates.push_back(v.a); 
           ^
make.EXE: *** [Geometry.o] Ошибка 1
