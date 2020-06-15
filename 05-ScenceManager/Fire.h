#ifndef FIRE_H
#define FIRE_H

#include "Effect.h"

// w 42 , h 44

class Fire : public Effect
{


public:
	Fire(float x = 0, float y = 0);
	~Fire();

	void Update(DWORD dt);
};
#endif