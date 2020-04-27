#ifndef HIT_H
#define HIT_H

#include "Effect.h"

class Hit : public Effect
{
public:
	Hit(int x = 0, int y = 0);
	~Hit();

	void Update();
};

#endif