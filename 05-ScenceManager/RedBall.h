#ifndef REDBALL_H
#define	REDBALL_H
#include "Item.h"

#define REDBALL_GRAVITY 0.25f

#define REDBALL_TIMEDISPLAYMAX 5000
#define REDBALL_FRAMEWIDTH 28
#define REDBALL_FRAMEHEIGHT 32
#define REDBALL_TIMEWAITMAX 300

class RedBall : public Item
{
public:
	RedBall(float x = 0.0f, float y = 0.0f);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* listObject);

	~RedBall();
};

#endif 
