#include "Camera.h"

Camera * Camera::__instance = NULL;

Camera *Camera::GetInstance()
{
	if (__instance == NULL) __instance = new Camera();
	return __instance;
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
