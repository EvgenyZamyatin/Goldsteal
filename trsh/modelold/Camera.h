#ifndef CAMERA_H
#define CAMERA_H

#include <hge.h>
#include "../Geometry.h"
#include "../InputData.h"

struct IBody;
struct Camera {
	
	static const int SHIFT_MULTIPLY;
	
	const float KX;
	const float KY;
	const int RADIUS;
	const int RADIUS2;
	const int CAMERA_WIDTH;
	const int CAMERA_HIGHT;
	const int SCREEN_WIDTH;
	const int SCREEN_HIGHT;
	const int FIELD_WIDTH;
	const int FIELD_HIGHT;


	Camera(int fieldWidth, int fieldHight, Vector pos, 
			int cameraWidth, int cameraHight, int screenWidth, int screenHight,
			int radius);

	void frame(InputData* input, bool freeMode);
	
	Vector getPos() const {return pos;}
	
	void bind(IBody* body) {this->body=body;}
	void move(Vector const& v) {pos += v;}

	void convertSG(float& x, float& y) const;
	void convertGS(float& x, float& y) const;

private:
	IBody* body=NULL;
	Vector pos;
};
#endif