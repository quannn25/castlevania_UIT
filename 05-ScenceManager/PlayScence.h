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
#include "Bat.h"
#include "HolyWater.h"
#include "HolyWaterItem.h"
#include "BrokenBrick.h"
#include "HiddenObject.h"
#include "Crown.h"
#include "Ghost.h"
#include "StopWatch.h"
#include "StopWatchItem.h"
#include "BoomerangItem.h"
#include "Boomerang.h"
#include "AxeItem.h"
#include "Axe.h"
// kiểm tra define_H các file

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

	bool isStopWatch; // unload ?

	vector <LPGAMEOBJECT> listBlackKnight;

	vector <LPGAMEOBJECT> listBat;

	vector <LPGAMEOBJECT> listZombie; // hiện chỉ có zombie
	DWORD TimeZombie; // thoi gian tao zombie vừa rồi /////////////////////////// chua deleteeeeeeeeeeeeeeeeeeeeeeeeeeee
	vector <ZombieZone*> listZombieZone; // list các vùng simon vào thì cho zombie ra tại các vị trí dc parse vào

	vector <LPGAMEOBJECT> listGhost;
	vector <ZombieZone*> listGhostZone; // list các vùng simon vào thì cho zombie ra tại các vị trí dc parse vào

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP(string line);
	void _ParseSection_LOADSIMON0(string line);
	void _ParseSection_LOADSIMON1(string line);
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void LoadResources();
	virtual void Load(int SwitchType);
	virtual void LoadAgain();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void LoadSimonState(int SwitchType);

	void CheckCollision();
	void CheckCollisionWeapon(vector<LPGAMEOBJECT> listObj);
	void CheckCollisionSimonWithItem();
	void CheckCollisionWithEnemy();
	void CheckCollisionSimonWithEnemy(vector<LPGAMEOBJECT> listEnemyX);
	void CheckCollisionWithHiddenObject(vector<LPGAMEOBJECT> listObj);

	bool isOncam(float x1, float y1, float w1, float h1);

	void ResetResource(); // reset lai resource khi simon mất 1 mạng

	Item * GetNewItem(int id, eType type, float x, float y);

	void CreateZombie();
	void CreateGhost();

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

