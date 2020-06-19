#ifndef  AXE_H
#define AXE_H

#define AXE_FRAMEWIDTH 32
#define AXE_FRAMEHEIGHT 32
#define AXE_SPEED_X 0.2f
#define AXE_SPEED_Y 0.55f
#define AXE_GRAVITY 0.0015f


#include "Weapon.h"
class Axe : public Weapon
{
public:

	Axe();
	~Axe();

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	void Create(float simonX, float simonY, int simonNx);
	virtual void RenderIcon(float X, float Y);
	virtual void ReSetAniSetSwitchScene();
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	bool isCollision(LPGAMEOBJECT obj);

};

#endif