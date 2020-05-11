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

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
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
		break;
	case OBJECT_TYPE_BRICK:
		obj = new CBrick();
		obj->SetType(eType::BRICK);
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
		int t = atof(tokens[5].c_str()); //	1 up 2 down
		int nx1 = atof(tokens[6].c_str());
		obj = new Stair(x, y, t, nx1);
		if (t == 1)
			obj->SetType(eType::STAIR_UP);
		else
			obj->SetType(eType::STAIR_DOWN);
		}
		break;
	case OBJECT_TYPE_STAIR_QUIT:
		{
		int nx1 = atof(tokens[6].c_str());
		obj = new Stair(x, y, 3, nx1);
		obj->SetType(eType::STAIR_QUIT);
		}
		break;
	case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[5].c_str());
			float b = atof(tokens[6].c_str());
			int scene_id = atoi(tokens[7].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
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

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
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
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

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

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	LoadResources();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::LoadResources()
{
	Camera::GetInstance()->SetPosition(0.0f, 0.0f);
	grid = new Grid(objects);// ko bao gom Simon // overlaod hco nay
	boardGame = new Board(0, 0);

	gameTime = GameTime::GetInstance();
	gameTime->SetTime(0);

	player->SetPositionBackup(SIMON_POSITION_DEFAULT);

	listItem.clear();
	listEffect.clear();
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	//DebugOut(L"[Grid] Object = %d\n", objects.size());

	if (gameTime->GetTime() >= GAMETIME_SCENE_1)
	{
		if (player->GetLive() == 0) // xử lý...
			return;
		bool result = player->LoseLife();
		if (result == true) // còn mạng để chơi tiếp, giảm mạng reset máu xong
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

	
	grid->ResetTake(objects); // set lai trang thai onCam
	
	grid->GetListObject(coObjects, Camera::GetInstance()); // lay listObj onCam
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
			listEffect[i]->Update();
	}

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

	for (int i = 0; i < coObjects.size(); i++)
	{
		coObjects[i]->Render();
	}

	for (int i = 0; i < listItem.size(); i++)
	{
		if (listItem[i]->GetFinish() == false)
			listItem[i]->Render();
	}

	for (int i = 0; i < listEffect.size(); i++)
	{
		if (listEffect[i]->GetFinish() == false)
			listEffect[i]->Render();
	}

	boardGame->Render(player, 1, player->subWeapon, GAMETIME_SCENE_1 - gameTime->GetTime());

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

}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)// tạo is jumping, sitting... quản lý state
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Simon *simon = ((CPlayScene*)scence)->player;

	if (simon->GetFreeze() == true) // Đang bóng băng thì không quan tâm phím
	{
		return;
	}

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
		break;
	case DIK_X:
		simon->Attack(simon->mainWeapon);
		break;
	case DIK_Z:
		if (simon->subWeapon != NULL && simon->subWeapon->GetFinish() == true && simon->GetHeartCollected() > 0)
		{
			simon->SetHeartCollected(simon->GetHeartCollected() - 1); // giảm 1 heart
			simon->Attack(simon->subWeapon);
		}
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

	if (simon->GetFreeze() == true) // dang bóng băng
	{
		return;
	}

	if (simon->isJumping && simon->isWalking)
		return;
	// disable control key when Mario die 
	if (simon->GetState() == SIMON_STATE_DIE) return;


	if (game->IsKeyDown(DIK_UP)) 
	{
		if (simon->isOnStair == false) // nếu đang ko trên thang
		{
			for (UINT i = 0; i < _coObjects.size(); i++)
			{
				if (_coObjects[i]->GetType() == eType::STAIR_UP && simon->isCollitionObjectWithObject(_coObjects[i]) == true)
				{
					CGameObject* obj = dynamic_cast<Stair*>(_coObjects[i]);
					simon->NxStair = obj->GetNx();
					
					simon->SetNx(simon->NxStair); // hướng simon theo hướng thang

					simon->isOnStair = 1;
					simon->walkHeight = 0;

					simon->ny = -1; // hướng lên

					break;
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
	else
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
							CGameObject* gameobj = dynamic_cast<Stair*>(_coObjects[i]);
							simon->NxStair = gameobj->GetNx(); // lưu hướng của stair
							simon->ny = 1;// hướng đi xuống
							simon->SetNx(simon->NxStair * -1);// set hướng simon

							simon->isOnStair = true; // set trạng thái stair
							simon->walkHeight = 0;
						}
					}
				}

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
	CheckCollisionWeapon();

	CheckCollisionSimonWithItem();
}

