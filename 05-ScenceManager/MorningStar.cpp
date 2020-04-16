#include "MorningStar.h"



MorningStar::MorningStar()
{
	// quản lý sprite chạy từ 0 => 3 bằng cách set StateChange trong hàm Render
	//tạo animationSet trong file txt luôn cho MorningStar (tạo 4 sprite) aniMationSet_id = 44
	//

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(MORNINGSTAR_ANI_SET_ID);

	SetAnimationSet(ani_set);
	this->level = 0;
}


MorningStar::~MorningStar()
{
}

void MorningStar::Update() // sẽ chạy cùng simonUpdate để đảm bảo dt
{// thời gian update rồi tới render sẽ mất thêm đoạn dt, nên render sai thời gian thực??
	// mang dt vào render luôn??
	int curFrame = animation_set->at(0)->getCurrentFrame();

	if (curFrame == 3)
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
	
}

void MorningStar::Create(float simonX, float simonY, int simonNx)
{
	Weapon::Create(simonX, simonY, simonNx);

	UpdatePositionFitSimon();

	if (level == 0)
	{
		animation_set->at(0)->setCurrentFrame(-1);
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

void MorningStar::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (level == 0)
	{
		if (nx == 1)
		{
			left = x + 10;
			top = y + 15;
			right = x + 160 - 30; // 160 weight_ 68 height define lại
			bottom = y + 68 - 15;
		}
		else
		{
			left = x + 30;
			top = y + 15;
			right = x + 160 - 10;
			bottom = y + 68 - 15;

		}

	}

}

void MorningStar::CollisionWithObject(DWORD dt, vector<LPGAMEOBJECT>* listObj)
{
	RECT rect, rect1;
	float l, t, r, b;
	float l1, t1, r1, b1;

	GetBoundingBox(l, t, r, b);
	rect.left = (int)l;
	rect.top = (int)t;
	rect.right = (int)r;
	rect.bottom = (int)b;



	for (int i = 0; i< listObj->size(); i++)
		if (listObj->at(i)->GetHealth()>0 && listObj->at(i)->GetType() == eID::TORCH)
		{
			listObj->at(i)->GetBoundingBox(l1, t1, r1, b1);
			rect1.left = (int)l1;
			rect1.top = (int)t1;
			rect1.right = (int)r1;
			rect1.bottom = (int)b1;
			if (CGame::GetInstance()->AABBCheck(rect, rect1))
			{
				listObj->at(i)->beAttacked(1);
			}
		}
}