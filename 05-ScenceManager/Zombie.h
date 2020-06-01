#ifndef ZOMBIE_H
#define ZOMBIE_H

#define TIME_CREATE_ZOMBIE 1000
#define ZOMBIE_SPEED 0.05f
#define ZOMBIE_FRAMEWIDTH 32
#define ZOMBIE_FRAMEHEIGHT 32

#include "Enemy.h"
class Zombie : public Enemy
{
public:
	Zombie(int nx1 = 1);
	Zombie(float x1, float y1, int nx1);
	~Zombie();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);

	

};

#endif  