#include "Sprites.h"
#include "Game.h"
#include "Utils.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;

	spriteHandler = CGame::GetInstance()->GetSpriteHandler();

}

CSprites * CSprites::__instance = NULL;

CSprites *CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

void CSprite::Draw(float x, float y, int alpha)
{
	CGame * game = CGame::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, alpha);
}



void CSprite::DrawFlipX(float x, float y, int alpha)
{
	RECT r = GetRectFrame();

	D3DXMATRIX AA, BB;
	spriteHandler->GetTransform(&AA);
	D3DXMatrixTransformation2D(&BB, &D3DXVECTOR2(x, x), 0.0f, &D3DXVECTOR2(-1.0f, 1.0f), NULL, 0.0f, NULL);
	D3DXMATRIX CC = AA * BB;
	spriteHandler->SetTransform(&CC);
	x -= (right - left);
	//D3DXVECTOR3 p(trunc(x), trunc(y), 0);
	//spriteHandler->Draw(texture, &r, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	CGame * game = CGame::GetInstance();
	game->DrawFlipX(x, y, texture, r, alpha);
	spriteHandler->SetTransform(&AA);
}

void CSprite::DrawWithRect(RECT r, float x, float y, int alpha)
{
	CGame * game = CGame::GetInstance();
	game->Draw(x, y, texture, r.left, r.top, r.right, r.bottom, alpha);
}

RECT CSprite::GetRectFrame()
{
	RECT res;
	res.left = left;
	res.top = top;
	res.right = right;
	res.bottom = bottom;
	return res;
}

void CSprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
	sprites[id] = s;

	DebugOut(L"[INFO] sprite added: %d, %d, %d, %d, %d \n", id, left, top, right, bottom);
}

LPSPRITE CSprites::Get(int id)
{
	return sprites[id];
}

/*
	Clear all loaded textures
*/
void CSprites::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}



