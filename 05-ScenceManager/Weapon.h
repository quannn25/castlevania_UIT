#ifndef WEAPON_H
#define	WEAPON_H

#include "define.h"
#include "Camera.h"
#include "Animations.h"
#include "define.h"

class Weapon
{
protected:
	eID TypeWeapon;

	float vx;
	float vy;

	int nx;

	bool isFinish;

	float x;
	float y;

	LPANIMATION_SET animation_set;

public:
	Weapon();
	~Weapon();

	int GetNx();
	void SetNx(int nx1);

	virtual void Create(float simonX, float simonY, int simonTrend);
	virtual void Render();


	virtual void SetPosition(float X, float Y);
	virtual void UpdatePositionFitSimon();
	int GetFinish();
	void SetFinish(bool b);
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	eID GetType();
};



#endif