#include "Camera.h"

Camera * Camera::__instance = NULL;

Camera *Camera::GetInstance()
{
	if (__instance == NULL) __instance = new Camera(Window_Width, Window_Height);
	return __instance;
}

Camera::Camera(int w, int h)
{
	screen_width = w;
	screen_height = h;
	isBlockCamera = false;
}

void Camera::Update()
{

	if (x > MapWidth - Window_Width)
		x = MapWidth - Window_Width;

	if (x < 0)
		x = 0;

}

Camera::~Camera()
{
}

void Camera::SetPosition(float x1, float y1)
{
	x = x1;
	y = y1;
}

