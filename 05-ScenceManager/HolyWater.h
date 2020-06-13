#ifndef  HOLLYWATER_H
#define HOLLYWATER_H


#define HOLLYWATER_SPEED_X 0.24f
#define HOLLYWATER_SPEED_Y 0.25f
#define HOLYWATER_GRAVITY 0.0015f

#define HOLYWATER_FRAMEWIDTH 32
#define HOLYWATER_FRAMEHEIGHT 26
#include "Weapon.h"


class HolyWater : public Weapon
{
private:
	bool isCollisionBrick;

public:
	HolyWater();
	~HolyWater();

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);

	void Create(float simonX, float simonY, int simonNx);

	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void UpdatePositionFitSimon();

	void RenderIcon(float X, float Y);
	bool isCollision(LPGAMEOBJECT obj);
	void Render();

	virtual void ReSetAniSetSwitchScene();

};



#endif  