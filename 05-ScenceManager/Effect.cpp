#include "Effect.h"



Effect::Effect()
{
	isFinish = 0;
}


Effect::~Effect()
{
}

void Effect::Render()
{
	animation_set->at(0)->Render(x, y);
}

void Effect::Update()
{
}

bool Effect::GetFinish()
{
	return isFinish;
}

void Effect::SetFinish(int b)
{
	isFinish = b;
}