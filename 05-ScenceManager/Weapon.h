#ifndef WEAPON_H
#define	WEAPON_H

#include "define.h"
#include "Camera.h"
#include "Animations.h"
#include "define.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"

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

	virtual void Update();
	virtual void Create(float simonX, float simonY, int simonTrend);
	virtual void Render();


	virtual void SetPosition(float X, float Y);
	virtual void UpdatePositionFitSimon();
	int GetFinish();
	void SetFinish(bool b);
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	void RenderBoundingBox();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;

	eID GetType();
};



#endif