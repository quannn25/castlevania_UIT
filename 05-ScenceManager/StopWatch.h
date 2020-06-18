#ifndef STOPWATCH_H
#define STOPWATCH_H

#include "Weapon.h"
class StopWatch : public Weapon
{
protected:
	DWORD ActiveTime;

public:
	StopWatch();
	~StopWatch();

	virtual void Create(float simonX, float simonY, int simonNx);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	virtual bool isCollision(LPGAMEOBJECT obj);
	virtual void RenderIcon(float x1, float y1);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void ReSetAniSetSwitchScene();

};

#endif 