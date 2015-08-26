#ifndef VERTEX_H
#define VERTEX_H
#include "../Geometry.h"

struct IObject;

struct Vertex : Vector {

    using Vector::operator*;
	using Vector::operator^;
	
	                        
	using Vector::operator+;
	using Vector::operator-;
	using Vector::operator/;

	using Vector::operator+=;
	using Vector::operator-=;
	using Vector::operator/=;
	using Vector::operator*=;
	
	using Vector::operator==;
	using Vector::operator=;
			
	
	Vertex() {nodeNumber=-1, owner = NULL;}
	Vertex(long long const& x, long long const& y) : Vector(x, y), nodeNumber(-1), owner(NULL) {}
	Vertex(Vertex const& other) : Vector(other), nodeNumber(other.nodeNumber), owner(other.owner) {}
	Vertex(Vector const& other) : Vector(other), nodeNumber(-1), owner(NULL) {}
	
	int nodeNumber;
	IObject* owner;
};

#endif