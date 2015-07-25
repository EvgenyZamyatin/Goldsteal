#include <vector>
#include <hge.h>    
#include "Camera.h"
#include "IBody.h"

const double Camera::SHIFT_MULTIPLY = 5.0;

Camera::Camera(double fieldWidth, double fieldHight, Geo::Vector pos, 
				double cameraWidth, double cameraHight, double screenWidth, double screenHight,
				double radius) : KX(screenWidth/cameraWidth), KY(screenHight/cameraHight), FIELD_WIDTH(fieldWidth), FIELD_HIGHT(fieldHight),
									CAMERA_WIDTH(cameraWidth), CAMERA_HIGHT(cameraHight), SCREEN_WIDTH(screenWidth), SCREEN_HIGHT(screenHight),
									RADIUS(radius) {
	this->body = NULL;
	this->pos = pos;                
}


void Camera::frame(InputData* input, bool freeMode) {
	Geo::Vector mouseDir(input->mX-pos.x, input->mY-pos.y);
	mouseDir *= -1.0;//(RADIUS/mouseDir.len());
	if (input->pShift) {
		if (Geo::greater(mouseDir.len(), RADIUS*SHIFT_MULTIPLY)) 
			mouseDir *= RADIUS*SHIFT_MULTIPLY/mouseDir.len();
	} else {
		if (Geo::greater(mouseDir.len(), RADIUS)) 
			mouseDir *= RADIUS/mouseDir.len();	
	}

	Geo::Vector moveDir(pos+mouseDir);
	move((body->getPosition() - moveDir)/4);
	                
	pos.x=std::max(pos.x, CAMERA_WIDTH/2);
	pos.x=std::min(pos.x, FIELD_WIDTH - CAMERA_WIDTH/2); 

	pos.y=std::max(pos.y, CAMERA_HIGHT/2);
	pos.y=std::min(pos.y, FIELD_HIGHT - CAMERA_HIGHT/2); 
}

void Camera::convertSG(Geo::Vector& v) const {
	v.x = v.x/KX + pos.x-CAMERA_WIDTH/2;
	v.y = v.y/KY + pos.y-CAMERA_HIGHT/2;
}

void Camera::convertGS(Geo::Vector& v) const {
	v.x = (v.x-(pos.x-CAMERA_WIDTH/2))*KX;
	v.y = (v.y-(pos.y-CAMERA_HIGHT/2))*KY;
}

void Camera::convertGS(double &x, double& y) const {
	x = (x-(pos.x-CAMERA_WIDTH/2))*KX;
	y = (y-(pos.y-CAMERA_HIGHT/2))*KY;
}

void Camera::convertSG(double &x, double& y) const {
	x = x/KX + pos.x-CAMERA_WIDTH/2;
	y = y/KY + pos.y-CAMERA_HIGHT/2;
}
 	