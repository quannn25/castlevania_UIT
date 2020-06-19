#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"
class Enemy : public CGameObject
{
public:
	bool isHurt;
public:
	Enemy();
	~Enemy();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();

};
#endif  