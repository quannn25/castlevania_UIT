#ifndef EFFECT_H
#define EFFECT_H

#include "Animations.h"
#include "Camera.h"



class Effect
{
protected:
	LPANIMATION_SET animation_set;

	float x;
	float y;
	bool isFinish;

public:
	Effect();
	~Effect();

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	virtual void Render();
	virtual void Update();
	bool GetFinish();
	void SetFinish(int b);
};
#endif
