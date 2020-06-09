#ifndef SIMON_H
#define SIMON_H


#define SIMON_POSITION_DEFAULT  50.0f, 0.0f

#define SIMON_BBOX_WIDTH 32
#define SIMON_BBOX_HEIGHT 64
#define SIMON_BBOX_SITTING_HEIGHT 48

#define SIMON_GRAVITY 0.005f
#define SIMON_WALKING_SPEED 0.17f //0.12
#define SIMON_JUMP_SPEED_Y		0.8f
#define SIMON_DIE_DEFLECT_SPEED	0.5f
#define SIMON_SPEED_ONSTAIR 0.09f

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
#define SIMON_ANI_STAIR_UP_1	7
#define SIMON_ANI_STAIR_UP_2	8
#define SIMON_ANI_STAIR_DOWN_1	9
#define SIMON_ANI_STAIR_DOWN_2	10
#define SIMON_ANI_STAIR_UP_ATTACK 11
#define SIMON_ANI_STAIR_DOWN_ATTACK 12
#define SIMON_ANI_FREEZE 13
#define SIMON_ANI_HURTING 14

#define SIMON_UNTOUCHABLE_TIME 2000

#define SIMON_DEFAULT_HEALTH 16
#define SIMON_DEFAULT_HEARTCOLLECT 5

#define TIME_FREEZE_MAX 500



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

	bool untouchable;
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

	bool isHurting;

	bool isOnStair;
	int isWalkingOnStair;  // có 2 giai đoạn 
	float walkHeight = 0;//
	int NxStair; // để đảo chiều leo lên xuống stair
	int ny; // -1 đi hướng lên

	// backup trạng thái
	bool isWalking_Backup;
	bool isJumping_Backup;
	bool isSitting_Backup;
	bool isAttacking_Backup;

	bool isOnStair_Backup;
	int isWalkingOnStair_Backup;
	int NxStair_Backup;
	int ny_Backup;

	// thông số auto
	float xAuto;
	float speedAuto;
	int NxAuto;
	float AutoGoX_Backup_X; // x trước khi auto

	bool isAutoGoX;
	int NxAfterAuto; //Nx mong muốn sao khi auto

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

	void CollisionWhenOnStair(vector<LPGAMEOBJECT> *coObjects = NULL);

	void SetAutoGoX(int NxAuto, int NxAfterAuto1, float xAuto1, float Speed); // cài đặt auto và backup trạng thái hiện tại
	void RestoreAfterAutoGoX(); // khôi phục trạng thái

	void SetHurt(LPCOLLISIONEVENT e);

	bool getUntouchable();
	void setUntouchable(bool a);
	DWORD getUntouchable_start();
	void setUntouchable_start(int time);

	void CollisionWithPortal(vector<LPGAMEOBJECT> *coObjects = NULL);

};
#endif