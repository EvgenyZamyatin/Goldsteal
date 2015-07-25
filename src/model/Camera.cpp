#include <vector>
#include <hge.h>    
#include "Camera.h"
#include "IBody.h"

const double Camera::SHIFT_MULTIPLY = 5.0;

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


void Camera::frame(InputData* input, bool freeMode) {
	Geo::Vector mouseDir(input->mX-screenWidth/2, input->mY-screenHight/2);
	mouseDir *= -1.0;//(radius/mouseDir.len());
	if (input->pShift) {
		if (Geo::greater(mouseDir.len(), radius*SHIFT_MULTIPLY)) 
			mouseDir *= radius*SHIFT_MULTIPLY/mouseDir.len();
	} else {
		if (Geo::greater(mouseDir.len(), radius)) 
			mouseDir *= radius/mouseDir.len();	
	}

	Geo::Vector moveDir(pos+mouseDir);
	move((body->getPosition() - moveDir)/4);
	                
	pos.x=std::max(pos.x, cameraWidth/2);
	pos.x=std::min(pos.x, fieldWidth - cameraWidth/2); 

	pos.y=std::max(pos.y, cameraHight/2);
	pos.y=std::min(pos.y, fieldHight - cameraHight/2); 

}

/*
Geo::Vector tmp;
	if (input->pShift || freeMode) {
		tmp = (Geo::Vector(input->mX, input->mY)-Geo::Vector(screenWidth/2, screenHight/2));
	} else {
		tmp = (body->getPosition() - pos);
	}
	if (Geo::greater(tmp.len(), radius)) 
		move(tmp/radius*3);                
	else return;
	
	pos.x=std::max(pos.x, cameraWidth/2);
	pos.x=std::min(pos.x, fieldWidth - cameraWidth/2); 

	pos.y=std::max(pos.y, cameraHight/2);
	pos.y=std::min(pos.y, fieldHight - cameraHight/2); 

*/ 	