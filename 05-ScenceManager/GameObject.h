﻿#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

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
	eType type; // loai obj
public:

	float x; 
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;	// vx > 0 quay sang phai, vx = 0 idle 
	float vy;

	int nx;	 // nx > 0 quay sang phai

	int state; 

	DWORD dt; 

	LPANIMATION_SET animation_set; // ham destroy??

	int id; // id obj

	bool onCam;
	

public: 
	virtual void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	float GetX() { return x; }
	float GetY() { return y; }
	float GetVx() { return vx; }
	float GetVy() { return vy; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	int GetState() { return this->state; }

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
	bool isCollitionObjectWithObject(LPGAMEOBJECT obj); // kt doi voi Obj xac dinh

	CGameObject();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }

	void beAttacked(int damage);
	int GetHealth();
	void SetHealth(int h);
	int GetNx() { return nx; }
	void SetNx(int Nx);
	void SetId(int ID);
	int GetId() { return id; }
	void SetType(eType t) { this->type = t; }
	eType GetType() { return type; }
	bool GetOnCam() { return onCam; }
	void SetOnCam(bool a) { onCam = a; }


	~CGameObject();
};

#endif