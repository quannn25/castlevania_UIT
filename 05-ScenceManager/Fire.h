#ifndef FIRE_H
#define FIRE_H

#include "Effect.h"

// w 42 , h 44

class Fire : public Effect
{


public:
	Fire(int x = 0, int y = 0);
	~Fire();

	void Update();
};
#endif