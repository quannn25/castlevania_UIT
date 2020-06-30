#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/



void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());
	int x_draw;

	if (tokens.size() > 6)
		x_draw = atoi(tokens[6].c_str());
	else
		x_draw = 0;

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex, x_draw);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	int id = atoi(tokens[4].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_SIMON:
		if (MainSimon::GetInstance()->GetSimon() != NULL)  // reset
		{
			DebugOut(L"[ERROR] MARIO object was created before! \n");
			player = MainSimon::GetInstance()->GetSimon();

			player->SetPosition(x, y);
			player->SetId(id);
			if (!player->isOnStair)
			{
				player->isJumping = 1;
				player->vx = 0;
				player->vy = 0;
			}

			// reset các ani_set của các Obj đc truyền qua
			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id); // ani_set Simon
			player->SetAnimationSet(ani_set);

			player->mainWeapon->ReSetAniSetSwitchScene(); // weapon
			if(player->subWeapon != NULL) // subWeapon
				player->subWeapon->ReSetAniSetSwitchScene();


			objects.push_back(player); // add lai
			return;
		}
		obj = new Simon();
		obj->SetType(eType::SIMON);

		MainSimon::GetInstance()->SetSimon((Simon*)obj);
		player = (Simon*)obj;
		if (!player->isOnStair)
		{
			player->isJumping = 1;
			player->vx = 0;
			player->vy = 0;
		}

		break;
	case OBJECT_TYPE_BRICK:
		obj = new CBrick(OBJECT_TYPE_BRICK);
		obj->SetType(eType::BRICK);
		break;
	case OBJECT_TYPE_SPECIALBRICK: // gạch bbox lớn
	{
		if (tokens.size() < 7)
		{
			DebugOut(L"[ERROR] SpecialBrick not found!\n");
			return;
		}
		float w = atof(tokens[5].c_str());
		float h = atof(tokens[6].c_str());

		obj = new CBrick(w, h);
		if (id == 1000)
		{
			obj->SetHealth(2);
		}
		obj->SetType(eType::SPECIALBRICK);
	}
		break;
	case OBJECT_TYPE_MOVINGBRICK: // gạch chuyển động
	{
		if (tokens.size() < 7)
		{
			DebugOut(L"[ERROR] SpecialBrick not found!\n");
			return;
		}
		float left_boundary = atof(tokens[5].c_str());
		float right_boundary = atof(tokens[6].c_str());

		obj = new CBrick(left_boundary, right_boundary, OBJECT_TYPE_MOVINGBRICK);

		obj->SetType(eType::MOVINGBRICK);
	}
	break;
	case OBJECT_TYPE_SPECIALBRICKSMALL: //gạch bbox nhỏ
		obj = new CBrick(OBJECT_TYPE_SPECIALBRICKSMALL);
		obj->SetType(eType::SPECIALBRICKSMALL);
		break;
	case OBJECT_TYPE_TORCH:
		obj = new Torch();
		obj->SetType(eType::TORCH);
		break;
	case OBJECT_TYPE_CANDLE:
		obj = new Candle();
		obj->SetType(eType::CANDLE);
		break;
	case OBJECT_TYPE_STAIR:
		{
		if (tokens.size() < 7)
		{
			DebugOut(L"[ERROR] Stair not found!\n");
			return;
		}
			int t = atof(tokens[5].c_str()); //	1 up 2 down
			int nx1 = atof(tokens[6].c_str());
			obj = new Stair(x, y, t, nx1);
			if (t == 1)
				obj->SetType(eType::STAIR_UP);
			else
				obj->SetType(eType::STAIR_DOWN);
		}
		break;
	case OBJECT_TYPE_BLACKKNIGHT:
	{
		if (tokens.size() < 7)
		{
			DebugOut(L"[ERROR] BlackKinght not found!\n");
			return;
		}

		float left_boundary = atof(tokens[5].c_str());
		float right_boundary = atof(tokens[6].c_str());

		obj = new BlackKnight(left_boundary, right_boundary);
		obj->SetType(eType::BLACKKNIGHT);

		obj->SetPosition(x, y);
		obj->SetId(id);

		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);

		listBlackKnight.push_back((Enemy*)obj);
		return;
	}
		break;
	case OBJECT_TYPE_ZOMBIEZONE:
	{
		if (tokens.size() < 11)
		{
			DebugOut(L"[ERROR] ZombieZone not found!\n");
			return;
		}

		float l1 = atof(tokens[1].c_str()); // bbox zone zombie
		float t1 = atof(tokens[2].c_str());
		float r1 = atof(tokens[3].c_str());
		float b1 = atof(tokens[4].c_str());

		float xZom1 = atof(tokens[5].c_str()); // tọa độ xuất hiện zombie, = -1 => theo camera
		float yZom1 = atof(tokens[6].c_str());
		float xZom2 = atof(tokens[7].c_str());
		float yZom2= atof(tokens[8].c_str());

		int time = atof(tokens[9].c_str());
		int count = atof(tokens[10].c_str());

		ZombieZone *z = new ZombieZone(l1, t1, r1, b1, xZom1, yZom1, xZom2, yZom2, time, count);
		
		listZombieZone.push_back(z);

		return;
	}
		break;

	case OBJECT_TYPE_GHOSTZONE:
	{
		if (tokens.size() < 11)
		{
			DebugOut(L"[ERROR] GhostZone not found!\n");
			return;
		}

		float l1 = atof(tokens[1].c_str()); // bbox zone zombie
		float t1 = atof(tokens[2].c_str());
		float r1 = atof(tokens[3].c_str());
		float b1 = atof(tokens[4].c_str());

		float x1 = atof(tokens[5].c_str()); // tọa độ tạo ghost
		float y1 = atof(tokens[6].c_str());

		float left_boundary = atof(tokens[7].c_str()); // vùng đệm tạo
		float right_boundary = atof(tokens[8].c_str());

		int time = atof(tokens[9].c_str()); //ko quan trọng
		int count = atof(tokens[10].c_str()); //ko quan trọng

		ZombieZone *z = new ZombieZone(l1, t1, r1, b1, x1, y1, left_boundary, right_boundary, time, count);

		listGhostZone.push_back(z);

		return;
	}
	break;
	case OBJECT_TYPE_HUNCHBACKZONE:
	{
		if (tokens.size() < 11)
		{
			DebugOut(L"[ERROR] HunchbackZone not found!\n");
			return;
		}

		float l1 = atof(tokens[1].c_str()); // bbox zone zombie
		float t1 = atof(tokens[2].c_str());
		float r1 = atof(tokens[3].c_str());
		float b1 = atof(tokens[4].c_str());

		float x1 = atof(tokens[5].c_str()); // tọa độ tạo ghost
		float y1 = atof(tokens[6].c_str());

		float left_boundary = atof(tokens[7].c_str()); // vùng đệm tạo
		float right_boundary = atof(tokens[8].c_str());

		int time = atof(tokens[9].c_str()); //ko quan trọng
		int count = atof(tokens[10].c_str()); //ko quan trọng

		ZombieZone *z = new ZombieZone(l1, t1, r1, b1, x1, y1, left_boundary, right_boundary, time, count);

		listHunchbackZone.push_back(z);

		return;
	}
	break;
	case OBJECT_TYPE_SKELETONZONE:
	{
		if (tokens.size() < 11)
		{
			DebugOut(L"[ERROR] SkeletonZone not found!\n");
			return;
		}

		float l1 = atof(tokens[1].c_str()); // bbox zone zombie
		float t1 = atof(tokens[2].c_str());
		float r1 = atof(tokens[3].c_str());
		float b1 = atof(tokens[4].c_str());

		float x1 = atof(tokens[5].c_str()); // tọa độ tạo ghost
		float y1 = atof(tokens[6].c_str());

		float left_boundary = atof(tokens[7].c_str()); // vùng đệm tạo
		float right_boundary = atof(tokens[8].c_str());

		int time = atof(tokens[9].c_str()); //ko quan trọng
		int count = atof(tokens[10].c_str()); //ko quan trọng

		ZombieZone *z = new ZombieZone(l1, t1, r1, b1, x1, y1, left_boundary, right_boundary, time, count);

		listSkeletonZone.push_back(z);

		return;
	}
	break;
	case OBJECT_TYPE_BAT:
	{
		obj = new Bat(x, y);
		obj->SetType(eType::BAT);

		obj->SetId(id);

		listBat.push_back(obj);
		return;
	}
		break;
	case OBJECT_TYPE_HIDDENOBJECT:
	{
		float xItem = atoi(tokens[5].c_str());
		float yItem = atoi(tokens[6].c_str());

		obj = new HiddenObject(x, y, xItem, yItem);
		obj->SetType(eType::HIDDENOBJECT);
	}
	break;
	case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[5].c_str());
			float b = atof(tokens[6].c_str());
			int scene_id = atoi(tokens[7].c_str());
			int switchType = atoi(tokens[8].c_str()); // 1 chuyển sang scene sau, 0 quay về scene trước

			obj = new CPortal(x, y, r, b, scene_id, switchType);
			obj->SetType(eType::PORTAL);
		}
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	obj->SetId(id);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CPlayScene::_ParseSection_TILEMAP(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 2) return;

	wstring texPath = ToWSTR(tokens[0]);
	wstring txtPath = ToWSTR(tokens[1]);

	tileMap = new Map(texPath.c_str(), txtPath.c_str());
	tileMap->load();
	DebugOut(L"[INFO] Map resources from : %s \n", texPath.c_str());
	DebugOut(L"[INFO] Map resources from : %s \n", txtPath.c_str());
}

