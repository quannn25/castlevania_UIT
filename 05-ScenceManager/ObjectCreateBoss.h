#ifndef OBJECTCREATEBOSS_H
#define OBJECTCREATEBOSS_H
#include "GameObject.h"

class ObjectCreateBoss : public CGameObject
{
public:
	float width;
	float height;

	float xBoss;
	float yBoss;

	float leftBoundaryBoss;
	float rightBoundaryBoss;

	bool isActive; // đã hoạt động hay chưa
public:
	ObjectCreateBoss(float x1, float y1, float xBoss, float yBoss, float _leftBoundaryBoss, float _rightBoundaryBoss);
	~ObjectCreateBoss();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	virtual bool getIsActive();
	virtual void setIsActive(bool a);

};

#endif