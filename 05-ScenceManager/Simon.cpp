#include "Simon.h"

#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Game.h"
#include "Goomba.h"
#include "Portal.h"

Simon::Simon() : CGameObject()
{

	isWalking = 0;
	isJumping = 0;
	isSitting = 0;
	isAttacking = 0;
	isWalkingOnStair = 0;

	health = SIMON_DEFAULT_HEALTH;
	live = SIMON_DEFAULT_HEARTCOLLECT;
	score = 0;
	heartCollected = 500;

	mainWeapon = new MorningStar();
	subWeapon = NULL;
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

	if (isAttacking == true)
	{
		if (mainWeapon->GetFinish() == false) // nếu MorningStar đang đánh
		{
			mainWeapon->Update(dt);
			if (mainWeapon->GetFinish() == true)
				isAttacking = false;
		}
		else
		{
			if (subWeapon != NULL)
			{
				isAttacking = !(animation_set->at(SIMON_ANI_SIT_ATTACK)->getCurrentFrame() == 2 || animation_set->at(SIMON_ANI_STAND_ATTACK)->getCurrentFrame() == 2);
			}
		}
	}

	if (subWeapon != NULL && subWeapon->GetFinish() == false)
	{
		subWeapon->Update(dt);
	}


	if (isOnStair)
	{

		if (isWalking == true)
		{

			walkHeight += abs(vy) * 10.0f;

			float k = 8.0f;

			if (walkHeight >= k)
			{
				walkHeight = 0;

				isWalkingOnStair++;
			}

		}


		if (isWalkingOnStair == 3)
		{
			isWalkingOnStair = 0;
			vx = 0; vy = 0;


			x = round(x / 16) * 16; // làm tròn, đứng đúng vị trí ô gạch cầu thang
			y = round(y / 16) * 16;


			isWalking = false;
		}

	}


	// Calculate dx, dy 
	CGameObject::Update(dt);

	if (isOnStair == false)
		vy += SIMON_GRAVITY * dt;// Simple fall down
	else
	{
		this->dt = dt;
		dx = vx * 10;
		dy = vy * 10;
	}


	vector<LPGAMEOBJECT> coBrick;
	coBrick.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if(dynamic_cast<CBrick *>(coObjects->at(i)) || dynamic_cast<CPortal *>(coObjects->at(i))) // tutu xu ly portal
			coBrick.push_back(coObjects->at(i));
	}
	CollisionWithBrick(&coBrick); // check Collision and update x, y for simon

	//reset vi tri weapon
	mainWeapon->SetPosition(this->x, this->y);
	mainWeapon->UpdatePositionFitSimon();
	// reset de kt va cham
	mainWeapon->SetSpeed(vx, vy);

}


void Simon::Render()
{

	/////////////////////
	int ani = -1;
	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;

	//======================================================================================================================

	if (isOnStair)
	{

		if (isWalking == true)
		{
			if (isWalkingOnStair == 1) // nếu ở giai đoạn 1
			{
				ani = SIMON_ANI_STAIR1;
			}

			if (isWalkingOnStair == 2) // nếu ở giai đoạn 2
			{
				ani = SIMON_ANI_STAIR2;
			}
		}
		else
		{
			ani = SIMON_ANI_STAIR1;
		}

	}
	else
	{
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
	}

	

	int alpha = 255;
	if (untouchable) alpha = 128;
	bool isLeft = true;
	if (nx > 0)
		isLeft = false;

	//animation_set->at(ani)->Render(x, y, alpha, isLeft); // ani sẽ có tham số nx dùng để so sanh chiều trái phải của hành động
	//// hàm Render() sẽ làm thêm 1 tham số chiều trái phải

	if (this->GetFreeze() == true)
	{
		animation_set->at(ani)->Render(x, y, alpha, isLeft, true);
	}
	else
	{
		animation_set->at(ani)->Render(x, y, alpha, isLeft);
	}

	RenderBoundingBox();
	// render weapon
	if (mainWeapon->GetFinish() == false)
		mainWeapon->Render();

	if (subWeapon != NULL && subWeapon->GetFinish() == false)
		subWeapon->Render();
}

