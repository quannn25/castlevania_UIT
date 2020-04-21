#ifndef DEFINE_H
#define DEFINE_H

#define _USE_MATH_DEFINES

#include <d3d9.h>
#include <d3dx9.h>
#include <map> 
#include "Utils.h"
using namespace std;

#define COLOR_BLACK D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)  
#define COLOR_KEY D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f) 


#define SHOWBOX_PINK 0


#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 448

#define ID_SPRITE_TILEMAP	50001
#define ID_SPRITE_BOARDGAME 50002

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_TILEMAP	7

#define OBJECT_TYPE_SIMON	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_TORCH	2
#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024

#define MORNINGSTAR_ANI_SET_ID 44
#define LARGEHEART_ANI_SET_ID 45
#define UPGRADEMORNINGSTAR_ANI_SET_ID 46

extern int Window_Width;
extern int Window_Height;

extern int MapWidth;
extern int MapHeight;


extern int ScreenColumn;
extern int ScreenRow;


// ID của Sprite, object
enum eID
{
	SIMON = 0,


	BRICK = 1,

	TORCH = 2,

	PORTAL = 50,

	MORNINGSTAR = 51,

	LARGEHEART = 52,

	UPGRADEMORNINGSTAR = 53

};



typedef D3DXVECTOR3 GVector3;
typedef D3DXVECTOR2 GVector2;
#define VECTOR2ZERO GVector2(0.0f, 0.0f)









#ifndef SAFE_DELETE

#define SAFE_DELETE(ptr) \
if(ptr) \
{\
	delete (ptr); \
	ptr = nullptr; \
} \

#endif // !SAFE_DELETE

#endif // !_DEFINE_H__
