#include "Fire.h"

Fire::Fire(float x1, float y1)
{
	this->x = x1;
	this->y = y1;

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(FIREEFFECT_ANI_SET_ID);
	SetAnimationSet(ani_set);
	isFinish = false;
	curFrame = -1;
	lastFrameTime = 0;
}


Fire::~Fire()
{
}

void Fire::Update(DWORD dt)
{
	animation_set->at(0)->setLastFrameTime(this->lastFrameTime);// điều này chỉ để gán lại cho nó đúng frame của nó thôi ( mỗi obj có frame riêng), chứ ko phải render trong update
	animation_set->at(0)->setCurrentFrame(this->curFrame); // gán lại bởi vì obj khác dùng chung aniset này

	if (animation_set->at(0)->getCurrentFrame() == 3) // nếu là frame cuối thì xong
	{
		isFinish = true;
		//animation_set->at(0)->setCurrentFrame(0); // tất cả các đối tượng Effect này dùng chung ani_set này thì currentFrame cũng chung :(
		// set lại oke rồi, vậy ổn ko?
	}
}

void Fire::Render()
{
	animation_set->at(0)->setLastFrameTime(this->lastFrameTime);
	animation_set->at(0)->setCurrentFrame(this->curFrame);

	animation_set->at(0)->Render(x, y);

	this->curFrame = animation_set->at(0)->getCurrentFrame();
	this->lastFrameTime = animation_set->at(0)->getLastFrameTime();
}