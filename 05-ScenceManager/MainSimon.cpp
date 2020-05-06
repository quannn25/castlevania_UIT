#include "MainSimon.h"

MainSimon * MainSimon::__instance = NULL;

MainSimon *MainSimon::GetInstance()
{
	if (__instance == NULL) __instance = new MainSimon();
	return __instance;
}

Simon * MainSimon::GetSimon()
{
	return simon;
}

void MainSimon::SetSimon(Simon *s)
{
	simon = s;
}

MainSimon::~MainSimon()
{
}

MainSimon::MainSimon()
{
}