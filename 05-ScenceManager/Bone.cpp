#include "Bone.h"



Bone::Bone()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BONE_ANI_SET_ID);
	SetAnimationSet(ani_set);

	type = eType::BONE;
	isFinish = true;
	curFrame = rand() % COUNT_BONE_FRAME;
	lastFrameTime = 0;
}


Bone::~Bone()
{
}

void Bone::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isFinish)
		return;
	if (x + BONE_FRAMEWIDTH <= Camera::GetInstance()->Getx() || x >= Camera::GetInstance()->Getx() + Camera::GetInstance()->GetScreenWidth() || y > Camera::GetInstance()->GetScreenHeight())
	{
		isFinish = true;
		return;
	}

	

	Weapon::Update(dt); //update dt dx d 
	vy += BONE_GRAVITY * dt;

	y += dy;
	x += dx;
}

void Bone::Create(float simonX, float simonY, int simonNx)
{
	if (isFinish == false)
		return;
	Weapon::Create(simonX, simonY, simonNx);
	UpdatePositionFitSimon();
	vx = BONE_SPEED_X * simonNx;
	vy = -BONE_SPEED_Y;
	animation_set->at(0)->setCurrentFrame(curFrame);
}

void Bone::RenderIcon(float X, float Y)
{
	animation_set->at(1)->Render(X, Y);
}

void Bone::Render()
{
	bool isLeft = true;
	if (nx > 0)
		isLeft = false;

	animation_set->at(0)->setLastFrameTime(this->lastFrameTime);
	animation_set->at(0)->setCurrentFrame(this->curFrame);

	animation_set->at(0)->Render(x, y, 255, isLeft);

	this->curFrame = animation_set->at(0)->getCurrentFrame();
	this->lastFrameTime = animation_set->at(0)->getLastFrameTime();

	RenderBoundingBox();
}

void Bone::ReSetAniSetSwitchScene()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BONE_ANI_SET_ID);

	SetAnimationSet(ani_set);
}

void Bone::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BONE_FRAMEWIDTH;
	bottom = y + BONE_FRAMEHEIGHT;
}

bool Bone::isCollision(LPGAMEOBJECT obj)
{

	CGameObject *gameObj = dynamic_cast<CGameObject*>(obj);
	if (gameObj->GetHealth() <= 0)
		return false;

	return isCollitionObjectWithObject(gameObj);
}