void Simon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		if (isOnStair == true)
			return;
		if (isAttacking == true)
			return;
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		isWalking = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		if (isOnStair == true)
			return;
		if (isAttacking == true)
			return;
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		isWalking = 1;
		break;
	case SIMON_STATE_SITTING:
		if (isOnStair == true)
			return;
		vx = 0;
		isWalking = 0;

		if (isSitting == false) // nếu trước đó simon chưa ngồi
			y = y + 16; // kéo simon xuống

		isSitting = 1;
		break;
	case SIMON_STATE_JUMPING:
		if (isOnStair == true)
			return;
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
		if (isOnStair == true)
			return;
		nx = 1;
		break;
	case SIMON_STATE_LEFT:
		if (isOnStair == true)
			return;
		nx = -1;
		break;
	case SIMON_STATE_STOP:
		if (isOnStair == true)
			return;
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
	if (isOnStair == false) // ko trên thang thì check
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

			if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				DebugOut(L"[INFO] Switching to scene %d", p->GetSceneId());
				CGame::GetInstance()->SwitchScene(p->GetSceneId()); // thực thi dòng này có bug, có thể nó đã xóa coEventsResult[i] - là Obj tồn tại ở scene hiện tại - thủ phạm là Unload() của scene. Xử lý: là portal thì dừng kiểm tra các va chạm còn lại

				for (UINT i = 0; i < coEvents.size(); i++)
					delete coEvents[i];
				return;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void Simon::Attack(Weapon * w)
{
	if (isAttacking == true && dynamic_cast<MorningStar*>(w) != NULL)
		return;

	animation_set->at(SIMON_ANI_STAND_ATTACK)->setCurrentFrame(-1);// fix?
	animation_set->at(SIMON_ANI_SIT_ATTACK)->setCurrentFrame(-1);

	isAttacking = true; // set trang thái tấn công
	w->Create(this->x, this->y, this->nx); // set vị trí weapon theo simon
}

bool Simon::isCollisionWithItem(Item * objItem)
{
	if (objItem->GetFinish() == true)
		return false;

	return isCollitionObjectWithObject(objItem);
}

void Simon::SetHeartCollected(int h)
{
	heartCollected = h;
}

int Simon::GetHeartCollected()
{
	return heartCollected;
}

int Simon::GetLive()
{
	return live;
}

void Simon::SetLive(int l)
{
	live = l;
}

int Simon::GetScore()
{
	return score;
}

void Simon::SetScore(int s)
{
	score = s;
}

bool Simon::LoseLife()
{
	if (live - 1 < 0)
		return false;
	health = SIMON_DEFAULT_HEALTH;
	live = live - 1;
	heartCollected = SIMON_DEFAULT_HEARTCOLLECT;
	if (mainWeapon)
	{
		delete mainWeapon;
		mainWeapon = NULL;
	}
	if (subWeapon)
	{
		delete subWeapon;
		subWeapon = NULL;
	}
	mainWeapon = new MorningStar();
	subWeapon = NULL;

	isAttacking = 0;
	isJumping = 1;
	isSitting = 0;
	isWalking = 0;
	isFreeze = 0;

	nx = 1;


	x = xBackup;
	y = yBackup;

	vx = 0; vy = 0;
	SetState(SIMON_STATE_IDLE);

	return true;
}

void Simon::SetPositionBackup(float x1, float y1)
{
	xBackup = x1;
	yBackup = y1;
}

bool Simon::GetFreeze()
{
	return isFreeze;
}

void Simon::SetFreeze(int f)
{
	isFreeze = f;
	TimeFreeze = 0; // thời gian đã đóng băng
}

void Simon::UpdateFreeze(DWORD dt)
{
	if (TimeFreeze + dt >= TIME_FREEZE_MAX)
	{
		SetFreeze(false); // kết thúc đóng băng
	}
	else
		TimeFreeze += dt;
}



void Simon::GoUpStair()
{
	isOnStair = true;
	vx = nx * 0.5f;
	vy = -1 * 0.5f;

}