void CPlayScene::_ParseSection_LOADSIMON0(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	float xSimon = atoi(tokens[0].c_str());
	float ySimon = atoi(tokens[1].c_str());

	MainSimon::GetInstance()->GetSimon()->SetPosition(xSimon, ySimon); // == player
}

void CPlayScene::_ParseSection_LOADSIMON1(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	float xSimon = atoi(tokens[0].c_str());
	float ySimon = atoi(tokens[1].c_str());

	MainSimon::GetInstance()->GetSimon()->SetPosition(xSimon, ySimon); // == player

	DebugOut(L"x = %f, y = %f\n", MainSimon::GetInstance()->GetSimon()->GetX(), MainSimon::GetInstance()->GetSimon()->GetY());
}

void CPlayScene::Load(int SwitchType)
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;	
	int flag = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { 
			section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[TILEMAP]") {
			section = SCENE_SECTION_TILEMAP; continue; }
		if (line[0] == '[') 
		{
			section = SCENE_SECTION_UNKNOWN; continue;
		}	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_TILEMAP: _ParseSection_TILEMAP(line); break;
		}
	}

	f.close();

	LoadSimonState(SwitchType); // hàm load simon khi chuyển đến scene dạng 0 hoặc 1

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	LoadResources();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::LoadSimonState(int SwitchType)
{
	if (SwitchType == 1)
	{
		ifstream f;
		f.open(sceneFilePath);

		// current resource section flag
		int section = SCENE_SECTION_UNKNOWN;

		char str[MAX_SCENE_LINE];
		while (f.getline(str, MAX_SCENE_LINE))
		{
			string line(str);

			if (line[0] == '#') continue;	// skip comment lines	

			if (line == "[LOADSIMON_1]") {
				section = SCENE_SECTION_LOADSIMON_1; continue;
			}
			if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

			//
			// data section
			//
			switch (section)
			{
			case SCENE_SECTION_LOADSIMON_1: _ParseSection_LOADSIMON1(line); break; // hàm này đọc thông số từ file rồi gán cho simon static hoặc player
			}
		}

		f.close();
	}
	else
	{
		ifstream f;
		f.open(sceneFilePath);

		// current resource section flag
		int section = SCENE_SECTION_UNKNOWN;

		char str[MAX_SCENE_LINE];
		while (f.getline(str, MAX_SCENE_LINE))
		{
			string line(str);

			if (line[0] == '#') continue;	// skip comment lines	

			if (line == "[LOADSIMON_0]") {
				section = SCENE_SECTION_LOADSIMON_0; continue;
			}

			if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

			//
			// data section
			//
			switch (section)
			{
			case SCENE_SECTION_LOADSIMON_0: _ParseSection_LOADSIMON0(line); break; // hàm này đọc thông số từ file rồi gán cho simon static hoặc player
			}
		}

		f.close();
	}

	// xử lý chuyển Scene và Simon đang trên thang
	if (MainSimon::GetInstance()->GetSimon()->isOnStair)
	{
		MainSimon::GetInstance()->GetSimon()->isWalkingOnStair = 0;
		MainSimon::GetInstance()->GetSimon()->vx = 0;
		MainSimon::GetInstance()->GetSimon()->vy = 0;
		MainSimon::GetInstance()->GetSimon()->isWalking = false;
		MainSimon::GetInstance()->GetSimon()->walkHeight = 0;
	}

}

void CPlayScene::LoadResources()
{
	Camera::GetInstance()->SetPosition(0.0f, 0.0f);
	grid = new Grid(objects);
	boardGame = new Board(0, 0);

	gameTime = GameTime::GetInstance();
	gameTime->SetTime(0);

	player->SetPositionBackup(SIMON_POSITION_DEFAULT);

	TimeZombie = 0;
	isStopWatch = false;

	listItem.clear();
	listEffect.clear();
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	//DebugOut(L"[Grid] Object = %d\n", objects.size());
	//DebugOut(L"x = %f, y = %f\n", MainSimon::GetInstance()->GetSimon()->GetX(), MainSimon::GetInstance()->GetSimon()->GetY());

	if (gameTime->GetTime() >= GAMETIME_SCENE_1 || player->GetHealth() <= 0)
	{
		if (player->GetLive() == 0) // xử lý...
			return;
		bool result = player->LoseLife();
		if (result == true) // còn mạng để chơi tiếp
		{
			ResetResource(); // reset lại game
		}
		return;
	}
	else
		gameTime->Update();


	if (player->GetFreeze() == true)
	{
		player->UpdateFreeze(dt);
		if (player->GetFreeze() == true)
			return;
	}

	if (player->subWeapon != NULL && player->subWeapon->GetType() == eType::STOPWATCH)
	{
		if (player->subWeapon->GetFinish() == false)
		{
			isStopWatch = true;
		}
		else
			isStopWatch = false;
	}
	else
		isStopWatch = false;

	
	grid->ResetOnCam(objects); // set lai trang thai onCam
	
	grid->GetListObject(coObjects); // lay listObj onCam
	//DebugOut(L"[Grid] Object on Camera = %d\n", coObjects.size());

	player->Update(dt, &coObjects);

	for (size_t i = 0; i < coObjects.size(); i++)
	{
		coObjects[i]->Update(dt, &coObjects);
	}

	for (size_t i = 0; i < listItem.size(); i++)
	{
		if (listItem[i]->GetFinish() == false)
			listItem[i]->Update(dt, &coObjects);
	}

	for (int i = 0; i < listEffect.size(); i++)
	{
		if (listEffect[i]->GetFinish() == false)
			listEffect[i]->Update(dt);
	}

	// update enemy

	if(isStopWatch == false)
		updateEnemy(dt);

	CreateZombie();

	CreateGhost();

	CreateHunchback();

	CreateSkeleton();

	// check collision
	CheckCollision();

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	int w = Camera::GetInstance()->GetScreenWidth();
	int h = Camera::GetInstance()->GetScreenHeight();

	cx -= w / 2;
	cy -= h / 2;

	Camera::GetInstance()->SetPosition(cx, 0.0f);
	Camera::GetInstance()->Update();
}

