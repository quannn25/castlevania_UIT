#ifndef BAT_H
#define BAT_H

#include "Enemy.h"
#include "Simon.h"

#define BAT_SPEED_X 0.3f 
#define BAT_SPEED_Y 0.1f
#define BAT_FRAMEWIDTH 32
#define BAT_FRAMEHEIGHT 32

#define DeltaY 60


class Bat : public Enemy
{
private:
	float deltaY;
	bool isIdle;
public:
	Bat(float x1 = 0, float y1 = 0, int nx1 = -1);
	virtual ~Bat();

	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, Simon * simon,vector<LPGAMEOBJECT> *listObject = NULL);
	void Render();
};

#endif 