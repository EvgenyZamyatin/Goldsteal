#include <vector>
#include <hge.h>    
#include "Camera.h"
#include "IBody.h"

Camera::Camera(double fieldWidth, double fieldHight, Geo::Vector pos, 
				double cameraWidth, double cameraHight, double screenWidth, double screenHight,
				double radius) {
	this->fieldWidth = fieldWidth;
	this->fieldHight = fieldHight;
	body = NULL;
	this->pos = pos;
	this->cameraWidth = cameraWidth;
	this->cameraHight = cameraHight;
	this->body=body;                
	this->screenWidth = screenWidth;
	this->screenHight = screenHight;
	this->radius = radius;
}

void Camera::frame(Geo::Vector const& mouse, bool freeMode) {
	Geo::Vector tmp;
	if (freeMode) {
		tmp = (mouse-Geo::Vector(screenWidth/2, screenHight/2));
	} else {
		tmp = (mouse - body->getPosition());
	}
	
	if (Geo::greater(tmp.len(), radius)) 
		move(tmp/radius);                
	else return;

	pos.x=std::max(pos.x, cameraWidth/2);
	pos.x=std::min(pos.x, fieldWidth - cameraWidth/2); 

	pos.y=std::max(pos.y, cameraHight/2);
	pos.y=std::min(pos.y, fieldHight - cameraHight/2); 

}    	