#ifndef BOARDHEALTH_H
#define BOARDHEALTH_H

#include "Sprites.h"
#include "Textures.h"
#include "Animations.h"
#include "define.h"
#define BOARDHEALTH_MAX_CELL 16

#define FRAME_PLAYER 0
#define FRAME_NONE 1
#define FRAME_ENEMY 2

#define BOARDHEALTH_FRAMEWIDTH 11
#define BOARDHEALTH_FRAMEHEIGHT 18

class BoardHealth
{
private:
	LPANIMATION_SET animation_set;

public:
	BoardHealth();
	~BoardHealth();
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	void Draw(float x, float y, int PlayerHealth, int EnemyHealth);
};
#endif