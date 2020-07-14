#ifndef CAMERA_H
#define CAMERA_H

#include "define.h"

class Camera
{
	static Camera * __instance;
protected:
	float x;
	float y;

	int screen_width;
	int screen_height;

	bool isBlockCamera;

public:
	Camera(int w, int h);
	~Camera();
	
	void SetPosition(float x, float y);

	void Update();

	float Getx() { return x; }
	float Gety() { return y; }

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	void SetScreenWidth(int width) { this->screen_width = width; }
	void SetScreenHeight(int height) { this->screen_height = height; }

	bool getIsBlockCamera() { return isBlockCamera; }
	void setIsBlockCamera(bool a) { this->isBlockCamera = a; }

	static Camera * GetInstance();
	
};

#endif



