#include "HolyWater.h"



HolyWater::HolyWater()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(HOLYWATER_ANI_SET_ID);
	SetAnimationSet(ani_set);

	//_spriteIcon = new GSprite(TextureManager::GetInstance()->GetTexture(eID::ITEMHOLYWATER), 200); // trong aniset->at(1)

	type = eType::HOLYWATER;
	isCollisionBrick = false;
	isFinish = true;
}

HolyWater::~HolyWater()
{

}

void HolyWater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (animation_set->at(0)->getCurrentFrame() == 3) // là frame cuối cùng thì kết thúc
	{
		isFinish = true;
	}

	if (isFinish)
		return;

	Weapon::Update(dt); //update dt dx dy
	if (!isCollisionBrick)
		vy += HOLYWATER_GRAVITY * dt;

	vector<LPGAMEOBJECT> listObject_Brick;
	listObject_Brick.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
		if (coObjects->at(i)->GetType() == eType::BRICK)
			listObject_Brick.push_back(coObjects->at(i));

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(&listObject_Brick, coEvents); // Lấy danh sách các va chạm 
	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (nx != 0)
		{
			vy = 0;
			vx = 0;
			isCollisionBrick = true;

		}

		if (ny != 0)
		{
			vy = 0;
			vx = 0;
			isCollisionBrick = true;
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void HolyWater::Create(float simonX, float simonY, int simonTrend)
{
	if (isFinish == false)
		return;
	Weapon::Create(simonX, simonY, simonTrend);
	UpdatePositionFitSimon();
	vx = HOLLYWATER_SPEED_X * simonTrend;
	vy = -HOLLYWATER_SPEED_Y;
	isCollisionBrick = false;
	animation_set->at(0)->setCurrentFrame(-1); // set về frame đầu
	//_sprite->SelectIndex(0);
}

void HolyWater::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x - 5;
	top = y;
	right = x + HOLYWATER_FRAMEWIDTH + 5;
	bottom = y + HOLYWATER_FRAMEHEIGHT;
	if (animation_set->at(0)->getCurrentFrame() == 0) // frame đầu
	{
		left += 5;
		top += 5;
		right -= 5;
		bottom -= 0;
	}
}

void HolyWater::UpdatePositionFitSimon()
{
	y += 20;
}

void HolyWater::RenderIcon(float X, float Y)
{
	animation_set->at(1)->Render(X, Y);
	//_spriteIcon->Draw(X, Y);
}

bool HolyWater::isCollision(LPGAMEOBJECT obj)
{
	if (isFinish == true)
		return false;
	// dt, dx, dy đã update
	CGameObject *gameObj = dynamic_cast<CGameObject*>(obj);
	if (gameObj->GetHealth() <= 0) // vật này die rồi thì ko va chạm
		return false;
	return isCollitionObjectWithObject(obj);
}

void HolyWater::Render()
{
	if (isFinish == true)
		return;
	if (!isCollisionBrick) // chưa va chạm đất thì cứ frame 0, cho time vẽ frame 0 thật nhỏ để va chạm thì 0 chuyển sang 1 thật nhanh
		animation_set->at(0)->setCurrentFrame(-1);

	Weapon::Render();
}

void HolyWater::ReSetAniSetSwitchScene()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(HOLYWATER_ANI_SET_ID);

	SetAnimationSet(ani_set);
}