#include "HiddenObject.h"


HiddenObject::HiddenObject(float x1, float y1, float xItem1, float yItem1)
{
	this->x = x1;
	this->y = y1;
	this->xItem = xItem1;
	this->yItem = yItem1;

	width = 16;
	height = 16;
	isActive = false;
}

HiddenObject::~HiddenObject()
{
}

void HiddenObject::Render()
{
	RenderBoundingBox();
}

void HiddenObject::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

bool HiddenObject::getIsActive()
{
	return isActive;
}

void HiddenObject::setIsActive(bool a)
{
	this->isActive = a;
}

float HiddenObject::getXItem()
{
	return xItem;
}

float HiddenObject::getYItem()
{
	return yItem;
}
