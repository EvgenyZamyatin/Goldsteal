#include "InputData.h"
#include "cstdlib"
#include "model/Camera.h"
void InputData::flush() {
	pUp = 0;
	pDown = 0;
	pRight = 0;
	pLeft = 0;

	pShift = 0;
}

void InputData::update(HGE* hge, Camera const* cam) {               
	pUp = hge->Input_GetKeyState(HGEK_W);
	pDown = hge->Input_GetKeyState(HGEK_S);
	pRight = hge->Input_GetKeyState(HGEK_D);
	pLeft = hge->Input_GetKeyState(HGEK_A);

	pShift = hge->Input_GetKeyState(HGEK_SHIFT);
	hge->Input_GetMousePos(&mX, &mY);
	double a=mX,b=mY;
	cam->convertSG(a, b);
	mX=a, mY=b;
	//mX = mX/cam->KX + cam->getPos().x;
	//mY = mY/cam->KY + cam->getPos().y;
}
