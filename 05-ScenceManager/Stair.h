#ifndef STAIR_H
#define STAIR_H

#include "GameObject.h"
class Stair : public CGameObject
{
private:
	int width;
	int height;
	int t; // 1 ở dưới, 2 ở trên

public:
	Stair(float x1, float y1, int t1);
	~Stair();


	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();

};

#endif