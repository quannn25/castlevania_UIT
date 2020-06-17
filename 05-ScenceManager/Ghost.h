#ifndef GHOST_H
#define GHOST_H


#define GHOST_SPEED_X 0.05f
#define GHOST_SPEED_Y 0.05f
#define GHOST_FRAMEWIDTH 32
#define GHOST_FRAMEHEIGHT 32

#include "Enemy.h"
#include "Simon.h"
class Ghost : public Enemy
{
	bool isAuto;
public:
	Ghost(int nx1 = 1);
	Ghost(float x1, float y1, int nx1);
	~Ghost();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, Simon * simon, vector<LPGAMEOBJECT> *coObjects = NULL);

};

#endif  