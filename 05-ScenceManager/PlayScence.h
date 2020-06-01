#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Torch.h"
#include "Candle.h"
#include "MainSimon.h"
#include "map.h"
#include "grid.h"
#include "Board.h"
#include "DaggerItem.h"
#include "Dagger.h"
#include "Effect.h"
#include "Fire.h"
#include "Hit.h"
#include "GameTime.h"
#include "Monney.h"
#include "SmallHeart.h"
#include "Stair.h"
#include "BlackKnight.h"
#include "Zombie.h"
#include "ZombieZone.h"

class CPlayScene: public CScene
{
protected: 
	Simon *player;					// A play scene has to have player, right? 
	
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> coObjects; // các obj trong vùng camera

	Map *tileMap;

	Grid *grid;

	Board *boardGame;

	vector <Item*> listItem;

	vector <Effect*> listEffect;

	GameTime * gameTime; //unload()??? hay truyền

	vector <Enemy*> listEnemy;

	DWORD TimeZombie; // thoi gian tao zombie vừa rồi
	vector <ZombieZone*> listZombieZone; // list các vùng simon vào thì cho zombie ra tại các vị trí dc parse vào

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP(string line);
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void LoadResources();
	virtual void Load();
	virtual void LoadAgain();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void CheckCollision();
	void CheckCollisionWeapon();
	void CheckCollisionSimonWithItem();

	bool isOncam(float x1, float y1, float w1, float h1);

	void ResetResource(); // reset lai resource khi simon mất 1 mạng

	Item * GetNewItem(int id, eType type, float x, float y);

	void CreateZombie();

	void updateEnemy(DWORD dt);

	friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

