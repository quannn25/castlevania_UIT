#ifndef ZOMBIE_H
#define ZOMBIE_H

#define TIME_CREATE_ZOMBIE 1000
#define ZOMBIE_SPEED_X 0.05f
#define ZOMBIE_FRAMEWIDTH 31
#define ZOMBIE_FRAMEHEIGHT 64
#define ZOMBIE_GRAVITY 0.005f

#include "Enemy.h"
#include "Brick.h"
class Zombie : public Enemy
{
public:
	Zombie(int nx1 = 1);
	Zombie(float x1, float y1, int nx1);
	~Zombie();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	

};

#endif  