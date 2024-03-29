﻿#ifndef GAMETIME_H
#define GAMETIME_H

#include "define.h"

#define GAMETIME_SCENE_1 300

class GameTime
{
	static GameTime * __instance;
private:
	DWORD accumulationTime; // thời gian đã tích lũy
	int time; // time game

public:
	GameTime();
	~GameTime();
	void Update(DWORD dt);
	void SetTime(int t);
	int GetTime();

	static GameTime * GetInstance();
};
#endif 