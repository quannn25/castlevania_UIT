#ifndef ZOMBIEZONE_H
#define ZOMBIEZONE_H
#include"Simon.h"
#include "Game.h"

class ZombieZone
{
public:
	float l, t, r, b; // tọa độ Simon trong Box này thì trong vùng cần xuất zombie
	float xZombie1, xZombie2; // biên x muốn Zombie xuất hiện khi Simon trong zone _ x1 biên trái, x2 biên phải __ -1 theo camera
	float yZombie1, yZombie2; // biên y muốn Zombie xuất hiện khi Simon trong zone
	DWORD timeCreateZombie; //time tạo ra bầy zombie
	DWORD lastTimeCreate;

	int totalZombie;
	int countZombie;
public:
	bool isSimonInZoneBefore; // kiểm tra có phải simon vừa ở bên ngoài rồi vào zone ko, hay ở trong zone nảy giờ
public:
	ZombieZone(float l, float t, float r, float b, float xZombie1, float yZombie1, float xZombie2, float yZombie2 , int time, int totalZombie);
	~ZombieZone();

	void RenderBoundingBox();

	bool isSimonInZombieZone(Simon *simon);

	DWORD getTimeCreateZombie();

	int getTotalZombie();

	int getCountZombie();
	void setCountZombie(int n);

	DWORD getLastTimeCreate();
	void setLastTimeCreate(int lastTime);

	int getFlag(); // xZombie1 == -1 thì tạo zombie theo camera

	void getCreateLocation(float &xZombie1, float &yZombie1, float &xZombie2, float &yZombie2);

	void getBoundingBox(float &l, float &t, float &r, float &b);

};
#endif  