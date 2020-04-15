#ifndef MORNINGSTAR_H
#define MORNINGSTAR_H

#define MORNINGSTAR_ANI_LEVEL0_START 0
#define MORNINGSTAR_ANI_LEVEL0_END 3


#include "Weapon.h"

class MorningStar : public Weapon
{
protected:
	int level;

public:
	MorningStar();
	~MorningStar();
	virtual void Update();
	virtual void Create(float simonX, float simonY, int simonTrend);

	virtual void UpdatePositionFitSimon();

};



#endif // !__MORNINGSTAR_H__
