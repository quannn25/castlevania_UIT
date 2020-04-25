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

	bool isFinish;

public:
	Weapon();
	~Weapon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Create(float simonX, float simonY, int simonNx);
	virtual void Render();

	virtual void UpdatePositionFitSimon();
	bool GetFinish();
	void SetFinish(bool b);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual bool isCollision(LPGAMEOBJECT obj) = 0; // kt va chạm với từng Obj

};
#endif