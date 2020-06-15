#ifndef BROKENBRICK_H
#define BROKENBRICK_H

#include "Effect.h"

#define BROKENBRICK_GRAVITY 0.03f 


class BrokenBrick : public Effect
{
private:
	int typeBrick;

	DWORD dt;
	float dx;
	float dy;
	float vx;
	float vy;
	int nx;

public:
	BrokenBrick(int X, int Y, int type);
	~BrokenBrick();
	void Update(DWORD dt);
	void Render();
};


#endif  