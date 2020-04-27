#include "GameTime.h"

GameTime * GameTime::__instance = NULL;

GameTime *GameTime::GetInstance()
{
	if (__instance == NULL) __instance = new GameTime();
	return __instance;
}

GameTime::GameTime()
{
	this->accumulationTime = 0;
	frameStart = GetTickCount();
}


GameTime::~GameTime()
{
}

void GameTime::Update()
{
	DWORD now = GetTickCount();
	DWORD dt = now - frameStart;

	DebugOut(L"dt time = %d \n", dt);

	if (accumulationTime + dt < 1000)
	{
		accumulationTime += dt;
	}
	else
	{
		accumulationTime = (accumulationTime + dt) % 1000;
		time++;
	}

	frameStart = now;
}

void GameTime::SetTime(int t)
{
	time = t;
}

int GameTime::GetTime()
{
	return time;
}