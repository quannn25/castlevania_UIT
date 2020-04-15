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

void MorningStar::Update(int dt) // sẽ chạy cùng simonUpdate để đảm bảo dt
{// thời gian update rồi tới render sẽ mất thêm đoạn dt, nên render sai thời gian thực??
	// mang dt vào render luôn??
	int curFrame = animation_set->at(0)->getCurrentFrame();

	if (curFrame == 3)
	{
		isFinish = true;
	}


	if (level == 0)// kiểm tra sai frame ko
	{ 
		if (curFrame >= MORNINGSTAR_ANI_LEVEL0_END) // tới frame 3 thì quay lại 0
		{
			animation_set->at(0)->setCurrentFrame(0);
		}
	}
}

void MorningStar::Create(float simonX, float simonY, int simonNx)
{
	Weapon::Create(simonX, simonY, simonNx);

	UpdatePositionFitSimon();

	if (level == 0)
	{
		animation_set->at(0)->setCurrentFrame(0);
	}
}

void MorningStar::UpdatePositionFitSimon()
{
	if (nx == -1)
	{
		this->x = x - 76;

	}
	else
	{
		this->x = x - 25;
	}
}