void CPlayScene::Render()
{
	tileMap->DrawMap(Camera::GetInstance());

	// coObj=================
	for (int i = 0; i < coObjects.size(); i++)
	{
		coObjects[i]->Render();
	}

	// ListItem=================
	for (int i = 0; i < listItem.size(); i++)
	{
		if (listItem[i]->GetFinish() == false)
			listItem[i]->Render();
	}

	// listZombie==================
	for (UINT i = 0; i < listZombie.size(); i++)
	{
		if (listZombie[i]->GetHealth() > 0)
			listZombie[i]->Render();
	}

	for (UINT i = 0; i < listZombieZone.size(); i++)
	{
		listZombieZone[i]->RenderBoundingBox();
	}

	// listGhost=====================
	for (UINT i = 0; i < listGhost.size(); i++)
	{
		if (listGhost[i]->GetHealth() > 0)
			listGhost[i]->Render();
	}

	for (UINT i = 0; i < listGhostZone.size(); i++)
	{
		//listGhostZone[i]->RenderBoundingBox();
	}

	// listHunchback=====================
	for (UINT i = 0; i < listHunchback.size(); i++)
	{
		if (listHunchback[i]->GetHealth() > 0)
			listHunchback[i]->Render();
	}

	for (UINT i = 0; i < listHunchbackZone.size(); i++)
	{
		//listHunchbackZone[i]->RenderBoundingBox();
	}


	// listSkeleton==========================
	for (UINT i = 0; i < listSkeleton.size(); i++)
	{
		if (listSkeleton[i]->GetHealth() > 0)
			listSkeleton[i]->Render();
	}

	for (UINT i = 0; i < listSkeletonZone.size(); i++)
	{
		//listSkeletonZone[i]->RenderBoundingBox();
	}

	// listBlackKnight===============================
	for (UINT i = 0; i < listBlackKnight.size(); i++)
	{
		float l, t, r, b;
		float widthEnemy, heightEnemy;
		listBlackKnight[i]->GetBoundingBox(l, t, r, b);
		widthEnemy = b - t;
		heightEnemy = r - l;

		if (listBlackKnight[i]->GetHealth() > 0)
		{
			if (isOncam(listBlackKnight[i]->GetX(), listBlackKnight[i]->GetY(), widthEnemy, heightEnemy) == true)
			{
				listBlackKnight[i]->Render();
			}
		}
			
	}


	// listBat==================================
	for (UINT i = 0; i < listBat.size(); i++)
	{
		float l, t, r, b;
		float widthEnemy, heightEnemy;
		listBat[i]->GetBoundingBox(l, t, r, b);
		widthEnemy = b - t;
		heightEnemy = r - l;

		if (listBat[i]->GetHealth() > 0)
		{
			if (isOncam(listBat[i]->GetX(), listBat[i]->GetY(), widthEnemy, heightEnemy) == true)
			{
				listBat[i]->Render();
			}
		}
	}


	// listEffect=========================
	for (int i = 0; i < listEffect.size(); i++)
	{
		if (listEffect[i]->GetFinish() == false)
			listEffect[i]->Render();
	}

	boardGame->Render(player, CGame::GetInstance()->getCurrentSceneId(), player->subWeapon, GAMETIME_SCENE_1 - gameTime->GetTime());

	player->Render();

}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<Simon*>(objects[i]))
			continue;
		delete objects[i];
	}

	objects.clear();

	coObjects.clear();

	//player = NULL;

	delete tileMap;
	tileMap = NULL;

	delete grid;
	grid = NULL;

	delete boardGame;
	boardGame = NULL;

	for (int i = 0; i < listItem.size(); i++)
		delete listItem[i];

	listItem.clear();

	for (int i = 0; i < listEffect.size(); i++)
		delete listEffect[i];

	listEffect.clear();

	// Zombie===================
	for (int i = 0; i < listZombie.size(); i++)
		delete listZombie[i];

	listZombie.clear();

	for (int i = 0; i < listZombieZone.size(); i++)
		delete listZombieZone[i];

	listZombieZone.clear();


	// Ghost=======================
	for (int i = 0; i < listGhost.size(); i++)
		delete listGhost[i];

	listGhost.clear();

	for (int i = 0; i < listGhostZone.size(); i++)
		delete listGhostZone[i];

	listGhostZone.clear();

	// Hunchback=======================
	for (int i = 0; i < listHunchback.size(); i++)
		delete listHunchback[i];

	listHunchback.clear();

	for (int i = 0; i < listHunchbackZone.size(); i++)
		delete listHunchbackZone[i];

	listHunchbackZone.clear();

	// Skeleton==========================
	for (int i = 0; i < listSkeleton.size(); i++)
		delete listSkeleton[i];

	listSkeleton.clear();

	for (int i = 0; i < listSkeletonZone.size(); i++)
		delete listSkeletonZone[i];

	listSkeletonZone.clear();

	//BlackKnight========================
	for (int i = 0; i < listBlackKnight.size(); i++)
		delete listBlackKnight[i];

	listBlackKnight.clear();

	// Bat===============================
	for (int i = 0; i < listBat.size(); i++)
		delete listBat[i];

	listBat.clear();
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)// tạo is jumping, sitting... quản lý state
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Simon *simon = ((CPlayScene*)scence)->player;

	if (simon->isHurting)
		return;

	if (simon->GetFreeze() == true) // Đang bóng băng thì không quan tâm phím
	{
		return;
	}

	if (simon->isAutoGoX == true)
		return;


	switch (KeyCode)
	{
	case DIK_SPACE:
		if(simon->isOnStair == false)
			simon->SetState(SIMON_STATE_JUMPING);
		break;
	case DIK_A: // reset
		simon->SetState(SIMON_STATE_IDLE);
		simon->SetPosition(50.0f, 0.0f);
		simon->SetSpeed(0, 0);
		simon->isWalking = 0;
		simon->isOnStair = 0;
		simon->isWalkingOnStair = 0;
		simon->isAutoGoX = 0;
		simon->isOnMovingBrick = 0;
		break;
	case DIK_X:
		simon->Attack(simon->mainWeapon);
		break;
	case DIK_Z:
		if (simon->subWeapon != NULL && simon->subWeapon->GetFinish() == true)
		{
			bool useSubWeapon = false;

			switch (simon->subWeapon->GetType())
			{
			case eType::STOPWATCH:
				if (simon->GetHeartCollected() >= 5)
				{
					simon->SetHeartCollected(simon->GetHeartCollected() - 5);
					useSubWeapon = true;
				}
				break;

			default:
				if (simon->GetHeartCollected() > 0)
				{
					simon->SetHeartCollected(simon->GetHeartCollected() - 1);
					useSubWeapon = true;
				}
				break;

			}

			if (useSubWeapon)
				simon->Attack(simon->subWeapon);

		}
		break;
	case DIK_I:
		//((CPlayScene*)scence)->listItem.push_back(new DaggerItem(simon->GetX(), simon->GetY()));
		((CPlayScene*)scence)->listItem.push_back(new StopWatchItem(simon->GetX(), simon->GetY()));
		break;
	case DIK_1:
		CGame::GetInstance()->SwitchScene(1, 1);
		break;
	case DIK_2:
		CGame::GetInstance()->SwitchScene(2, 1);
		break;
	case DIK_3:
		CGame::GetInstance()->SwitchScene(3, 1);
		break;
	case DIK_4:
		CGame::GetInstance()->SwitchScene(4, 1);
		break;
	case DIK_5:
		CGame::GetInstance()->SwitchScene(5, 1);
		break;
	case DIK_6:
		CGame::GetInstance()->SwitchScene(6, 1);
		break;
	}
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	Simon *simon = ((CPlayScene*)scence)->player;
	vector<LPGAMEOBJECT> _coObjects = ((CPlayScene*)scence)->coObjects;

	if (simon->isHurting)
		return;

	if (simon->GetFreeze() == true) // dang bóng băng
	{
		return;
	}

	// disable control key when Mario die 
	if (simon->GetState() == SIMON_STATE_DIE) return;

	if (simon->isAutoGoX == true)
		return;

	if (game->IsKeyDown(DIK_UP) && game->IsKeyDown(DIK_X) && simon->isWalkingOnStair == 0 && !simon->isAttacking && simon->subWeapon != NULL && simon->subWeapon->GetFinish() == true)
	{
		bool useSubWeapon = false;

			switch (simon->subWeapon->GetType())
			{
			case eType::STOPWATCH:
				if (simon->GetHeartCollected() >= 5)
				{
					simon->SetHeartCollected(simon->GetHeartCollected() - 5);
					useSubWeapon = true;
				}
				break;

			default:
				if (simon->GetHeartCollected() > 0)
				{
					simon->SetHeartCollected(simon->GetHeartCollected() - 1);
					useSubWeapon = true;
				}
				break;

			}

			if (useSubWeapon)
				simon->Attack(simon->subWeapon);
	}
	else
	{
		if (simon->isJumping == false && simon->isAttacking == false)
		{
			if (game->IsKeyDown(DIK_UP)) // if (UP) else (if (DOWN) else (ko làm gì))
			{
				if (simon->isOnStair == false) // nếu đang ko trên thang
				{
					for (UINT i = 0; i < _coObjects.size(); i++)
					{
						if (_coObjects[i]->GetType() == eType::STAIR_UP && simon->isCollitionObjectWithObject(_coObjects[i]) == true)
						{
							Stair* obj = dynamic_cast<Stair*>(_coObjects[i]);
							simon->NxStair = obj->GetNx();

							simon->SetNx(simon->NxStair); // hướng simon theo hướng thang
							simon->ny = -1; // hướng lên
							simon->isOnStair = true;
							simon->walkHeight = 0;

							if (simon->GetX() < obj->GetX())
							{
								simon->SetAutoGoX(1, obj->GetNx(), obj->GetX(), SIMON_WALKING_SPEED);
							}
							else
								simon->SetAutoGoX(-1, obj->GetNx(), obj->GetX(), SIMON_WALKING_SPEED);

							return;
						}
					}
				}
				else // nếu đang trên thang rồi thì bật xử lý, walking = 1
				{
					simon->isWalking = true;

					simon->ny = -1;

					simon->isWalkingOnStair = 1;

					simon->SetNx(simon->NxStair);

					simon->SetSpeed(simon->GetNx()* SIMON_SPEED_ONSTAIR, -1 * SIMON_SPEED_ONSTAIR);
					return;
				}

			}
			else // DOWN
			{
				if (game->IsKeyDown(DIK_DOWN))
				{
					if (simon->isOnStair == false) // chưa trên stair
					{
						for (UINT i = 0; i < _coObjects.size(); i++)
						{
							if (_coObjects[i]->GetType() == eType::STAIR_DOWN)
							{
								if (simon->isCollitionObjectWithObject(_coObjects[i]))
								{
									Stair* obj = dynamic_cast<Stair*>(_coObjects[i]);
									simon->NxStair = obj->GetNx(); // lưu hướng của stair
									simon->ny = 1;// hướng đi xuống
									simon->SetNx(simon->NxStair * -1);// set hướng simon

									simon->isOnStair = true; // set trạng thái stair
									simon->walkHeight = 0;

									if (simon->GetX() < obj->GetX())
									{
										simon->SetAutoGoX(1, -obj->GetNx(), obj->GetX(), SIMON_WALKING_SPEED);
									}
									else
									{
										simon->SetAutoGoX(-1, -obj->GetNx(), obj->GetX(), SIMON_WALKING_SPEED);
									}

									return;
								}
							}
						}

						simon->SetState(SIMON_STATE_SITTING);
						if (game->IsKeyDown(DIK_RIGHT))
							simon->SetState(SIMON_STATE_RIGHT);

						if (game->IsKeyDown(DIK_LEFT))
							simon->SetState(SIMON_STATE_LEFT);
						return;

					}
					else // đã trên stair
					{
						if (simon->isWalkingOnStair == 0 || simon->isWalkingOnStair == 3) // kết thúc xử lí trước đó
						{
							simon->isWalking = true;
							simon->isWalkingOnStair = 1;
							simon->ny = 1;// hướng đi xuống
							simon->SetNx(simon->NxStair*-1);// đi xuống thì ngược so với stair
							simon->SetSpeed(simon->GetNx()* SIMON_SPEED_ONSTAIR, SIMON_SPEED_ONSTAIR);
						}
					}

				}
				else
				{
					simon->SetState(SIMON_STATE_STOP);
				}

			}
		}
	}

	if (simon->isJumping && simon->isWalking)
	{
		if (simon->GetNx() > 0) // tiến vào sát gạch cao nhảy ko dc // mở ra dính lỗi nhảy lên tường
			simon->SetState(SIMON_STATE_WALKING_RIGHT);
		else
			simon->SetState(SIMON_STATE_WALKING_LEFT);
		return;
	}
	

	if (simon->isJumping)
		return;

	if(game->IsKeyDown(DIK_RIGHT))
	{
		if (simon->isAttacking)
		{
			float vx, vy;
			simon->GetSpeed(vx, vy);
			simon->SetSpeed(0, vy);
			return;
		}
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (simon->isAttacking)
		{
			float vx, vy;
			simon->GetSpeed(vx, vy);
			simon->SetSpeed(0, vy);
			return;
		}
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	}
	else
	{
		simon->SetState(SIMON_STATE_STOP);
	}
}


