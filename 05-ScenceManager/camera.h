#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "define.h"

class Camera
{
	static Camera * __instance;
protected:
	D3DXVECTOR2 viewport;
	int screen_width;
	int screen_height;

public:
	Camera(int w, int h);
	~Camera();
	


	D3DXVECTOR2 Transform(int x, int y);

	void SetPosition(float x, float y);

	D3DXVECTOR2 GetViewport();
	void Update();

	float Getx() { return viewport.x; }
	float Gety() { return viewport.y; }

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	void SetScreenWidth(int width) { this->screen_width = width; }
	void SetScreenHeight(int height) { this->screen_height = height; }

	static Camera * GetInstance();
	
};

#endif



