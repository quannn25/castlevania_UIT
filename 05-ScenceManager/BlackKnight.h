#ifndef BLACKKNIGHT_H
#define BLACKKNIGHT_H

#include "Enemy.h"
#include "Simon.h"

#define BLACKKNIGHT_SPEED 0.05f
#define BLACKKNIGHT_FRAMEWIDTH 32
#define BLACKKNIGHT_FRAMEHEIGHT 64

class BlackKnight : public Enemy
{
protected:
	float left_boundary;
	float right_boundary;

	float delta; // đoạn đường đã đi được từ lần cuối đổi hướng
	float walkDelta; // đoạn đường phải đi để đươc đổi hướng (nx)
public:
	BlackKnight(float left_boundary, float right_boundary, int nx1 = 1);
	~BlackKnight();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, Simon * simon, vector<LPGAMEOBJECT> *coObjects = NULL);
};


#endif 