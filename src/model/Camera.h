#ifndef CAMERA_H
#define CAMERA_H

#include <hge.h>
#include "../geometry/Geometry.h"
#include "../InputData.h"

struct IBody;
struct Camera {
	
	static const double SHIFT_MULTIPLY;
	
	const double KX;
	const double KY;
	const double RADIUS;
	const double CAMERA_WIDTH;
	const double CAMERA_HIGHT;
	const double SCREEN_WIDTH;
	const double SCREEN_HIGHT;
	const double FIELD_WIDTH;
	const double FIELD_HIGHT;


	Camera(double fieldWidth, double fieldHight, Geo::Vector pos, 
			double cameraWidth, double cameraHight, double screenWidth, double screenHight,
			double radius);

	void frame(InputData* input, bool freeMode);
	
	Geo::Vector getPos() const {return pos;}
	
	void bind(IBody* body) {this->body=body;}
	void move(Geo::Vector const& v) {pos += v;}

	void convertSG(Geo::Vector& v) const;
	void convertGS(Geo::Vector& v) const;

	void convertSG(double& x, double& y) const;
	void convertGS(double& x, double& y) const;


private:
	IBody* body=NULL;
	Geo::Vector pos;
};
#endif