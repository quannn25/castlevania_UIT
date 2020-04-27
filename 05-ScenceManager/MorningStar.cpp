#include "MorningStar.h"



MorningStar::MorningStar()
{
	//tạo animationSet trong file txt luôn cho MorningStar (tạo 4 sprite) aniMationSet_id = 44

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(MORNINGSTAR_ANI_SET_ID);

	SetAnimationSet(ani_set);
	type = eID::MORNINGSTAR;
	this->level = 0;
}


MorningStar::~MorningStar()
{
}

void MorningStar::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) // sẽ chạy cùng simonUpdate để đảm bảo dt
{
	this->dt = dt;
	this->dx = vx * dt;
	this->dy = vy * dt;
	
	// thời gian update rồi tới render sẽ mất thêm đoạn dt, nên render sai thời gian thực??
	// mang dt vào render luôn??
	int curFrame = animation_set->at(0)->getCurrentFrame();
	int curFrame1 = animation_set->at(1)->getCurrentFrame();
	int curFrame2 = animation_set->at(2)->getCurrentFrame();
	if ((curFrame == 3 && level == 0) || (curFrame1 == 3 && level == 1) || (curFrame2 == 3 && level == 2))
	{
		isFinish = true;
	}

	switch (level)
	{
	case 0:
		if (curFrame >= 3) // tới frame 3 thì quay lại 0
		{
			animation_set->at(0)->setCurrentFrame(-1);
		}
		break;
	case 1:
		if (curFrame1 >= 3) // tới frame 3 thì quay lại 0
		{
			animation_set->at(1)->setCurrentFrame(-1);
		}
		break;
	case 2:
		if (curFrame2 >= 3) // tới frame 3 thì quay lại 0
		{
			animation_set->at(2)->setCurrentFrame(-1);
		}
		break;
	}
}

void MorningStar::Create(float simonX, float simonY, int simonNx)
{
	Weapon::Create(simonX, simonY, simonNx);

	UpdatePositionFitSimon();

	switch (level)
	{
	case 0:
		animation_set->at(0)->setCurrentFrame(-1);
		break;
	case 1:
		animation_set->at(1)->setCurrentFrame(-1);
		break;
	case 2:
		animation_set->at(2)->setCurrentFrame(-1);
		break;
	}
}

void MorningStar::UpdatePositionFitSimon()
{
	if (nx < 0)
	{
		this->x = x - 65;

	}
	else
	{
		this->x = x - 30;
	}
}

void MorningStar::GetBoundingBox(float & left, float & top, float & right, float & bottom) // danh toi frame 2 bi dung dot ngot
{
	switch (level)
	{
	case 0:
	{
		if (nx == 1)
		{
			left = x + 78;
			top = y + 15;
			right = x + MORNINGSTAR_FRAMEWIDTH - 30;
			bottom = y + MORNINGSTAR_FRAMEHEIGHT - 30;
		}
		else
		{
			left = x + 30;
			top = y + 15;
			right = x + MORNINGSTAR_FRAMEWIDTH - 80;
			bottom = y + MORNINGSTAR_FRAMEHEIGHT - 30;

		}
		break;
	}
	case 1:
	{
		if (nx == 1)
		{
			left = x + 78;
			top = y + 15;
			right = x + MORNINGSTAR_FRAMEWIDTH - 30;
			bottom = y + MORNINGSTAR_FRAMEHEIGHT - 30;
		}
		else
		{
			left = x + 30;
			top = y + 15;
			right = x + MORNINGSTAR_FRAMEWIDTH - 80;
			bottom = y + MORNINGSTAR_FRAMEHEIGHT - 30;

		}
		break;
	}
	case 2:
	{
		if (nx == 1)
		{
			left = x + 78;
			top = y + 15;
			right = x + MORNINGSTAR_FRAMEWIDTH;
			bottom = y + MORNINGSTAR_FRAMEHEIGHT - 30;
		}
		else
		{
			left = x;
			top = y + 15;
			right = x + MORNINGSTAR_FRAMEWIDTH - 80;
			bottom = y + MORNINGSTAR_FRAMEHEIGHT - 30;

		}
		break;
	}
	default:
		break;
	}

}

bool MorningStar::isCollision(LPGAMEOBJECT obj)
{
	if (level == 0 && animation_set->at(0)->getCurrentFrame() != 2)
		return false;

	if (level == 1 && animation_set->at(1)->getCurrentFrame() != 2)
		return false;

	if (level == 2 && animation_set->at(2)->getCurrentFrame() != 2)
		return false;

	CGameObject *gameObj = dynamic_cast<CGameObject*>(obj);
	if (gameObj->GetHealth() <= 0)
		return false;

	return isCollitionObjectWithObject(gameObj);
}

void MorningStar::UpgradeLevel()
{
	if (level >= 2)
		return;
	level++;
}

void MorningStar::Render()
{
	bool isLeft = true;
	if (nx > 0)
		isLeft = false;

	switch (level)
	{
	case 0:
		animation_set->at(0)->Render(x, y, 255, isLeft);
		break;
	case 1:
		animation_set->at(1)->Render(x, y, 255, isLeft);
		break;
	case 2:
		animation_set->at(2)->Render(x, y, 255, isLeft);
		break;
	}

	RenderBoundingBox();
}

void MorningStar::RenderIcon(float x1, float y1)
{
}