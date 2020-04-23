#include "MorningStar.h"



MorningStar::MorningStar()
{
	//tạo animationSet trong file txt luôn cho MorningStar (tạo 4 sprite) aniMationSet_id = 44

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(MORNINGSTAR_ANI_SET_ID);

	SetAnimationSet(ani_set);
	Type = eID::MORNINGSTAR;
	this->level = 0;
}


MorningStar::~MorningStar()
{
}

void MorningStar::Update() // sẽ chạy cùng simonUpdate để đảm bảo dt
{// thời gian update rồi tới render sẽ mất thêm đoạn dt, nên render sai thời gian thực??
	// mang dt vào render luôn??
	int curFrame = animation_set->at(0)->getCurrentFrame();
	int curFrame1 = animation_set->at(1)->getCurrentFrame();
	if (curFrame == 3 || curFrame1 == 3)
	{
		isFinish = true;
	}


	if (level == 0)// kiểm tra sai frame ko
	{ 
		if (curFrame >= 3) // tới frame 3 thì quay lại 0
		{
			animation_set->at(0)->setCurrentFrame(-1);
		}
	}

	
	if (level == 1)
	{
		if (curFrame1 >= 3) // tới frame 3 thì quay lại 0
		{
			animation_set->at(1)->setCurrentFrame(-1);
		}
	}
}

void MorningStar::Create(float simonX, float simonY, int simonNx)
{
	Weapon::Create(simonX, simonY, simonNx);

	UpdatePositionFitSimon();

	if (level == 0)
	{
		animation_set->at(0)->setCurrentFrame(-1);
	}

	if (level == 1)
	{
		animation_set->at(1)->setCurrentFrame(-1);
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
	if (level == 0) // 160 width_ 68 height define lại
	{
		if (nx == 1)
		{
			left = x + 80;
			top = y + 15;
			right = x + MORNINGSTAR_FRAMEWIDTH - 30;
			bottom = y + MORNINGSTAR_FRAMEHEIGHT - 30;
		}
		else
		{
			left = x + 30;
			top = y + 15;
			right = x + MORNINGSTAR_FRAMEWIDTH - 85;
			bottom = y + MORNINGSTAR_FRAMEHEIGHT - 30;
		}
	}

	if (level == 1)
	{
		if (nx == 1)
		{
			left = x + 80;
			top = y + 15;
			right = x + MORNINGSTAR_FRAMEWIDTH - 30;
			bottom = y + MORNINGSTAR_FRAMEHEIGHT - 30;
		}
		else
		{
			left = x + 30;
			top = y + 15;
			right = x + MORNINGSTAR_FRAMEWIDTH - 85;
			bottom = y + MORNINGSTAR_FRAMEHEIGHT - 30;
		}
	}

}

bool MorningStar::isCollision(LPGAMEOBJECT obj)
{
	if (animation_set->at(0)->getCurrentFrame() != 2)
		return false;

	CGameObject *gameObj = dynamic_cast<CGameObject*>(obj);
	if (gameObj->GetHealth() <= 0)
		return false;

	RECT rect, rect1;
	float l, t, r, b;
	float l1, t1, r1, b1;

	GetBoundingBox(l, t, r, b);
	rect.left = l;
	rect.top = t;
	rect.right = r;
	rect.bottom = b;

	gameObj->GetBoundingBox(l1, t1, r1, b1);
	rect1.left = l1;
	rect1.top = t1;
	rect1.right = r1;
	rect1.bottom = b1;
	return CGame::GetInstance()->AABBCheck(rect, rect1);
}

void MorningStar::UpgradeLevel()
{
	if (level >= 2)
		return;
	level++;
}