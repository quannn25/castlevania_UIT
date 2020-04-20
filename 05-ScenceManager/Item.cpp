#include "Item.h"



Item::Item()
{
	TimeDisplayed = 0;
}


Item::~Item()
{
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* listObject)
{
}

void Item::Render()
{
	if (isFinish == true)
		return;

	animation_set->at(0)->Render(x, y, 255);
	RenderBoundingBox();
}

//void Item::SetPosition(float X, float Y)
//{
//	this->x = X;
//	this->y = Y;
//}

//eID Item::GetType()
//{
//	return type;
//}

//void Item::RenderBoundingBox()
//{
//	D3DXVECTOR3 p(x, y, 0);
//	RECT rect;
//
//	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);
//
//	float l, t, r, b;
//
//	GetBoundingBox(l, t, r, b);
//	rect.left = 0;
//	rect.top = 0;
//	rect.right = (int)r - (int)l;
//	rect.bottom = (int)b - (int)t;
//
//	CGame::GetInstance()->Draw(l, t, bbox, rect.left, rect.top, rect.right, rect.bottom, 92);
//
//}

bool Item::GetFinish()
{
	return isFinish;
}

void Item::SetFinish(bool f)
{
	isFinish = f;
}