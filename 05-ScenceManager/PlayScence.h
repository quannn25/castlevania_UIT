#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Torch.h"
#include "Simon.h"
#include "map.h"
#include "grid.h"
#include "Board.h"
#include "DaggerItem.h"
#include "Dagger.h"
#include "Effect.h"
#include "Fire.h"
#include "Hit.h"
#include "GameTime.h"

class CPlayScene: public CScene
{
protected: 
	Simon *player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> coObjects; // unLoad() ko cần delete

	Map *tileMap;

	Grid *grid;

	Board *boardGame;

	vector <Item*> listItem;

	vector <Effect*> listEffect;

	GameTime * gameTime; //unload()??? hay truyền

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

	void ResetResource(); // reset lai resource khi simon mất 1 mạng

	Item * GetNewItem(int id, eID type, float x, float y);

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

