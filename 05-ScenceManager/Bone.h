#ifndef BONE_H
#define BONE_H

#define BONE_FRAMEWIDTH 32
#define BONE_FRAMEHEIGHT 32
#define BONE_SPEED_X 0.2f
#define BONE_SPEED_Y 0.55f
#define BONE_GRAVITY 0.0015f
#define COUNT_BONE_FRAME 3


#include "Weapon.h"
#include "MainSimon.h"
class Bone : public Weapon
{
public:
	int curFrame;
	DWORD lastFrameTime;

	Bone();
	~Bone();

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	void Render();
	void Create(float simonX, float simonY, int simonNx);
	virtual void RenderIcon(float X, float Y);
	virtual void ReSetAniSetSwitchScene();
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	bool isCollision(LPGAMEOBJECT obj);

};

#endif