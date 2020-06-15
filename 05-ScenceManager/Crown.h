#ifndef CROWN_H
#define CROWN_H
#include "Item.h"

#define CROWN_GRAVITY 0.05f

#define CROWN_TIMEDISPLAYMAX 10000
#define CROWN_TIMEWAITMAX 1

#define CROWN_FRAMEWIDTH 32
#define CROWN_FRAMEHEIGHT 32

class Crown : public Item
{
protected:
	float y_backup;
public:
	Crown(float x = 0.0f, float y = 0.0f);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* listObject);
	void Render();

	~Crown();
};

#endif 
