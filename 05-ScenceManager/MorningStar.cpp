#include "MorningStar.h"



MorningStar::MorningStar()
{
	// quản lý sprite chạy từ 0 => 3 bằng cách set StateChange trong hàm Render
	//tạo animationSet trong file txt luôn cho MorningStar (tạo 4 sprite) aniMationSet_id = 44
	//

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

void MorningStar::GetBoundingBox(float & left, float & top, float & right, float & bottom) // danh toi frame 2 bi dung dot ngot
{
	if (level == 0) // 160 weight_ 68 height define lại
	{
		if (nx == 1)
		{
			left = x + 80;
			top = y + 15;
			right = x + MORNINGSTAR_FRAMEWEIGHT - 30;
			bottom = y + MORNINGSTAR_FRAMEHEIGHT - 35;
		}
		else
		{
			left = x + 30;
			top = y + 15;
			right = x + MORNINGSTAR_FRAMEWEIGHT - 85;
			bottom = y + MORNINGSTAR_FRAMEHEIGHT - 35;

		}

	}

}

void MorningStar::CollisionWithObject(DWORD dt, vector<LPGAMEOBJECT>* listObj)
{
	if (animation_set->at(0)->getCurrentFrame() != 2)
		return;

	RECT rect, rect1;
	float l, t, r, b;
	float l1, t1, r1, b1;

	GetBoundingBox(l, t, r, b);
	rect.left = l;
	rect.top = t;
	rect.right = r;
	rect.bottom = b;



	for (int i = 0; i < listObj->size(); i++)
	{
		if (listObj->at(i)->GetHealth() > 0 && dynamic_cast<Torch *>(listObj->at(i)))
		{
			listObj->at(i)->GetBoundingBox(l1, t1, r1, b1);
			rect1.left = l1;
			rect1.top = t1;
			rect1.right = r1;
			rect1.bottom = b1;
			if (CGame::GetInstance()->AABBCheck(rect, rect1))
			{
				listObj->at(i)->beAttacked(1);
				ItemManager::GetInstance()->ListItem.push_back(Weapon::GetNewItem(listObj->at(i)->id, listObj->at(i)->GetType(), listObj->at(i)->x, listObj->at(i)->y));
			}
		}
	}
}