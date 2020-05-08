#ifndef SIMON_H
#define SIMON_H


#define SIMON_POSITION_DEFAULT  50.0f, 0.0f

#define SIMON_BBOX_WIDTH 60
#define SIMON_BBOX_HEIGHT 66

#define SIMON_BBOX_SITTING_HEIGHT 48

#define SIMON_GRAVITY 0.005f

#define SIMON_WALKING_SPEED 0.45f 
#define SIMON_JUMP_SPEED_Y		0.8f
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
#define SIMON_ANI_SIT_ATTACK	5
#define SIMON_ANI_STAND_ATTACK	6
#define SIMON_ANI_STAIR1	7
#define SIMON_ANI_STAIR2	8

#define SIMON_UNTOUCHABLE_TIME 5000

#define SIMON_DEFAULT_HEALTH 16
#define SIMON_DEFAULT_HEARTCOLLECT 5

#define TIME_FREEZE_MAX 500

#define SIMON_SPEED_ONSTAIR 0.09f



#include "GameObject.h"
#include "Weapon.h"
#include "MorningStar.h"
class Simon : public CGameObject
{
private:

	int level; // mario 123

	int live; // mạng sống
	int score;

	int heartCollected; // mana

	int untouchable;
	DWORD untouchable_start;

	float xBackup;
	float yBackup;

	bool isFreeze; // trạng thái đóng băng chóp chóp
	DWORD TimeFreeze; // thời gian đã đóng băng

public:
	Weapon * mainWeapon;
	Weapon* subWeapon;

	bool isWalking;
	bool isJumping;
	bool isSitting;
	bool isAttacking;

	bool isOnStair;
	int isWalkingOnStair;  // có 2 giai đoạn 
	float walkHeight = 0;//

public:
	Simon();
	~Simon();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);

	virtual void Render();

	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void CollisionWithBrick(vector<LPGAMEOBJECT> *coObjects = NULL);
	void Attack(Weapon *w);
	bool isCollisionWithItem(Item * objItem);

	void SetHeartCollected(int h);
	int GetHeartCollected();
	int GetLive();
	void SetLive(int l);
	int GetScore();
	void SetScore(int s);

	bool LoseLife();
	void SetPositionBackup(float x, float y);  // sau khi chết thì hồi sinh tại vị trí này

	bool GetFreeze();
	void SetFreeze(int f);
	void UpdateFreeze(DWORD dt);

	void GoUpStair();

};
#endif