#include <vector>
#include <hge.h>    
#include "Camera.h"
#include "IBody.h"

const int Camera::SHIFT_MULTIPLY = 10;

Camera::Camera(int fieldWidth, int fieldHight, Geo::Vector pos, 
				int cameraWidth, int cameraHight, int screenWidth, int screenHight,
				int radius) : KX(screenWidth*1.f/cameraWidth), KY(screenHight*1.f/cameraHight), FIELD_WIDTH(fieldWidth), FIELD_HIGHT(fieldHight),
									CAMERA_WIDTH(cameraWidth), CAMERA_HIGHT(cameraHight), SCREEN_WIDTH(screenWidth), SCREEN_HIGHT(screenHight),
									RADIUS(radius), RADIUS2(radius*radius) {
	this->body = NULL;
	this->pos = pos;                
}


void Camera::frame(InputData* input, bool freeMode) {
	Geo::Vector mouseDir(input->mX-pos.x, input->mY-pos.y);
	mouseDir *= -1;//(RADIUS/mouseDir.len());
	if (input->pShift) {
		if (mouseDir.len2() > RADIUS2*SHIFT_MULTIPLY) 
			mouseDir *= RADIUS2*SHIFT_MULTIPLY*1.0/mouseDir.len2();
	} else {
		if (mouseDir.len2() > RADIUS2) 
			mouseDir *= sqrt(RADIUS2*1.0/mouseDir.len2());	
	}

	Geo::Vector moveDir(pos+mouseDir);
	move((body->getPosition() - moveDir)/4);
	
	pos.x=std::max(pos.x, CAMERA_WIDTH/2);
	pos.x=std::min(pos.x, FIELD_WIDTH - CAMERA_WIDTH/2); 

	pos.y=std::max(pos.y, CAMERA_HIGHT/2);
	pos.y=std::min(pos.y, FIELD_HIGHT - CAMERA_HIGHT/2); 
}

void Camera::convertGS(float &x, float& y) const {
	x = (x-(pos.x-CAMERA_WIDTH/2))*KX;
	y = (y-(pos.y-CAMERA_HIGHT/2))*KY;
}

void Camera::convertSG(float &x, float& y) const {
	x = x/KX + pos.x-CAMERA_WIDTH/2;
	y = y/KY + pos.y-CAMERA_HIGHT/2;
}
 	