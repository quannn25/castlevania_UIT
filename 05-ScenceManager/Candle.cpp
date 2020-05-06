#include "Candle.h"




void Candle::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + CANDLE_FRAMEWIDTH;
	bottom = y + CANDLE_FRAMEHEIGHT;

}

void Candle::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}