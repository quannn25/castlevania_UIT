#ifndef HIT_H
#define HIT_H

#include "Effect.h"

class Hit : public Effect
{
public:
	Hit(float x = 0, float y = 0);
	~Hit();

	void Update(DWORD dt);
};

#endif