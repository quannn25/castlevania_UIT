#ifndef PHANTOMBAT_H
#define PHANTOMBAT_H
#include "Simon.h"
#include "Enemy.h"

#define PHANTOMBAT_DEFAULT_HEALTH 2
#define PHANTOMBAT_HURT_TIME 1000

#define PHANTOMBAT_FRAMEWIDTH 96
#define PHANTOMBAT_FRAMEHEIGHT 46

#define PHANTOMBAT_PROCESS_SLEEP 0

#define PHANTOMBAT_PROCESS_START_1 1 // lúc bắt đầu, Bat di chuyển xuống
#define PHANTOMBAT_PROCESS_START_2 2 // lúc bắt đầu, Bat di chuyển đến cửa sổ (nửa vòng cung)

#define PHANTOMBAT_PROCESS_CURVES 3 // lúc di chuyển theo vòng cung

#define PHANTOMBAT_PROCESS_STRAIGHT_1 4 // xử lí di chuyển thẳng lần 1
#define PHANTOMBAT_PROCESS_STRAIGHT_2 5 // xử lí di chuyển thẳng lần 2

#define PHANTOMBAT_PROCESS_ATTACK 6 // xử lí tấn công

#define PHANTOMBAT_PROCESS_START1_VY 0.05f

#define PHANTOMBAT_PROCESS_START2_X 1380
#define PHANTOMBAT_PROCESS_START2_Y 201
#define PHANTOMBAT_PROCESS_START2_SECOND 1500.0f
#define PHANTOMBAT_PROCESS_START2_V 0.0001f
#define PHANTOMBAT_PROCESS_START2_RANDOM 1500

#define PROCESS_SMART_DISTANCE 150.0f


class PhantomBat : public Enemy
{
public:
	int StatusProcessing;

private:

	float xBefore;
	float yBefore;

	float xTarget;
	float yTarget;
	Simon * simon;

	bool isUseBezierCurves = false;
	float x1;
	float y1;

	float x2;
	float y2;

	float x3;
	float y3;

	float yLastFrame; // y của frame trước đó

	DWORD TimeWaited; // thời gian đã chờ
	bool isWaiting;

	float leftBoundary;
	float rightBoundary;

	DWORD hurtTime;

public:
	PhantomBat(float X, float Y, float _leftBoundary, float _rightBoundary, Simon *_simon);
	~PhantomBat();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();

	void Start();
	void StartCurves();
	void StartStaight();
	void ProcessSmart();

	float getPt(float n1, float n2, float perc);
};

#endif