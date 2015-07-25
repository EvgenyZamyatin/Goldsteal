#include "InputData.h"
#include "cstdlib"

void InputData::flush() {
	pUp = 0;
	pDown = 0;
	pRight = 0;
	pLeft = 0;

	pShift = 0;
}

void InputData::update(HGE* hge) {               
	pUp = hge->Input_GetKeyState(HGEK_UP);
	pDown = hge->Input_GetKeyState(HGEK_DOWN);
	pRight = hge->Input_GetKeyState(HGEK_RIGHT);
	pLeft = hge->Input_GetKeyState(HGEK_LEFT);

	pShift = hge->Input_GetKeyState(HGEK_SHIFT);

	hge->Input_GetMousePos(&mX, &mY);
}