#ifndef Candle_H
#define Candle_H


#include "GameObject.h"

#define CANDLE_FRAMEWIDTH 16
#define CANDLE_FRAMEHEIGHT 32

class Candle : public CGameObject
{
public:

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Render();
};


#endif