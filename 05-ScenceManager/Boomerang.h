#ifndef  BOOMERANG_H
#define BOOMERANG_H

#define BOOMERANG_FRAMEWIDTH 32
#define BOOMERANG_FRAMEHEIGHT 32
#define BOOMERANG_SPEED_X 0.3f
#define BOOMERANG_PROCESS_1 1 // bước 1 ném theo hướng simon
#define BOOMERANG_PROCESS_2 2 // bước 2 ném ngược hướng ban đầu
#define BOOMERANG_LIMIT_DISTANCE_X 150.0f // boomerang đi được 285px thì đổi hướng

#include "Weapon.h" 
#include "MainSimon.h"
class Boomerang : public Weapon
{
private:
	int StatusProcess;

	float xCreate; // vị trí x boomerang lúc khởi tạo

public:
	Boomerang();
	~Boomerang();

	virtual void RenderIcon(float X, float Y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	void Create(float X, float Y, int Nx);
	virtual void ReSetAniSetSwitchScene();
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	bool isCollision(LPGAMEOBJECT obj);
	void Render();

};

#endif 