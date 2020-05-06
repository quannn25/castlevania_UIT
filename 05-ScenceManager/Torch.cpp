#include "Torch.h"


void Torch::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + TORCH_BBOX_WIDTH;
	bottom = y + TORCH_BBOX_HEIGHT;

}

void Torch::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}