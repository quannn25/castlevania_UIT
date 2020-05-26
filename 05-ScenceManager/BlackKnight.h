#ifndef BLACKKNIGHT_H
#define BLACKKNIGHT_H


#define BLACKKNIGHT_SPEED 0.05f
#define BLACKKNIGHT_FRAMEWIDTH 32
#define BLACKKNIGHT_FRAMEHEIGHT 64

#include "Enemy.h"
class BlackKnight : public Enemy
{
public:
	BlackKnight(int nx1 = 1);
	~BlackKnight();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
};


#endif 