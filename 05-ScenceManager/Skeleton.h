#ifndef SKELETON_H
#define SKELETON_H

#include "Enemy.h"
#include "Simon.h"
#include"Bone.h"

#define SKELETON_ACTIVEZONE 450

#define SKELETON_FRAMEWIDTH 32
#define SKELETON_FRAMEHEIGHT 63

#define SKELETON_SPEED_Y 0.5f
#define SKELETON_SPEED_X 0.12f
#define SEKELETON_GRAVITY 0.0011f

#define ZONEATTACK_MAX 150
#define ZONEATTACK_MIN 68

#define SKELETON_STATE_IDLE 0
#define SKELETON_STATE_ATTACK 1
#define SKELETON_STATE_JUMP 2

class Skeleton : public Enemy
{
	bool isJumping;
	bool isAllowJump;
	vector <Bone*> listBone;
public:
	Skeleton(float X, float Y, int Nx);
	~Skeleton();
	virtual void Update(DWORD dt, Simon *simon, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

#endif