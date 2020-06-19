#include "Boomerang.h"




Boomerang::Boomerang()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BOOMERANG_ANI_SET_ID);
	SetAnimationSet(ani_set);

	type = eType::BOOMERANG;
	isFinish = true;
}


void Boomerang::RenderIcon(float X, float Y)
{
	animation_set->at(1)->Render(X, Y);

}

void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt); //update dt dx dy
	x += dx;


	bool isInCamera = true;
	if (this->nx < 0)
	{
		if (x <= Camera::GetInstance()->Getx())
			isInCamera = false;
	}
	else
	{
		if (x + BOOMERANG_FRAMEWIDTH >= Camera::GetInstance()->Getx() + Camera::GetInstance()->GetScreenWidth())
			isInCamera = false;
	}


	switch (StatusProcess)
	{
	case BOOMERANG_PROCESS_1: // b1 boomerang đi theo hướng ném
	{
		if (abs(xCreate - x) >= BOOMERANG_LIMIT_DISTANCE_X || !isInCamera) // bay đủ xa or chạm cạnh camera thì sang B2
		{
			nx = -nx;
			x -= dx;
			vx = -vx;
			StatusProcess = BOOMERANG_PROCESS_2;
		}
		break;
	}

	case BOOMERANG_PROCESS_2: // b2 boomerang đi ngược hướng ném
	{
		if (!isInCamera || isCollision(MainSimon::GetInstance()->GetSimon())) // nếu boomerang ra khỏi cam, hoặc quay lại chạm simon
		{
			isFinish = true;
		}
		break;
	}
	default:
		DebugOut(L"[ERROR] Boomerang not found Process!\n");
		break;

	}

}

void Boomerang::Create(float X, float Y, int Nx)
{
	if (isFinish == false)
		return;
	Weapon::Create(X, Y, Nx);
	UpdatePositionFitSimon();
	vx = BOOMERANG_SPEED_X * Nx;
	vy = 0;

	StatusProcess = BOOMERANG_PROCESS_1; // ném theo hướng simon

	xCreate = X;

}

Boomerang::~Boomerang()
{
}

void Boomerang::ReSetAniSetSwitchScene()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BOOMERANG_ANI_SET_ID);

	SetAnimationSet(ani_set);
}

void Boomerang::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANG_FRAMEWIDTH;
	bottom = y + BOOMERANG_FRAMEHEIGHT;
}

bool Boomerang::isCollision(LPGAMEOBJECT obj)
{

	CGameObject *gameObj = dynamic_cast<CGameObject*>(obj);
	if (gameObj->GetHealth() <= 0)
		return false;

	return isCollitionObjectWithObject(gameObj);
}

void Boomerang::Render()
{
	bool isLeft = true;
	if (nx > 0)
		isLeft = false;
	if (isLeft)
	{
		animation_set->at(2)->Render(x, y); // at(0) chuỗi hành động đầu tiên của set (LEVEL 0)
	}
	else
		animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}