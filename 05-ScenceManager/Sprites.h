#ifndef CSPRITE_H
#define CSPRITE_H
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

using namespace std;

class CSprite
{
	int id;				// Sprite ID in the sprite database

	int left; 
	int top;
	int right;
	int bottom;
	int x_draw;

	LPDIRECT3DTEXTURE9 texture;
	LPD3DXSPRITE spriteHandler;
public: 
	CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex, int x_draw);
	void DrawFlipX(float x, float y, int alpha = 255);

	void Draw(float x, float y, int alpha = 255);

	void DrawWithRect(RECT rect, float x, float y, int alpha = 255);

	RECT GetRectFrame();

	int GetXDraw();
};

typedef CSprite * LPSPRITE;

/*
	Manage sprite database
*/
class CSprites
{
	static CSprites * __instance;

	unordered_map<int, LPSPRITE> sprites;

public:
	void Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex, int x_draw);
	LPSPRITE Get(int id);
	void CSprites::Clear();

	static CSprites * GetInstance();
};


#endif