void CPlayScene::CheckCollision()
{
	CheckCollisionWeapon(coObjects); // kt va chạm vũ khí với các object nền

	CheckCollisionSimonWithItem();

	CheckCollisionWithHiddenObject(coObjects);

	CheckCollisionWithEnemy(); // kt vũ khí cới enemy và simon với enemy
}

void CPlayScene::CheckCollisionWithHiddenObject(vector<LPGAMEOBJECT> listObj)
{
	for (UINT i = 0; i < listObj.size(); i++)
	{
		if (listObj[i]->GetType() == eType::HIDDENOBJECT)
		{
			if (player->isCollitionObjectWithObject(listObj[i]))
			{
				HiddenObject *h = dynamic_cast<HiddenObject*>(listObj[i]);

				if (h->getIsActive() == false) // nếu chưa hoạt động
				{
					listItem.push_back(GetNewItem(h->GetId(), h->GetType(), h->getXItem(), h->getYItem()));

					h->setIsActive(true); // bật đã hoạt động rồi
				}
			}
		}
	}
}

void CPlayScene::CheckCollisionWeapon(vector<LPGAMEOBJECT> listObj)
{
	// main weapon
	if (player->mainWeapon->GetFinish() == false) // Vũ khí đang hoạt động
	{
		for (UINT i = 0; i < listObj.size(); i++)
		{
			if (player->mainWeapon->isCollision(listObj[i]) == true) // có va chạm với obj (heal > 0)
			{
				switch (listObj[i]->GetType())
				{
				case eType::CANDLE:
				{
					CGameObject *gameObj = dynamic_cast<CGameObject*>(listObj[i]);
					gameObj->beAttacked(1);

					listEffect.push_back(new Hit(gameObj->GetX() + 14, gameObj->GetY() + 14)); // hiệu ứng
					listEffect.push_back(new Fire(gameObj->GetX() - 5, gameObj->GetY() + 8)); // hiệu ứng
					listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));

					break;
				}
				case eType::TORCH:
				{
					CGameObject *gameObj = dynamic_cast<CGameObject*>(listObj[i]);
					gameObj->beAttacked(1);

					listEffect.push_back(new Hit(gameObj->GetX() + 14, gameObj->GetY() + 14)); // hiệu ứng
					listEffect.push_back(new Fire(gameObj->GetX() - 5, gameObj->GetY() + 8)); // hiệu ứng
					listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));

					break;
				}

				case eType::ZOMBIE:
				{
					CGameObject *gameObj = dynamic_cast<CGameObject*>(listObj[i]);
					gameObj->beAttacked(1);

					listEffect.push_back(new Hit(gameObj->GetX() + 14, gameObj->GetY() + 14)); // hiệu ứng
					
					// if health <= 0 moi rot item
					//if (rand() % 2 == 1) // tỉ lệ 50%
					//{
					//	listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
					//}
					listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
					break;
				}
				case eType::BLACKKNIGHT:
				{
					BlackKnight *gameObj = dynamic_cast<BlackKnight*>(listObj[i]);
					if (!gameObj->isHurt)
					{
						gameObj->beAttacked(1);
						gameObj->isHurt = true;

						listEffect.push_back(new Hit(gameObj->GetX() + 14, gameObj->GetY() + 14)); // hiệu ứng

						//if (rand() % 2 == 1) // tỉ lệ 50%
						//{
						//	listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
						//}
						listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
					}
					break;
				}
				case eType::BAT:
				{
					CGameObject *gameObj = dynamic_cast<CGameObject*>(listObj[i]);
					gameObj->beAttacked(1);

					listEffect.push_back(new Hit(gameObj->GetX() + 10, gameObj->GetY() + 10)); // hiệu ứng

					//if (rand() % 2 == 1) // tỉ lệ 50%
					//{
					//	listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
					//}
					listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));

					break;
				}
				case eType::GHOST:
				{
					CGameObject *gameObj = dynamic_cast<CGameObject*>(listObj[i]);
					gameObj->beAttacked(1);

					listEffect.push_back(new Hit(gameObj->GetX() + 10, gameObj->GetY() + 10)); // hiệu ứng

					//if (rand() % 2 == 1) // tỉ lệ 50%
					//{
					//	listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
					//}
					listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
					break;
				}
				case eType::HUNCHBACK:
				{
					CGameObject *gameObj = dynamic_cast<CGameObject*>(listObj[i]);
					gameObj->beAttacked(1);

					listEffect.push_back(new Hit(gameObj->GetX() + 10, gameObj->GetY() + 10)); // hiệu ứng
					listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
					break;
				}
				case eType::SKELETON:
				{
					CGameObject *gameObj = dynamic_cast<CGameObject*>(listObj[i]);
					gameObj->beAttacked(1);

					listEffect.push_back(new Hit(gameObj->GetX() + 10, gameObj->GetY() + 10)); // hiệu ứng
					listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
					break;
				}

				default:
					break;
				}
			}
		}
	}

	// sub
	if (player->subWeapon != NULL && player->subWeapon->GetFinish() == false)
	{
		for (UINT i = 0; i < listObj.size(); i++)
		{
			if (player->subWeapon->isCollision(listObj[i]) == true) // có va chạm với obj (heal > 0)
			{
				bool isCollisonWithEnemy = false;

				switch (listObj[i]->GetType())
				{
				case eType::CANDLE:
				{
					CGameObject *gameObj = dynamic_cast<CGameObject*>(listObj[i]);
					gameObj->beAttacked(1);

					isCollisonWithEnemy = true;

					listEffect.push_back(new Hit(gameObj->GetX() + 14, gameObj->GetY() + 14)); // hiệu ứng
					listEffect.push_back(new Fire(gameObj->GetX() - 5, gameObj->GetY() + 8)); // hiệu ứng
					listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));

					break;
				}
				case eType::TORCH:
				{
					CGameObject *gameObj = dynamic_cast<CGameObject*>(listObj[i]);
					gameObj->beAttacked(1);

					isCollisonWithEnemy = true;

					listEffect.push_back(new Hit(gameObj->GetX() + 14, gameObj->GetY() + 14)); // hiệu ứng
					listEffect.push_back(new Fire(gameObj->GetX() - 5, gameObj->GetY() + 8)); // hiệu ứng
					listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));

					break;
				}

				case eType::ZOMBIE:
				{
					CGameObject *gameObj = dynamic_cast<CGameObject*>(listObj[i]);
					gameObj->beAttacked(1);

					player->SetScore(player->GetScore() + 100);

					isCollisonWithEnemy = true;

					listEffect.push_back(new Hit(gameObj->GetX() + 14, gameObj->GetY() + 14)); // hiệu ứng

					if (rand() % 2 == 1) // tỉ lệ 50%
					{
						listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
					}

					break;
				}
				case eType::BLACKKNIGHT:
				{
					BlackKnight *gameObj = dynamic_cast<BlackKnight*>(listObj[i]);
					if (!gameObj->isHurt)
					{
						gameObj->beAttacked(1);
						gameObj->isHurt = true;

						player->SetScore(player->GetScore() + 100);

						isCollisonWithEnemy = true;

						listEffect.push_back(new Hit(gameObj->GetX() + 14, gameObj->GetY() + 14)); // hiệu ứng

						if (rand() % 2 == 1) // tỉ lệ 50%
						{
							listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
						}
					}

					break;
				}
				case eType::BAT:
				{
					CGameObject *gameObj = dynamic_cast<CGameObject*>(listObj[i]);
					gameObj->beAttacked(1);

					player->SetScore(player->GetScore() + 100);

					isCollisonWithEnemy = true;

					listEffect.push_back(new Hit(gameObj->GetX() + 10, gameObj->GetY() + 10)); // hiệu ứng

					if (rand() % 2 == 1) // tỉ lệ 50%
					{
						listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
					}

					break;
				}

				case eType::GHOST:
				{
					CGameObject *gameObj = dynamic_cast<CGameObject*>(listObj[i]);
					gameObj->beAttacked(1);

					player->SetScore(player->GetScore() + 100);

					isCollisonWithEnemy = true;

					listEffect.push_back(new Hit(gameObj->GetX() + 10, gameObj->GetY() + 10)); // hiệu ứng

					if (rand() % 2 == 1) // tỉ lệ 50%
					{
						listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
					}

					break;
				}
				case eType::HUNCHBACK:
				{
					CGameObject *gameObj = dynamic_cast<CGameObject*>(listObj[i]);
					gameObj->beAttacked(1);

					player->SetScore(player->GetScore() + 100);

					isCollisonWithEnemy = true;

					listEffect.push_back(new Hit(gameObj->GetX() + 10, gameObj->GetY() + 10)); // hiệu ứng

					if (rand() % 2 == 1) // tỉ lệ 50%
					{
						listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
					}

					break;
				}
				case eType::SKELETON:
				{
					CGameObject *gameObj = dynamic_cast<CGameObject*>(listObj[i]);
					gameObj->beAttacked(1);

					player->SetScore(player->GetScore() + 100);

					isCollisonWithEnemy = true;

					listEffect.push_back(new Hit(gameObj->GetX() + 10, gameObj->GetY() + 10)); // hiệu ứng

					if (rand() % 2 == 1) // tỉ lệ 50%
					{
						listItem.push_back(GetNewItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
					}

					break;
				}

				default:
					break;
				}

				if (isCollisonWithEnemy)
				{
					switch (player->subWeapon->GetType())
					{
					case eType::HOLYWATER:
					{
						break;
					}

					case eType::DAGGER:
					{

						player->subWeapon->SetFinish(true); // hủy cây kiếm
						break;
					}
					default:
						DebugOut(L"[ERROR] there is no case apply!\n");
						break;
					}
				}

			}
		}
	}

	// main weapon with brokenBrick
	if (player->mainWeapon->GetFinish() == false)
	{
		for (UINT i = 0; i < listObj.size(); i++)
		{
			if (listObj[i]->GetType() == eType::SPECIALBRICK)
			{
				CGameObject * gameObject = dynamic_cast<CGameObject*>(listObj[i]);
				if (gameObject->GetHealth() > 0)
				{
					switch (gameObject->GetId())
					{
					case 999:
					{
						if (player->mainWeapon->isCollision(listObj[i]) == true)
						{
							gameObject->beAttacked(1);
							if (gameObject->GetHealth() <= 0)
							{
								//listItem.push_back(GetNewItem(gameObject->GetId(), gameObject->GetType(), gameObject->GetX(), gameObject->GetY()));
								listEffect.push_back(new Hit((int)gameObject->GetX() + 14, (int)gameObject->GetY() + 14));
								listEffect.push_back(new BrokenBrick((int)gameObject->GetX() + 14, (int)gameObject->GetY() + 14, 1)); // hiệu ứng BrokenBrick
								listEffect.push_back(new BrokenBrick((int)gameObject->GetX() + 14, (int)gameObject->GetY() + 14, 2)); // hiệu ứng BrokenBrick
								listEffect.push_back(new BrokenBrick((int)gameObject->GetX() + 14, (int)gameObject->GetY() + 14, 3)); // hiệu ứng BrokenBrick
								listEffect.push_back(new BrokenBrick((int)gameObject->GetX() + 14, (int)gameObject->GetY() + 14, 4)); // hiệu ứng BrokenBrick 
							}

						}
						break;
					}
					case 1000:
					{
						if (player->mainWeapon->isCollision(listObj[i]) == true)
						{
							gameObject->beAttacked(1);
							if (gameObject->GetHealth() <= 0)
							{
								listItem.push_back(GetNewItem(gameObject->GetId(), gameObject->GetType(), gameObject->GetX(), gameObject->GetY()));
								listEffect.push_back(new Hit((int)gameObject->GetX() + 14, (int)gameObject->GetY() + 14));
								listEffect.push_back(new BrokenBrick((int)gameObject->GetX() + 14, (int)gameObject->GetY() + 14, 1)); // hiệu ứng BrokenBrick
								listEffect.push_back(new BrokenBrick((int)gameObject->GetX() + 14, (int)gameObject->GetY() + 14, 2)); // hiệu ứng BrokenBrick
								listEffect.push_back(new BrokenBrick((int)gameObject->GetX() + 14, (int)gameObject->GetY() + 14, 3)); // hiệu ứng BrokenBrick
								listEffect.push_back(new BrokenBrick((int)gameObject->GetX() + 14, (int)gameObject->GetY() + 14, 4)); // hiệu ứng BrokenBrick 
							}

						}
						break;
					}
					default:
						break;
					}
				}



			}
		}
	}
}

