#ifndef BOARD_H
#define BOARD_H

#include "camera.h"
#include "Font.h"
#include "Animations.h"
#include "Simon.h"
#include "BoardHealth.h"

class Board
{
private:
	LPANIMATION_SET animation_set;
	float x, y;

	Font font;
	BoardHealth boardHealth;
public:
	Board(float x, float y);
	void Render(Simon *simon, int state);
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	string FillNumber(string s, int MaxNumber);
	~Board();
};
#endif