#include "ObjectCreateBoss.h"


ObjectCreateBoss::ObjectCreateBoss(float x1, float y1, float _xBoss, float _yBoss, float _leftBoundaryBoss, float _rightBoundaryBoss)
{
	this->x = x1;
	this->y = y1;
	this->xBoss = _xBoss;
	this->yBoss = _yBoss;
	this->leftBoundaryBoss = _leftBoundaryBoss;
	this->rightBoundaryBoss = _rightBoundaryBoss;

	width = 32;
	height = 300;
	isActive = false;
}

ObjectCreateBoss::~ObjectCreateBoss()
{
}

void ObjectCreateBoss::Render()
{
	RenderBoundingBox();
}

void ObjectCreateBoss::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

bool ObjectCreateBoss::getIsActive()
{
	return isActive;
}

void ObjectCreateBoss::setIsActive(bool a)
{
	this->isActive = a;
}
