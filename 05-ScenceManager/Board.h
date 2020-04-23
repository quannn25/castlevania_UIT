#ifndef BOARD_H
#define BOARD_H

#include "Sprites.h"
#include "Textures.h"
#include "camera.h"
#include "Font.h"
#include "Animations.h"

class Board
{
private:
	LPANIMATION_SET animation_set;
	Font font;
	float x, y;
	string information;
public:
	Board(float x, float y);
	void Render();
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	~Board();
};
#endif