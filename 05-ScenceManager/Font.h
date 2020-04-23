#ifndef FONT_H
#define FONT_H

#include "Sprites.h"
#include "Textures.h"
#include "define.h"
#include "Animations.h"

#define FONT_FRAMEWIDTH 15
#define FONT_FRAMEHEIGHT 14


class Font
{
private:
	LPANIMATION_SET animation_set;

public:
	Font();
	~Font();
	void Draw(float x, float y, string s);
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
};
#endif 