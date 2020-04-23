﻿#include "Simon.h"

#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

Simon::Simon() : CGameObject()
{

	isWalking = 0; // sua doi is jumping, sitting... thành state hết
	isJumping = 0;// sua doi is jumping, sitting... thành state hết
	isSitting = 0;// sua doi is jumping, sitting... thành state hết
	isAttacking = 0;
	health = 16;
	ListWeapon.clear();
	ListWeapon.push_back(new MorningStar());
}


Simon::~Simon()
{
}

void Simon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (isSitting == true) // simon đang ngồi
	{
		left = x + 15;
		top = y;
		right = x + SIMON_BBOX_WIDTH - 12;
		bottom = y + SIMON_BBOX_SITTING_HEIGHT - 3;
	}
	else
	{
		left = x + 15;
		top = y;
		right = x + SIMON_BBOX_WIDTH - 12;
		bottom = y + SIMON_BBOX_HEIGHT - 3;
	}

}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	// Không cho lọt khỏi camera 
	if (x < -12) // -12 do sprite render to hơn bbox 12
		x = -12.0f;
	if (x + SIMON_BBOX_WIDTH > MapWidth)
		x = float(MapWidth - SIMON_BBOX_WIDTH);

	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	vector<LPGAMEOBJECT> coBrick;
	coBrick.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if(dynamic_cast<CBrick *>(coObjects->at(i)) || dynamic_cast<CPortal *>(coObjects->at(i))) // tutu xu ly portal
			coBrick.push_back(coObjects->at(i));
	}
	CollisionWithBrick(&coBrick); // check Collision and update x, y for simon

	CollisionWithItem();

	if (isAttacking == true) 
	{
		if (ListWeapon[0]->GetFinish() == false) 
		{
			ListWeapon[0]->SetPosition(this->x, this->y);
			ListWeapon[0]->UpdatePositionFitSimon();
			ListWeapon[0]->Update();
			if (ListWeapon[0]->GetFinish() == true) 
				isAttacking = false;
			ListWeapon[0]->CollisionWithObject(dt, coObjects); // kiểm tra va chạm với các object khác
		}
	}
}


void Simon::Render()
{

	/////////////////////
	int ani = -1;
	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;

	if (isSitting == true)
	{
		if (isAttacking == true) // tấn công
		{
			ani = SIMON_ANI_SIT_ATTACK;
			int curFrame = animation_set->at(SIMON_ANI_SIT_ATTACK)->getCurrentFrame();
			if (curFrame > 2 || curFrame < 0) // bị lỗi vũ khí render tốn time nên kết thúc đánh ở frame 1 còn frame 2 chưa render nên hàm này ko vào dc => lần đánh sau đủ sẽ bắt đầu render frame 2 dù mới bắt đầu đánh
			{
				animation_set->at(SIMON_ANI_SIT_ATTACK)->setCurrentFrame(-1); // set -1 vào render cập nhật lại 0, tránh mất frame 0
			}
		}
		else
			ani = SIMON_ANI_SITTING;
	}
	else // standing
	{
		if (isAttacking == true)
		{
			ani = SIMON_ANI_STAND_ATTACK;
			int curFrame = animation_set->at(SIMON_ANI_STAND_ATTACK)->getCurrentFrame();
			if (curFrame > 2 || curFrame < 0)
			{
				animation_set->at(SIMON_ANI_STAND_ATTACK)->setCurrentFrame(-1);
			}
		}
		else if (isWalking == true)
		{
			if (isJumping == false) 
			{
				ani = SIMON_ANI_WALKING;

			}
			else
			{
				ani = SIMON_ANI_JUMPING;
			}

		}
		else
		{
			if (isJumping == true) // nếu ko đi mà chỉ nhảy
			{
				ani = SIMON_ANI_JUMPING;
			}
			else
			{
				ani = SiMON_ANI_IDLE;		// SIMON đứng yên

			}
		}
	}

	int alpha = 255;
	if (untouchable) alpha = 128;
	bool isLeft = true;
	if (nx > 0)
		isLeft = false;

	animation_set->at(ani)->Render(x, y, alpha, isLeft, GetStateChange()); // ani sẽ có tham số nx dùng để so sanh chiều trái phải của hành động
	// hàm Render() sẽ làm thêm 1 tham số chiều trái phải

	RenderBoundingBox();
	// render weapon
	for (UINT i = 0; i < ListWeapon.size(); i++)
	{
		if (ListWeapon[i]->GetFinish() == false)
		{
			ListWeapon[i]->Render();
		}
	}
}