void CPlayScene::CheckCollisionSimonWithItem()
{
	for (UINT i = 0; i < listItem.size(); i++)
	{
		if (listItem[i]->GetFinish() == false && listItem[i]->isWaitingDisplay() == false) // chưa kết thúc và False => "KHÔNG" đang chờ để hiển thị
		{
			if (player->isCollisionWithItem(listItem[i]) == true) // có va chạm
			{
				switch (listItem[i]->GetType())
				{
				case eType::LARGEHEART:
				{
					player->SetHeartCollected(player->GetHeartCollected() + 5);
					listItem[i]->SetFinish(true);
					break;
				}
				case eType::SMALLHEART:
				{
					player->SetHeartCollected(player->GetHeartCollected() + 1);
					listItem[i]->SetFinish(true);
					break;
				}

				case eType::UPGRADEMORNINGSTAR:
				{
					MorningStar * objMorningStar = dynamic_cast<MorningStar*>(player->mainWeapon);
					objMorningStar->UpgradeLevel(); // Nâng cấp vũ khí roi
					listItem[i]->SetFinish(true);
					player->SetFreeze(true);
					break;
				}
				case eType::DAGGERITEM:
				{
					if (player->subWeapon)
					{
						delete player->subWeapon;
						player->subWeapon = NULL;
					}
					player->subWeapon = new Dagger();
					listItem[i]->SetFinish(true);
					break;
				}
				case eType::MONNEY:
				{
					listItem[i]->SetFinish(true);
					player->SetScore(player->GetScore() + 1000);
					break;
				}
				case eType::CROWN:
				{
					listItem[i]->SetFinish(true);
					player->SetScore(player->GetScore() + 1000);
					break;
				}
				case eType::HOLYWATERITEM:
				{
					if (player->subWeapon)
					{
						delete player->subWeapon;
						player->subWeapon = NULL;
					}
					player->subWeapon = new HolyWater();
					listItem[i]->SetFinish(true);
					break;
				}
				case eType::STOPWATCHITEM:
				{
					if (player->subWeapon)
					{
						delete player->subWeapon;
						player->subWeapon = NULL;
					}
					player->subWeapon = new StopWatch();
					listItem[i]->SetFinish(true);
					break;
				}

				case eType::BOOMERANGITEM:
				{
					if (player->subWeapon)
					{
						delete player->subWeapon;
						player->subWeapon = NULL;
					}
					player->subWeapon = new Boomerang();
					listItem[i]->SetFinish(true);
					break;
				}
				case eType::AXEITEM:
				{
					if (player->subWeapon)
					{
						delete player->subWeapon;
						player->subWeapon = NULL;
					}
					player->subWeapon = new Axe();
					listItem[i]->SetFinish(true);
					break;
				}
				default:
					DebugOut(L"[CheckCollisionSimonWithItem] Loi nhat item\n");
					break;
				}

			}
		}
	}
}

