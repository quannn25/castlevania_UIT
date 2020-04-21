#ifndef MORNINGSTAR_H
#define MORNINGSTAR_H



#include "Weapon.h"
#include "ItemManager.h"
#include "LargeHeart.h"
#include "Torch.h"

#define MORNINGSTAR_FRAMEWEIGHT 160
#define MORNINGSTAR_FRAMEHEIGHT 68

class MorningStar : public Weapon
{
protected:
	int level;
	float xBackup, yBackup;
public:
	MorningStar();
	~MorningStar();
	virtual void Update();
	virtual void Create(float simonX, float simonY, int simonTrend);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void UpdatePositionFitSimon();
	void CollisionWithObject(DWORD dt, vector<LPGAMEOBJECT>* listObj);
	void UpgradeLevel();
};



#endif
