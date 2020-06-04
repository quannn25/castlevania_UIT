#ifndef DEFINE_H
#define DEFINE_H

#define _USE_MATH_DEFINES

#include <d3d9.h>
#include <d3dx9.h>
#include <map> 
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;


#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 499 //448

#define ID_SPRITE_TILEMAP	50001

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_TILEMAP	7
#define SCENE_SECTION_LOADSIMON_0	8
#define SCENE_SECTION_LOADSIMON_1	9


#define OBJECT_TYPE_SIMON	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_TORCH	2
#define OBJECT_TYPE_CANDLE	3
#define OBJECT_TYPE_STAIR	4
#define OBJECT_TYPE_BLACKKNIGHT 5
#define OBJECT_TYPE_ZOMBIEZONE 7
#define OBJECT_TYPE_SPECIALBRICK 8
#define OBJECT_TYPE_SPECIALBRICKSMALL 9
#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024

#define MORNINGSTAR_ANI_SET_ID 44
#define LARGEHEART_ANI_SET_ID 45
#define UPGRADEMORNINGSTAR_ANI_SET_ID 46
#define FONT_ANI_SET_ID 47
#define BOARD_ANI_SET_ID 48
#define DAGGER_ANI_SET_ID 49
#define BOARDHEALTH_ANI_SET_ID 50
#define FIREEFFECT_ANI_SET_ID 51
#define HITEFFECT_ANI_SET_ID 52
#define	MONNEY_ANI_SET_ID 53
#define SMALLHEART_ANI_SET_ID 54
#define ZOMBIE_ANI_SET_ID 56

#define TIME_CREATE_GHOST 1000

extern int Window_Width;
extern int Window_Height;

extern int MapWidth;
extern int MapHeight;


extern int ScreenColumn;
extern int ScreenRow;


// ID của Sprite, object
enum eType
{
	SIMON = 0,


	BRICK = 1,

	TORCH = 2,

	PORTAL = 50,

	MORNINGSTAR = 51,

	LARGEHEART = 52,

	UPGRADEMORNINGSTAR = 53,

	DAGGERITEM = 54,

	DAGGER = 55,

	FIRE = 56,

	HIT = 57,

	MONNEY = 58,

	CANDLE = 59,

	SMALLHEART = 60,

	STAIR_UP = 61,

	STAIR_DOWN = 62,

	BLACKKNIGHT = 63,

	ZOMBIE = 64,

	ZOMBIEZONE = 65,

	SPECIALBRICK = 66,

	SPECIALBRICKSMALL = 67

};


#endif