void CPlayScene::CheckCollisionWithEnemy()
{
	// kiểm tra va chạm vũ khí với enemy
	CheckCollisionWeapon(listZombie);
	CheckCollisionWeapon(listBlackKnight);
	CheckCollisionWeapon(listBat);
	CheckCollisionWeapon(listGhost);
	CheckCollisionWeapon(listHunchback);
	CheckCollisionWeapon(listSkeleton);

	// kiểm tra va chạm simon với enemy
	CheckCollisionSimonWithEnemy(listZombie);
	CheckCollisionSimonWithEnemy(listBlackKnight);
	CheckCollisionSimonWithEnemy(listBat);
	CheckCollisionSimonWithEnemy(listGhost);
	CheckCollisionSimonWithEnemy(listHunchback);
	CheckCollisionSimonWithEnemy(listSkeleton);
}

void CPlayScene::CheckCollisionSimonWithEnemy(vector<LPGAMEOBJECT> listEnemyX)
{

	if (GetTickCount() - player->getUntouchable_start() > SIMON_UNTOUCHABLE_TIME)
	{
		player->setUntouchable_start(0);
		player->setUntouchable(false);
	}

	if (player->getUntouchable() == false) // tắt chế độ ko cho đụng
	{
		for (UINT i = 0; i < listEnemyX.size(); i++)
		{
			CGameObject * gameobj = dynamic_cast<CGameObject *> (listEnemyX[i]);
			if (gameobj->GetHealth() > 0) // còn sống
			{
				LPCOLLISIONEVENT e = player->SweptAABBEx(gameobj);
				if (e->t > 0 && e->t <= 1) // có va chạm, chưa AABB
				{
					player->SetHurt(e);
					return; // ko cần xét tiếp vì đang untouchable
				}

				if (player->isCollitionObjectWithObject(gameobj)) // chủ yếu dùng hàm này để có AABBcheck, chứ nếu có (e->t > 0 && e->t <= 1) == true thì nó dính if ở trên
				{
					LPCOLLISIONEVENT e = new CCollisionEvent(1, -player->GetNx(), 0, NULL); // player->nx đẩy hướng ngược lại

					player->SetHurt(e);
					return;
				}

			}
		}
	}


}

