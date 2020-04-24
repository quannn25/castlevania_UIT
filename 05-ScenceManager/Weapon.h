#ifndef WEAPON_H
#define	WEAPON_H

#include "define.h"
#include "Camera.h"
#include "Animations.h"
#include "define.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Item.h"
#include "LargeHeart.h"
#include "UpgradeMorningStar.h"

class Weapon : public CGameObject
{
protected:
	eID Type;

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
	virtual void Create(float simonX, float simonY, int simonNx);
	virtual void Render();


	virtual void SetPosition(float X, float Y);
	virtual void UpdatePositionFitSimon();
	bool GetFinish();
	void SetFinish(bool b);
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	void RenderBoundingBox();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual bool isCollision(LPGAMEOBJECT obj) = 0; // kt va chạm với từng Obj

	eID GetType();
};
#endif