#ifndef TORCH_H
#define TORCH_H

#include "GameObject.h"

#define TORCH_BBOX_WIDTH 32
#define TORCH_BBOX_HEIGHT 64
class Torch : public CGameObject
{

public:

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Render();

};

#endif 