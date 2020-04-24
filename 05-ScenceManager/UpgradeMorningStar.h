#ifndef UPGRADEMORNINGSTAR_H
#define UPGRADEMORNINGSTAR_H

#define UPGRADEMORNINGSTAR_GRAVITY 0.25f

#define UPGRADEMORNINGSTAR_TIMEDISPLAYMAX 3000

#define UPGRADEMORNINGSTAR_FRAMEWIDTH 32
#define UPGRADEMORNINGSTAR_FRAMEHEIGHT 32

#include "Item.h"
class UpgradeMorningStar : public Item
{
public:
	UpgradeMorningStar();
	UpgradeMorningStar(float X, float Y);

	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *listObject = NULL);


	~UpgradeMorningStar();
};
#endif