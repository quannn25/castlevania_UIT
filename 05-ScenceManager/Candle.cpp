#include "Candle.h"




void Candle::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x - 5;
	top = y - 5;
	right = x + CANDLE_FRAMEWIDTH + 5;
	bottom = y + CANDLE_FRAMEHEIGHT + 5;

}

void Candle::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}