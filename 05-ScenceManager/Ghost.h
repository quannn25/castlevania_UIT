#ifndef GHOST_H
#define GHOST_H


#define GHOST_SPEED 0.05f
#define GHOST_FRAMEWIDTH 32
#define GHOST_FRAMEHEIGHT 32

#include "Enemy.h"
class Ghost : public Enemy
{
public:
	Ghost(int nx1 = 1);
	~Ghost();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);

};

#endif  