void CPlayScene::CheckCollisionWeapon()
{
	// main weapon
	if (player->mainWeapon->GetFinish() == false) // Vũ khí đang hoạt động
	{
		for (UINT i = 0; i < coObjects.size(); i++)
		{
			if (dynamic_cast<Torch *>(coObjects[i])) // Torch
			{
				if (player->mainWeapon->isCollision(coObjects[i]) == true)
				{
					CGameObject *gameObjTorch = dynamic_cast<Torch*>(coObjects[i]);

					gameObjTorch->beAttacked(1);

					listEffect.push_back(new Hit(gameObjTorch->GetX() + 14, gameObjTorch->GetY() + 14)); // hiệu ứng
					listEffect.push_back(new Fire(gameObjTorch->GetX() - 5, gameObjTorch->GetY() + 8)); // hiệu ứng
					listItem.push_back(GetNewItem(gameObjTorch->GetId(), eType::TORCH, gameObjTorch->GetX() + 5, gameObjTorch->GetY()));
				}
			}

			if (dynamic_cast<Candle *>(coObjects[i])) // Candle
			{
				if (player->mainWeapon->isCollision(coObjects[i]) == true)
				{
					CGameObject *gameObjCandle = dynamic_cast<Candle *>(coObjects[i]);

					gameObjCandle->beAttacked(1);

					listEffect.push_back(new Hit(gameObjCandle->GetX() + 10, gameObjCandle->GetY() + 14)); // hiệu ứng
					listItem.push_back(GetNewItem(gameObjCandle->GetId(), eType::CANDLE, gameObjCandle->GetX() + 5, gameObjCandle->GetY()));
				}
			}
		}
	}

	// sub
	if (player->subWeapon != NULL && player->subWeapon->GetFinish() == false)
	{
		for (UINT i = 0; i < coObjects.size(); i++)
		{
			if (dynamic_cast<Torch *>(coObjects[i]))
			{
				if (player->subWeapon->isCollision(coObjects[i]) == true)
				{
					CGameObject *gameObjTorch = dynamic_cast<Torch*>(coObjects[i]);

					gameObjTorch->beAttacked(1);

					player->subWeapon->SetFinish(true);   // trúng object thì tắt luôn

					listEffect.push_back(new Hit(gameObjTorch->GetX() + 14, gameObjTorch->GetY() + 14)); // hiệu ứng
					listEffect.push_back(new Fire(gameObjTorch->GetX() - 5, gameObjTorch->GetY() + 8)); // hiệu ứng
					listItem.push_back(GetNewItem(gameObjTorch->GetId(), eType::TORCH, gameObjTorch->GetX() + 5, gameObjTorch->GetY()));
				}
			}

			if (dynamic_cast<Candle *>(coObjects[i])) // Candle
			{
				if (player->subWeapon->isCollision(coObjects[i]) == true)
				{
					CGameObject *gameObjCandle = dynamic_cast<Candle *>(coObjects[i]);

					gameObjCandle->beAttacked(1);

					player->subWeapon->SetFinish(true);   // trúng object thì tắt luôn

					listEffect.push_back(new Hit(gameObjCandle->GetX() + 10, gameObjCandle->GetY() + 14)); // hiệu ứng
					listItem.push_back(GetNewItem(gameObjCandle->GetId(), eType::CANDLE, gameObjCandle->GetX() + 5, gameObjCandle->GetY()));
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

					break;
				}
				default:
					DebugOut(L"[CheckCollisionSimonWithItem] ko xac nhan dc iTem!\n");
					break;
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
			return new UpgradeMorningStar(x, y);

	}

	if (type == eType::CANDLE)
	{
		return new SmallHeart(x, y);
	}

	return new Monney(x, y);
}

void CPlayScene::ResetResource() // ko dùng cách xóa các reSource đc...
{
	Unload();

	LoadAgain();
}


void CPlayScene::LoadAgain()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") continue;
		if (line == "[SPRITES]") continue;
		if (line == "[ANIMATIONS]") continue;
		if (line == "[ANIMATION_SETS]") continue;
		if (line == "[TILEMAP]") {
			section = SCENE_SECTION_TILEMAP; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
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

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}