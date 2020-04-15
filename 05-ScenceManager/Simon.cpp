#include "Simon.h"

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
		left = x;
		top = y; 
		right = x + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_SITTING_HEIGHT;
	}
	else
	{
		left = x;
		top = y;
		right = x + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_HEIGHT;
	}

}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	// Không cho lọt khỏi camera 
	if (x < 0)
		x = 0;
	if (x + SIMON_BBOX_WIDTH > MapWidth)
		x = MapWidth - SIMON_BBOX_WIDTH;

	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	vector<LPGAMEOBJECT> coBrick;
	coBrick.clear();
	for (int i = 0; i < coObjects->size(); i++)
		if (coObjects->at(i)->GetType() == eID::BRICK || coObjects->at(i)->GetType() == eID::PORTAL)
			coBrick.push_back(coObjects->at(i));
	CollisionWithBrick(&coBrick); // check Collision and update x, y for simon


	if (isAttacking == true) 
	{
		if (ListWeapon[0]->GetFinish() == false) 
		{
			ListWeapon[0]->SetPosition(this->x, this->y);
			ListWeapon[0]->UpdatePositionFitSimon();
			ListWeapon[0]->Update();
			if (ListWeapon[0]->GetFinish() == true) 
				isAttacking = false;
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
		ani = SIMON_ANI_SITTING;
	}
	else
	{
		if (isWalking == true) // đang di chuyển
		{
			if (isJumping == false) // ko nhảy
			{
				ani = SIMON_ANI_WALKING;

				//cập nhật frame mới
				//_sprite->Update(dt); // dt này được cập nhật khi gọi update; 
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
	for (int i = 0; i < ListWeapon.size(); i++)
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
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		isWalking = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
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
		if (vx != 0)
			vx -= dt * SIMON_GRAVITY*0.1*nx;
		if (nx == 1 && vx < 0)
			vx = 0;
		if (nx == -1 && vx > 0)
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
