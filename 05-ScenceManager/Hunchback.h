#ifndef HUNCHBACK_H
#define HUNCHBACK_H


#define HUNCHBACK_SPEED_X 0.15f
#define HUNCHBACK_SPEED_Y 0.5f
#define HUNCHBACK_GRAVITY 0.0015f
#define HUNCHBACK_FRAMEWIDTH 32
#define HUNCHBACK_FRAMEHEIGHT 32

#define HUNCHBACK_ACTIVEZONE 190
#define TIME_READY 550
#define HUNCHBACK_ATTACKZONE 100

#define HUNCHBACK_STATE_WAIT 0
#define HUNCHBACK_STATE_ATTACK 1
#define HUNCHBACK_STATE_JUMP 2
#define HUNCHBACK_STATE_READY 3

#include "Enemy.h"
#include "Simon.h"
class Hunchback : public Enemy
{
	bool isActive;
	DWORD timeReady;
	bool isJumping;
public:
	Hunchback(float x1, float y1, int nx1);
	~Hunchback();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, Simon * simon, vector<LPGAMEOBJECT> *coObjects = NULL);
	void SetState(int state);
	void Render();

};

#endif 