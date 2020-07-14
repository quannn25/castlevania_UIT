#ifndef HIDDENOBJECT_H
#define HIDDENOBJECT_H
#include "GameObject.h"

class HiddenObject : public CGameObject
{
protected:
	float width;
	float height;

	float xItem;
	float yItem;

	bool isActive; // đã hoạt động hay chưa
public:
	HiddenObject(float x1, float y1, float xItem, float yItem);
	HiddenObject(float x1, float y1);
	~HiddenObject();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual bool getIsActive();
	virtual void setIsActive(bool a);

	float getXItem();
	float getYItem();

};

#endif