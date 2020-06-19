#include "Axe.h"



Axe::Axe()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(AXE_ANI_SET_ID);
	SetAnimationSet(ani_set);

	type = eType::AXE;
	isFinish = true;
}


Axe::~Axe()
{
}

void Axe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (x + AXE_FRAMEWIDTH <= Camera::GetInstance()->Getx() || x >= Camera::GetInstance()->Getx() + Camera::GetInstance()->GetScreenWidth() || y > Camera::GetInstance()->GetScreenHeight())
	{
		isFinish = true;
	}

	Weapon::Update(dt); //update dt dx d 
	vy += AXE_GRAVITY * dt;

	y += dy;
	x += dx;
}

void Axe::Create(float simonX, float simonY, int simonNx)
{
	if (isFinish == false)
		return;
	Weapon::Create(simonX, simonY, simonNx);
	UpdatePositionFitSimon();
	vx = AXE_SPEED_X * simonNx;
	vy = -AXE_SPEED_Y;
	animation_set->at(0)->setCurrentFrame(-1);
}

void Axe::RenderIcon(float X, float Y)
{
	animation_set->at(1)->Render(X, Y);
}

void Axe::ReSetAniSetSwitchScene()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(AXE_ANI_SET_ID);

	SetAnimationSet(ani_set);
}

void Axe::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + AXE_FRAMEWIDTH;
	bottom = y + AXE_FRAMEHEIGHT;
}

bool Axe::isCollision(LPGAMEOBJECT obj)
{

	CGameObject *gameObj = dynamic_cast<CGameObject*>(obj);
	if (gameObj->GetHealth() <= 0)
		return false;

	return isCollitionObjectWithObject(gameObj);
}