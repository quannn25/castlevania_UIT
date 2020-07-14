#ifndef FIRE_H
#define FIRE_H

#include "Effect.h"

#define FIRE_WIDTH 32
#define FIRE_HEIGHT 28
// w 42 , h 44

class Fire : public Effect
{
	int curFrame;
	DWORD lastFrameTime;

public:
	Fire(float x = 0, float y = 0);
	~Fire();

	void Update(DWORD dt);
	void Render();
};
#endif