void Simon::SetState(int state)
{

	int stateChange = CGameObject::GetStateChange();
	int LastState = CGameObject::GetState();
	if (LastState != state) // kiểm tra nếu trạng thái thay đổi so với hiện tại thì set stateChange
	{
		stateChange = 1;
	}
	else
		stateChange = 0;
	/*if (stateChange == 1)
	{
		DebugOut(L"[ERR] Invalid object type: %d\n", state);
		DebugOut(L"[ERR] Invalid LASTTT: %d\n", LastState);
	}
	else
		DebugOut(L"[ERR] Invalid object type: %d\n", 0);*/
	CGameObject::SetStateChange(stateChange);
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		if (isAttacking == true)
			return;
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		isWalking = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		if (isAttacking == true)
			return;
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		isWalking = 1;
		break;
	case SIMON_STATE_SITTING:
		vx = 0;
		isWalking = 0;

		if (isSitting == false) // nếu trước đó simon chưa ngồi
			y = y + 16; // kéo simon xuống

		isSitting = 1;
		break;
	case SIMON_STATE_JUMPING:
		if (isSitting == true)
			return;
		if (isAttacking == true)
			return;
		if (isJumping == true)
			return;
		y -= 16;
		vy = -SIMON_JUMP_SPEED_Y;
		isJumping = true;
		break;
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	case SIMON_STATE_RIGHT:
		nx = 1;
		break;
	case SIMON_STATE_LEFT:
		nx = -1;
		break;
	case SIMON_STATE_STOP:
		if (isAttacking == true)
			return;

		vx = 0;

		isWalking = 0;
		if (isSitting == true) // nếu simon đang ngồi
		{
			isSitting = 0; // hủy trạng thái ngồi
			y = y - 18; // kéo simon lên
		}
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	}

}

void Simon::CollisionWithBrick(vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	//if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	/*if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}*/

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0)
			vx = 0;
		if (ny != 0)
		{
			vy = 0;
			isJumping = false;
		}


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				//CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				//// jump on top >> kill Goomba and deflect a bit 
				//if (e->ny < 0)
				//{
				//	if (goomba->GetState() != GOOMBA_STATE_DIE)
				//	{
				//		goomba->SetState(GOOMBA_STATE_DIE);
				//		vy = -MARIO_JUMP_DEFLECT_SPEED;
				//	}
				//}
				//else if (e->nx != 0)
				//{
				//	if (untouchable == 0)
				//	{
				//		if (goomba->GetState() != GOOMBA_STATE_DIE)
				//		{
				//			if (level > MARIO_LEVEL_SMALL)
				//			{
				//				level = MARIO_LEVEL_SMALL;
				//				StartUntouchable();
				//			}
				//			else
				//				SetState(MARIO_STATE_DIE);
				//		}
				//	}
				//}
			} // if Goomba
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				DebugOut(L"[INFO] Switching to scene %d", p->GetSceneId());
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Simon::Attack(Weapon * w)
{
	if (isAttacking == true) // đang tấn công thì bỏ qua
		return;

	isAttacking = true; // set trang thái tấn công
	w->Create(this->x, this->y, this->nx); // set vị trí weapon theo simon
}

void Simon::CollisionWithItem()
{
	//ItemManager *ListItem = ItemManager::GetInstance();
	//vector<LPGAMEOBJECT> listObj;
	//listObj.clear();


	///*Xóa những Item đã kết thúc*/
	//vector<Item*>::iterator it;
	//for (it = ListItem->ListItem.begin(); it != ListItem->ListItem.end(); ) // stackoverlow remove obj vector c++
	//{
	//	if ((*it)->GetFinish() == true)
	//	{
	//		it = ListItem->ListItem.erase(it);
	//	}
	//	else
	//		++it;
	//}

	//float l, t, r, b; // bbox simon
	//float l1, t1, r1, b1;
	//RECT rect, rect1;
	//GetBoundingBox(l, t, r, b);
	//rect.left = l;
	//rect.top = t;
	//rect.right = r;
	//rect.bottom = b;

	//for (UINT i = 0; i < ListItem->ListItem.size(); i++) // simon có thể đã va chạm item rồi?
	//{
	//	ListItem->ListItem.at(i)->GetBoundingBox(l1, t1, r1, b1);
	//	rect1.left = l1;
	//	rect1.top = t1;
	//	rect1.right = r1;
	//	rect1.bottom = b1;
	//	if (CGame::GetInstance()->AABBCheck(rect, rect1) == true)
	//	{
	//		ListItem->ListItem.at(i)->SetReward();
	//		ListItem->ListItem.at(i)->SetFinish(true);
	//	}
	//}



	//for (UINT i = 0; i < ListItem->ListItem.size(); i++)
	//{
	//	if (ListItem->ListItem[i]->GetFinish() == false) // chưa kết thúc thì xét
	//	{
	//		listObj.push_back(ListItem->ListItem[i]);
	//	}
	//}


	//vector<LPCOLLISIONEVENT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;

	//coEvents.clear();

	//CalcPotentialCollisions(&listObj, coEvents); // Lấy danh sách các va chạm

	//											 // No collision occured, proceed normally
	//if (coEvents.size() != 0)
	//{
	//	float min_tx, min_ty, nx = 0, ny;
	//	float rdx = 0;
	//	float rdy = 0;

	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	//	for (UINT i = 0; i < coEventsResult.size(); i++) // xử lý
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];
	//		Item *item = dynamic_cast<Item *>(e->obj); 

	//		item->SetReward();
	//		item->SetFinish(true);

	//	}
	//}

	//for (UINT i = 0; i < coEvents.size(); i++)
	//	delete coEvents[i];

}

void Simon::SetHeartCollected(int h)
{
	heartCollected = h;
}

int Simon::GetHeartCollected()
{
	return heartCollected;
}