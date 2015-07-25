#ifndef CAMERA_H
#define CAMERA_H

#include <hge.h>
#include "../geometry/Geometry.h"
#include "../InputData.h"

struct IBody;
struct Camera {
	
	static const double SHIFT_MULTIPLY;

	Camera(double fieldWidth, double fieldHight, Geo::Vector pos, 
			double cameraWidth, double cameraHight, double screenWidth, double screenHight,
			double radius);

	void frame(InputData* input, bool freeMode);
	
	Geo::Vector getPos() const {return pos;}
	
	double cWidth() const {return cameraWidth;}
	double cHight() const {return cameraHight;}

	double sWidth() const {return screenWidth;}
	double sHight() const {return screenHight;}

	void bind(IBody* body) {this->body=body;}
	void move(Geo::Vector const& v) {pos += v;}

private:
	IBody* body=NULL;
	Geo::Vector pos;
	
	double radius;

	double cameraWidth;
	double cameraHight;
	double screenWidth;
	double screenHight;
	double fieldWidth;
	double fieldHight;
};
#endif