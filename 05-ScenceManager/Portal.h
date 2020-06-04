#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 
	int switchType; // 0 là switch đến môi trường dc setup như QUAY TRỜ LẠI scene nào đó, //1 là switch đến môi trường đc setup như MỚI ĐẾN scene nào đó

	int width;
	int height; 
public:
	CPortal(float l, float t, float r, float b, int scene_id, int switchType);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	int GetSceneId() { return scene_id;  }
	int GetSwitchType();
};