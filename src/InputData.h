#ifndef INPUTDATA_H
#define INPUTDATA_H
#include <hge.h>
//p-pressed
//d-downed
struct InputData {
	bool pUp;
	bool pDown;
	bool pRight;
	bool pLeft;
	bool pShift;

	/*bool dUp;
	bool dDown;
	bool dRight;
	bool dLeft;*/

	float mX, mY;

	/*bool pLMouse;
	bool pRMouse;

	bool dLMouse;
	bool dRMouse;*/

	void update(HGE* hge);
	void flush();
};
#endif