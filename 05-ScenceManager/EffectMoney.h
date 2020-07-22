#ifndef EFFECTMONEY_H
#define EFFECTMONEY_H


#define EFFECTMONEY_FITX 20
#define EFFECTMONEY_FITY 20
#include "Effect.h"
class EffectMoney : public Effect
{
public:
	EffectMoney(float X, float Y, eType typeEffectMoney);
	void Update(DWORD dt);
	virtual ~EffectMoney();
};

#endif 