Item * CPlayScene::GetNewItem(int id, eType type, float x, float y)
{
	if (type == eType::TORCH)
	{
		if (id == 99)
			return new DaggerItem(x, y);

		if (id == 100 || id == 101)
			return new HolyWaterItem(x, y);

	}

	if (type == eType::CANDLE)
	{
		return new SmallHeart(x, y);
	}


	if (type == eType::ZOMBIE || type == eType::BLACKKNIGHT || type == eType::BAT || type == eType::GHOST || type == eType::HUNCHBACK || type == eType::SKELETON)
	{
		int random = rand() % 10;

		if (dynamic_cast <MorningStar*> (player->mainWeapon))
		{
			while (dynamic_cast <MorningStar*> (player->mainWeapon)->getLevel() == 2 && random == 4)
			{
				random = rand() % 10;
			}
		}

		switch (random)
		{
		case 0:
			return	new LargeHeart(x, y);
			break;
		case 1:
			return	new SmallHeart(x, y);
			break;
		case 2:
			return new DaggerItem(x, y);
			break;
		case 3:
			return new Monney(x, y);
			break;
		case 4:
			return new UpgradeMorningStar(x, y);
			break;
		case 5:
			return new HolyWaterItem(x, y);
			break;
		case 6:
			return new StopWatchItem(x, y);
			break;
		case 7:
			return new BoomerangItem(x, y);
			break;
		case 8:
			return new AxeItem(x, y);
			break;
		default: // 50% còn lại là SmallHeart
			return new SmallHeart(x, y);
			break;
		}
	}

	


	if (type == eType::SPECIALBRICK)
	{
		switch (id)
		{
		case 1000:
			return new Monney(x, y);
			break;

		default:
			return new SmallHeart(x, y);
			break;
		}

	}

	if (type == eType::HIDDENOBJECT)
	{
		switch (id)
		{
		case 5:
			return new Crown(x, y);
			break;

		default:
			break;
		}

	}
	DebugOut(L"[Create Item] Item duoc khoi tao khong xac dinh Obj cha\n");
	return new BoomerangItem(x, y);
}

void CPlayScene::ResetResource()
{
	Unload();

	LoadAgain();
}


void CPlayScene::LoadAgain()
{
	DebugOut(L"[INFO] Start loading AGAIN scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[TILEMAP]") {
			section = SCENE_SECTION_TILEMAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_TILEMAP: _ParseSection_TILEMAP(line); break;
		}
	}

	f.close();

	LoadResources();

	DebugOut(L"[INFO] Done loading scene resources AGAIN %s\n", sceneFilePath);
}

bool CPlayScene::isOncam(float x1, float y1, float w1, float h1)
{
	float xCam = Camera::GetInstance()->Getx();
	float yCam = Camera::GetInstance()->Gety();
	float wCam = Camera::GetInstance()->GetScreenWidth();
	float hCam = Camera::GetInstance()->GetScreenHeight();

	if (x1 + w1 < xCam || xCam + wCam < x1)
		return false;
	if (y1 + h1 < yCam || yCam + hCam < y1)
		return false;

	return true;
}

void CPlayScene::CreateZombie()
{
	DWORD now = GetTickCount();

	if (now - TimeZombie >= TIME_CREATE_GHOST) // đủ th.gian tạo ra 1 con zombie
	{
		for (int i = 0; i < listZombieZone.size(); i++)
		{
			if (listZombieZone[i]->isSimonInZombieZone(player)) // nếu simon trong zone
			{
				if (now - listZombieZone[i]->getLastTimeCreate() >= listZombieZone[i]->getTimeCreateZombie()) // đủ thời gian tạo ra bầy zomebie
				{
					//DebugOut(L"[ZOMBIE] lastTime zombie = %d\n", now);
					float x1, x2, y1, y2;
					listZombieZone[i]->getCreateLocation(x1, y1, x2, y2);

					if (listZombieZone[i]->getCountZombie() < listZombieZone[i]->getTotalZombie()) // khi số lượng zombie trong zone chưa đủ
					{
						//DebugOut(L"[ZOMBIE] so luong zombie cua zone = %d\n", listZombieZone[i]->getCountZombie());
						if (player->GetVx() > 0) // vx>0 simon đang đi về bên phải
						{
							// zombie chạy từ phải qua trái
							if (listZombieZone[i]->getFlag() == -1) // tạo theo camera
							{
								listZombie.push_back(new Zombie(Camera::GetInstance()->Getx() + Camera::GetInstance()->GetScreenWidth(), listZombieZone[i]->yZombie1, -1));
							}
							else // tạo theo tọa độ parse vào
							{
								listZombie.push_back(new Zombie(x2, y2, -1));
							}
							
						}
						else if (player->GetVx() < 0) // vx>0 simon đang đi về bên trái
						{
							// zombie chạy từ bên trái qua phải
							if (listZombieZone[i]->getFlag() == -1) // tạo theo camera
							{
								listZombie.push_back(new Zombie(Camera::GetInstance()->Getx(), listZombieZone[i]->yZombie1, 1));
							}
							else // tạo theo tọa độ parse vào
							{
								listZombie.push_back(new Zombie(x1, y1, 1));
							}
						}
						else // đứng yên thì random chiều zombie chạy
						{
							int random = rand() % 2;
							if (random == 0) // đi từ bên trái
							{
								if (listZombieZone[i]->getFlag() == -1) // tạo theo camera
								{
									listZombie.push_back(new Zombie(Camera::GetInstance()->Getx(), listZombieZone[i]->yZombie1, 1));
								}
								else // tạo theo tọa độ parse vào
								{
									listZombie.push_back(new Zombie(x1, y1, 1));
								}
							}
							else // đi từ bên phải
							{
								if (listZombieZone[i]->getFlag() == -1) // tạo theo camera
								{
									listZombie.push_back(new Zombie(Camera::GetInstance()->Getx() + Camera::GetInstance()->GetScreenWidth(), listZombieZone[i]->yZombie1, -1));
								}
								else // tạo theo tọa độ parse vào
								{
									listZombie.push_back(new Zombie(x2, y2, -1));
								}
							}
						}

						listZombieZone[i]->setCountZombie(listZombieZone[i]->getCountZombie() + 1);
						TimeZombie = now; // set lại thời điểm đã tạo zombie
					}
					else
					{
						listZombieZone[i]->setCountZombie(0); // đã đủ zombie trong zone thì set lại 0
						listZombieZone[i]->setLastTimeCreate(now); // đã đủ zom thì set time lại
					}
				}

			}
			else
			{
				if(listZombieZone[i]->getCountZombie() > 0) // nếu đang ra zombie mà out zone thì set lại lastTime
					listZombieZone[i]->setLastTimeCreate(now);
				listZombieZone[i]->setCountZombie(0); // ko trong zone thì set số Zom thành 0
			}
		}
	}
	
}

void CPlayScene::CreateGhost()
{
	DWORD now = GetTickCount();

	for (int i = 0; i < listGhostZone.size(); i++)
	{
		if (listGhostZone[i]->isSimonInZombieZone(player)) // nếu simon trong zone
		{
			if (listGhostZone[i]->isSimonInZoneBefore == false) // nếu simon vừa ngoài vào thì cb tạo ghost
			{
				float l, t, r, b;
				listGhostZone[i]->getBoundingBox(l, t, r, b);

				float x1, y1, left_boundary, right_boundary; //x1,y1 là tọa độ tạo ghost, left right là vùng đệm theo chiều x
				listGhostZone[i]->getCreateLocation(x1, y1, left_boundary, right_boundary);

				if (player->GetX() > l + left_boundary && player->GetX() < r - right_boundary) //vào vùng đệm để đảm bảo ghost tạo ra (x1,y1) luôn trong camera an toàn
				{

					if (player->GetNx() > 0) // di sang phai
					{
						listGhost.push_back(new Ghost(x1, y1, -1));
					}
					else
					{
						listGhost.push_back(new Ghost(x1, y1, 1));
					}
					
					listGhostZone[i]->isSimonInZoneBefore = true;
				}
			}

		}
		else
		{
			listGhostZone[i]->isSimonInZoneBefore = false;
		}
	}

}

