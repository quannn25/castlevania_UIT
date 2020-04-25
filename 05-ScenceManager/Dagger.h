#ifndef DAGGER_H
#define DAGGER_H

#include "Weapon.h"

#define DAGGER_SPEED 0.35f

class Dagger : public Weapon
{
public:
	Dagger();
	~Dagger();

	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);

	void Create(float simonX, float simonY, int simonNx);

	void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	bool isCollision(LPGAMEOBJECT obj);

	void Render();
};

#endif
