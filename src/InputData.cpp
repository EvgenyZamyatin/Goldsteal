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
	float a,b;
	hge->Input_GetMousePos(&a, &b);
	cam->convertSG(a, b);
	mX=(int)a, mY=(int)b;
}
