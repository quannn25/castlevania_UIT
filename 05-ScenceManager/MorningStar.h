#ifndef MORNINGSTAR_H
#define MORNINGSTAR_H

#include "Weapon.h"
#include "LargeHeart.h"
#include "Torch.h"

#define MORNINGSTAR_FRAMEWIDTH 160
#define MORNINGSTAR_FRAMEHEIGHT 68

class MorningStar : public Weapon
{
protected:
	int level;
public:
	MorningStar();
	~MorningStar();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
	void RenderIcon(float x1, float y1);
	virtual void ReSetAniSetSwitchScene();

	virtual void Create(float simonX, float simonY, int simonTrend);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	virtual void UpdatePositionFitSimon();

	bool isCollision(LPGAMEOBJECT obj);

	void UpgradeLevel();
};

#endif
