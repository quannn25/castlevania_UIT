#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"
#include "define.h"
#include "camera.h"
#include "Utils.h"


using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
protected:
	int health;
public:

	float x; 
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;	// vx > 0 quay sang phai, vx = 0 idle 
	float vy;

	int nx;	 // nx > 0 quay sang phai

	int state; // trend

	DWORD dt; 

	LPANIMATION_SET animation_set; // ham destroy??

	eID type; // loai obj

	int id; // id obj

	bool onCam;
	
	int stateChange; // true thì đã đổi trạng thái nên set frame lại từ đầu, false là tiếp tục trạng thái cũ - tăng currentFrame lên
	// stateChange dùng để quản lý currentFrame trong mỗi CAnimation, thay cho index của của texture đc chia hàng và cột

public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	int GetState() { return this->state; }
	int GetStateChange() { return this->stateChange; }

	void RenderBoundingBox();

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx, 
		float &rdy);

	CGameObject();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	virtual void SetStateChange(int stateChange) { this->stateChange = stateChange; }

	int GetHealth();
	void SetNx(int Nx);
	void SetId(int ID);


	~CGameObject();
};

