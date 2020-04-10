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
}

void Camera::Update()
{
	if (viewport.x < 0)
		viewport.x = 0;

	if (viewport.x > MapWidth - Window_Width)
		viewport.x = MapWidth - Window_Width;

}

Camera::~Camera()
{
}

D3DXVECTOR2 Camera::Transform(int x, int y)
{
	//return D3DXVECTOR2(x - viewport.x, y - viewport.y);
	return D3DXVECTOR2(x, y);
}

void Camera::SetPosition(float x, float y)
{
	viewport.x = x;
	viewport.y = y;
}


D3DXVECTOR2 Camera::GetViewport()
{
	return viewport;
}