void CPlayScene::CreateHunchback()
{
	DWORD now = GetTickCount();

	for (int i = 0; i < listHunchbackZone.size(); i++)
	{
		if (listHunchbackZone[i]->isSimonInZombieZone(player)) // nếu simon trong zone
		{
			if (listHunchbackZone[i]->isSimonInZoneBefore == false) // nếu simon vừa ngoài vào thì cb tạo ghost
			{
				float l, t, r, b;
				listHunchbackZone[i]->getBoundingBox(l, t, r, b);

				float x1, y1, left_boundary, right_boundary; //x1,y1 là tọa độ tạo ghost, left right là vùng đệm theo chiều x
				listHunchbackZone[i]->getCreateLocation(x1, y1, left_boundary, right_boundary);

				if (player->GetX() > l + left_boundary && player->GetX() < r - right_boundary) //vào vùng đệm để đảm bảo ghost tạo ra (x1,y1) luôn trong camera an toàn
				{

					if (player->GetNx() > 0) // di sang phai
					{
						listHunchback.push_back(new Hunchback(x1, y1, -1));
					}
					else
					{
						listHunchback.push_back(new Hunchback(x1, y1, 1));
					}

					listHunchbackZone[i]->isSimonInZoneBefore = true;
				}
			}

		}
		else
		{
			listHunchbackZone[i]->isSimonInZoneBefore = false;
		}
	}

}

void CPlayScene::CreateSkeleton()
{
	DWORD now = GetTickCount();

	for (int i = 0; i < listSkeletonZone.size(); i++)
	{
		if (listSkeletonZone[i]->isSimonInZombieZone(player)) // nếu simon trong zone
		{
			if (listSkeletonZone[i]->isSimonInZoneBefore == false) // nếu simon vừa ngoài vào thì cb tạo ghost
			{
				float l, t, r, b;
				listSkeletonZone[i]->getBoundingBox(l, t, r, b);

				float x1, y1, left_boundary, right_boundary; //x1,y1 là tọa độ tạo ghost, left right là vùng đệm theo chiều x
				listSkeletonZone[i]->getCreateLocation(x1, y1, left_boundary, right_boundary);

				if (player->GetX() > l + left_boundary && player->GetX() < r - right_boundary) //vào vùng đệm để đảm bảo ghost tạo ra (x1,y1) luôn trong camera an toàn
				{

					if (player->GetNx() > 0) // di sang phai
					{
						listSkeleton.push_back(new Skeleton(x1, y1, -1));
					}
					else
					{
						listSkeleton.push_back(new Skeleton(x1, y1, 1));
					}

					listSkeletonZone[i]->isSimonInZoneBefore = true;
				}
			}

		}
		else
		{
			listSkeletonZone[i]->isSimonInZoneBefore = false;
		}
	}

}

void CPlayScene::updateEnemy(DWORD dt)
{
	//Zombie========================

	for (UINT i = 0; i < listZombie.size(); i++)
	{
		CGameObject * enemy = dynamic_cast<CGameObject *>(listZombie[i]);
		if (enemy->GetHealth() > 0) // còn máu
		{
			float l, t, r, b;
			float widthEnemy, heightEnemy;
			enemy->GetBoundingBox(l, t, r, b);
			widthEnemy = b - t;
			heightEnemy = r - l;

			if (isOncam(enemy->GetX(), enemy->GetY(), widthEnemy, heightEnemy) == false)  // ra khoi cam
			{

				if (dynamic_cast<Zombie*>(enemy) != NULL) // object này là Zombie
				{
					enemy->SetHealth(0); // ra khỏi cam thì coi như chết
				}

			}
			else
			{
				enemy->Update(dt, &coObjects);
			}
		}
	}

	// BlackKinght=======================

	for (UINT i = 0; i < listBlackKnight.size(); i++)
	{
		BlackKnight * enemy = dynamic_cast<BlackKnight *>(listBlackKnight[i]);
		if (enemy->GetHealth() > 0) // còn máu
		{
			float l, t, r, b;
			float widthEnemy, heightEnemy;
			enemy->GetBoundingBox(l, t, r, b);
			widthEnemy = b - t;
			heightEnemy = r - l;

			if (isOncam(enemy->GetX(), enemy->GetY(), widthEnemy, heightEnemy) == true)  // trong camera thi update
			{
				enemy->Update(dt, player, &coObjects);
			}
		}
	}

	// Bat==========================

	for (UINT i = 0; i < listBat.size(); i++)
	{
		Bat * enemy = dynamic_cast<Bat *>(listBat[i]);
		if (enemy->GetHealth() > 0) // còn máu
		{
			float l, t, r, b;
			float widthEnemy, heightEnemy;
			enemy->GetBoundingBox(l, t, r, b);
			widthEnemy = b - t;
			heightEnemy = r - l;

			if (isOncam(enemy->GetX(), enemy->GetY(), widthEnemy, heightEnemy) == true)  // trong camera thi update
			{
				enemy->Update(dt, player, &coObjects);
			}
		}
	}



	// Ghost===========================

	for (UINT i = 0; i < listGhost.size(); i++)
	{
		Ghost * enemy = dynamic_cast<Ghost *>(listGhost[i]);
		if (enemy->GetHealth() > 0) // còn máu
		{
			float l, t, r, b;
			float widthEnemy, heightEnemy;
			enemy->GetBoundingBox(l, t, r, b);
			widthEnemy = b - t;
			heightEnemy = r - l;

			if (isOncam(enemy->GetX(), enemy->GetY(), widthEnemy, heightEnemy) == false)  // ra khoi cam
			{

				enemy->SetHealth(0); // ra khỏi cam thì coi như chết

			}
			else
			{
				enemy->Update(dt, player, &coObjects);
			}
		}
	}

	//Hunchback=================================

	for (UINT i = 0; i < listHunchback.size(); i++)
	{
		Hunchback * enemy = dynamic_cast<Hunchback *>(listHunchback[i]);
		if (enemy->GetHealth() > 0) // còn máu
		{
			float l, t, r, b;
			float widthEnemy, heightEnemy;
			enemy->GetBoundingBox(l, t, r, b);
			widthEnemy = b - t;
			heightEnemy = r - l;

			if (isOncam(enemy->GetX(), enemy->GetY(), widthEnemy, heightEnemy) == false)  // ra khoi cam
			{

				enemy->SetHealth(0); // ra khỏi cam thì coi như chết

			}
			else
			{
				enemy->Update(dt, player, &coObjects);
			}
		}
	}


	// Skeleton=================================

	for (UINT i = 0; i < listSkeleton.size(); i++)
	{
		Skeleton * enemy = dynamic_cast<Skeleton *>(listSkeleton[i]);
		if (enemy->GetHealth() > 0) // còn máu
		{
			float l, t, r, b;
			float widthEnemy, heightEnemy;
			enemy->GetBoundingBox(l, t, r, b);
			widthEnemy = b - t;
			heightEnemy = r - l;

			if (isOncam(enemy->GetX(), enemy->GetY(), widthEnemy, heightEnemy) == false)  // ra khoi cam
			{

				enemy->SetHealth(0); // ra khỏi cam thì coi như chết

			}
			else
			{
				enemy->Update(dt, player, &coObjects);
			}
		}
	}
}

