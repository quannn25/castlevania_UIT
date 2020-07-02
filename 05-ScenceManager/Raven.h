#ifndef RAVEN_H
#define RAVEN_H


#define RAVEN_SPEED_X 0.15f
#define RAVEN_SPEED_Y 0.15f
#define RAVEN_GRAVITY 0.0015f
#define RAVEN_FRAMEWIDTH 32
#define RAVEN_FRAMEHEIGHT 32

#define RAVEN_ACTIVEZONE 190
#define TIME_READY 550
#define RAVEN_ATTACKZONE 100
#define RANDX_READY 80
#define RANDY_READY 130


#define RAVEN_STATE_WAIT 0
#define RAVEN_STATE_ATTACK 1
#define RAVEN_STATE_FIND_TARGET 2
#define RAVEN_STATE_READY 3
#define RAVEN_STATE_GO_TARGET 4
#define RAVEN_STATE_GO_RANDOM 5
#define RAVEN_STATE_FIND_TARGET_AGAIN 6

#include "Enemy.h"
#include "Simon.h"
class Raven : public Enemy
{
	bool isActive;
	DWORD timeReady;
	float randX;
	float randY;
	int ny;
public:
	Raven(float x1, float y1, int nx1);
	~Raven();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, Simon * simon, vector<LPGAMEOBJECT> *coObjects = NULL);
	void SetState(int state);
	void Render();

};

#endif 