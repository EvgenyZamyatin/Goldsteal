#include <vector>
#include <hge.h>    
#include "../Camera.h"
#include "../IBody.h"

const int Camera::SHIFT_MULTIPLY = 10;

Camera::Camera(int fieldWidth, int fieldHight, Vector pos, 
				int cameraWidth, int cameraHight, int screenWidth, int screenHight,
				int radius) : KX(screenWidth*1.f/cameraWidth), KY(screenHight*1.f/cameraHight), FIELD_WIDTH(fieldWidth), FIELD_HIGHT(fieldHight),
									CAMERA_WIDTH(cameraWidth), CAMERA_HIGHT(cameraHight), SCREEN_WIDTH(screenWidth), SCREEN_HIGHT(screenHight),
									RADIUS(radius), RADIUS2(radius*radius) {
	this->body = NULL;
	this->pos = pos;                
}


void Camera::frame(InputData* input, bool freeMode) {
	
	Vector mouseDir(input->mX-pos.x, input->mY-pos.y);
	mouseDir *= -1;//(RADIUS/mouseDir.len());
	if (input->pShift) {
		if (geo::distance2(mouseDir) > RADIUS2*SHIFT_MULTIPLY) 
			mouseDir *= RADIUS2*SHIFT_MULTIPLY*1.0/geo::distance2(mouseDir);
	} else {
		if (geo::distance2(mouseDir) > RADIUS2) 
			mouseDir *= sqrt(RADIUS2*1.0/geo::distance2(mouseDir));	
	}

	Vector moveDir(pos+mouseDir);
	move((body->getPosition() - moveDir)/4);
}

void Camera::convertGS(float &x, float& y) const {
	x = (x-(pos.x-CAMERA_WIDTH/2))*KX;
	y = (y-(pos.y-CAMERA_HIGHT/2))*KY;
}

void Camera::convertSG(float &x, float& y) const {
	x = x/KX + pos.x-CAMERA_WIDTH/2;
	y = y/KY + pos.y-CAMERA_HIGHT/2;
}
 	