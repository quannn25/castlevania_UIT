#include "Fire.h"

Fire::Fire(float x1, float y1)
{
	this->x = x1;
	this->y = y1;

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(FIREEFFECT_ANI_SET_ID);

	SetAnimationSet(ani_set);
}


Fire::~Fire()
{
}

void Fire::Update()
{
	if (animation_set->at(0)->getCurrentFrame() == 3) // nếu là frame cuối thì xong, frame cuối trống
	{
		isFinish = true;
		animation_set->at(0)->setCurrentFrame(0); // tất cả các đối tượng Effect này dùng chung ani_set này thì currentFrame cũng chung :(
		// set lại oke rồi, vậy ổn ko?
	}
}