#ifndef SIMON_H
#define SIMON_H


#define SIMON_POSITION_DEFAULT  50.0f, 0

#define SIMON_BBOX_WIDTH 43
#define SIMON_BBOX_HEIGHT 63

#define SIMON_BBOX_SITTING_HEIGHT 45



#define SIMON_GRAVITY 0.005f
#define SIMON_VJUMP 0.8f

#define SIMON_WALKING_SPEED 0.15f 
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_DIE_DEFLECT_SPEED	0.5f


#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMPING			300
#define SIMON_STATE_SITTING			400
#define SIMON_STATE_DIE				2
#define SIMON_STATE_RIGHT			3
#define SIMON_STATE_LEFT			4
#define SIMON_STATE_STOP			5



#define SIMON_ANI_WALKING 1

#define SiMON_ANI_IDLE 0

#define SIMON_ANI_JUMPING 3

#define SIMON_ANI_SITTING 2
#define SIMON_ANI_DIE 4

#define SIMON_UNTOUCHABLE_TIME 5000




#include "GameObject.h"
class Simon : public CGameObject
{
public:

	bool isWalking;
	bool isJumping;
	bool isSitting;



	int level; // mario 123
	int untouchable;
	DWORD untouchable_start;


public:
	Simon();
	~Simon();



	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);


	virtual void Render(Camera * camera);
	virtual void Render();

	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }


	void Left();  // set lại hướng của simon
	void Right(); // set lại hướng của simon
	void Go();
	void Sit();
	void Jump();


	void Stop();

};






#endif