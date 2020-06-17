#include "ZombieZone.h"

ZombieZone::ZombieZone(float l, float t, float r, float b, float xZombie1, float yZombie1, float xZombie2, float yZombie2, int time, int total)
{
	this->l = l;
	this->t = t;
	this->r = r;
	this->b = b;
	this->xZombie1 = xZombie1;
	this->yZombie1 = yZombie1;
	this->xZombie2 = xZombie2;
	this->yZombie2 = yZombie2;
	this->timeCreateZombie = time;
	this->totalZombie = total;

	this->countZombie = 0;
	this->lastTimeCreate = 0;

	this->isSimonInZoneBefore = false;
}

bool ZombieZone::isSimonInZombieZone(Simon * simon)
{
	if (simon->GetX() >= l && simon->GetX() <= r && simon->GetY() >= t && simon->GetY() <= b)
	{
		return true;
	}
	return false;
}

DWORD ZombieZone::getTimeCreateZombie()
{
	return timeCreateZombie;
}

int ZombieZone::getTotalZombie()
{
	return totalZombie;
}

int ZombieZone::getCountZombie()
{
	return countZombie;
}

void ZombieZone::setCountZombie(int n)
{
	countZombie = n;
}

DWORD ZombieZone::getLastTimeCreate()
{
	return lastTimeCreate;
}

void ZombieZone::setLastTimeCreate(int lastTime)
{
	this->lastTimeCreate = lastTime;
}

int ZombieZone::getFlag()
{
	return xZombie1;
}

void ZombieZone::getCreateLocation(float &xZombie1, float &yZombie1, float &xZombie2, float &yZombie2)
{
	xZombie1 = this->xZombie1;
	yZombie1 = this->yZombie1;
	xZombie2 = this->xZombie2;
	yZombie2 = this->yZombie2;
}

void ZombieZone::RenderBoundingBox()
{
	D3DXVECTOR3 p(l, t, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(l, t, bbox, rect.left, rect.top, rect.right, rect.bottom, 160);
}

ZombieZone::~ZombieZone()
{
}

void ZombieZone::getBoundingBox(float &l, float &t, float &r, float &b)
{
	l = this->l;
	t = this->t;
	r = this->r;
	b = this->b;
}