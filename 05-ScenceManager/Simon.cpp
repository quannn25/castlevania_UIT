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
	isOnStair = 0;
	walkHeight = 0;

	isAutoGoX = 0;
	isFreeze = 0;
	TimeFreeze = 0;
	isHurting = 0;

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
		left = x;
		top = y;
		right = x + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_SITTING_HEIGHT;
	}
	else if (isJumping == true)
	{
		left = x;
		top = y;
		right = x + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_SITTING_HEIGHT;
	}
	else
	{
		left = x;
		top = y + 1;
		right = x + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_HEIGHT - 1;
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
				isAttacking = !(animation_set->at(SIMON_ANI_SIT_ATTACK)->getCurrentFrame() == 2 || animation_set->at(SIMON_ANI_STAND_ATTACK)->getCurrentFrame() == 2 || 
					animation_set->at(SIMON_ANI_STAIR_UP_ATTACK)->getCurrentFrame() == 2 || animation_set->at(SIMON_ANI_STAIR_DOWN_ATTACK)->getCurrentFrame() == 2);
				//DebugOut(L"ani = %d\n", animation_set->at(SIMON_ANI_STAIR_UP_ATTACK)->getCurrentFrame());
				//DebugOut(L"aniiiiiiiiiiiiiiiiiii = %d\n", animation_set->at(SIMON_ANI_STAND_ATTACK)->getCurrentFrame());
			}
		}
	}

	if (subWeapon != NULL && subWeapon->GetFinish() == false)
	{
		subWeapon->Update(dt, coObjects);
	}


	if (isOnStair)
	{

		if (isWalking == true)
		{
			walkHeight = walkHeight + abs(vy) * 16.0f;

			if (walkHeight >= 8.0f && isWalkingOnStair == 1)
				isWalkingOnStair++;

			if (walkHeight >= 16.0f)
			{
				isWalkingOnStair++;

				// di hon 16px
				if (nx == 1 && ny == -1) // đi lên bên phải
				{
					x -= (walkHeight - 16.0f);
					y += (walkHeight - 16.0f);
				}
				if (nx == -1 && ny == -1) // đi lên bên trái
				{
					x += (walkHeight - 16.0f);
					y += (walkHeight - 16.0f);
				}

				if (nx == 1 && ny == 1) // đi xuống bên phải
				{
					x -= (walkHeight - 16.0f);
					y -= (walkHeight - 16.0f);
				}
				if (nx == -1 && ny == 1) // đi xuống bên trái
				{
					x += (walkHeight - 16.0f);
					y -= (walkHeight - 16.0f);
				}

				walkHeight = 0;

			}
			//DebugOut(L"walkheight = %f\n", walkHeight);

		}

	}


	// Calculate dx, dy 
	CGameObject::Update(dt);

	if (isOnStair == false)
		vy += SIMON_GRAVITY * dt;// Simple fall down
	else
	{
		this->dt = dt;
		dx = vx * 16;
		dy = vy * 16;
	}

	if (isAutoGoX == true) // fix vx = 0 khi autoGo
	{
		if (vx != speedAuto * NxAuto)
			vx = speedAuto * NxAuto;
	}

	if (isOnStair == false)
	{
		
		CollisionWithBrick(coObjects); // check Collision and update x, y for simon

		CollisionWithPortal(coObjects);
	}
	else
	{
		CollisionWhenOnStair(coObjects);

		CollisionWithPortal(coObjects);
	}

	if (isWalkingOnStair == 3)
	{
		isWalkingOnStair = 0;
		vx = 0;
		vy = 0;
		isWalking = false;
	}

	//reset vi tri weapon reset de kt va cham
	mainWeapon->SetPosition(this->x, this->y);
	mainWeapon->SetSpeed(vx, vy);
	mainWeapon->UpdatePositionFitSimon();

	if (isAutoGoX == true) // đang auto thì kt
	{
		if (NxAuto == -1)
		{
			if (x <= xAuto)
			{
				x = xAuto;
				RestoreAfterAutoGoX(); // xong hết thì restore lại
				isAutoGoX = false;

				if (this->ny > 0) //cho simon đi 1 lần
				{
					if (isWalkingOnStair == 0 || isWalkingOnStair == 3) // kết thúc xử lí trước đó
					{
						isWalking = true;
						isWalkingOnStair = 1;
						SetNx(NxStair*-1);// đi xuống thì ngược so với stair
						SetSpeed(this->nx* SIMON_SPEED_ONSTAIR, SIMON_SPEED_ONSTAIR);
					}
				}
			}
		}

		if (NxAuto == 1)
		{
			if (x >= xAuto)
			{
				x = xAuto;
				RestoreAfterAutoGoX(); // xong hết thì restore lại
				isAutoGoX = false;

				if (this->ny > 0) //cho simon đi 1 lần
				{
					if (isWalkingOnStair == 0 || isWalkingOnStair == 3) // kết thúc xử lí trước đó
					{
						isWalking = true;
						isWalkingOnStair = 1;
						SetNx(NxStair*-1);// đi xuống thì ngược so với stair
						SetSpeed(this->nx* SIMON_SPEED_ONSTAIR, SIMON_SPEED_ONSTAIR);
					}
				}
			}
		}
		
	}
}


void Simon::Render()
{
	int ani = -1;
	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;

	if (isOnStair)
	{

		if (isAttacking == true)
		{
			if (ny == -1)
			{
				ani = SIMON_ANI_STAIR_UP_ATTACK;
				int curFrame = animation_set->at(SIMON_ANI_STAIR_UP_ATTACK)->getCurrentFrame();
				if (curFrame > 2 || curFrame < 0) 
				{
					animation_set->at(SIMON_ANI_STAIR_UP_ATTACK)->setCurrentFrame(-1); // set -1 vào render cập nhật lại 0, tránh mất frame 0
				}
			}
			else
			{
				ani = SIMON_ANI_STAIR_DOWN_ATTACK;
				int curFrame = animation_set->at(SIMON_ANI_STAIR_DOWN_ATTACK)->getCurrentFrame();
				if (curFrame > 2 || curFrame < 0)
				{
					animation_set->at(SIMON_ANI_STAIR_DOWN_ATTACK)->setCurrentFrame(-1); // set -1 vào render cập nhật lại 0, tránh mất frame 0
				}
			}
		}
		else
		{
			if (isWalking == true)
			{
				if (isWalkingOnStair == 1) // nếu ở giai đoạn 1
				{
					if (vy<0)
						ani = SIMON_ANI_STAIR_UP_1;
					else
						ani = SIMON_ANI_STAIR_DOWN_1;
				}

				if (isWalkingOnStair == 2) // nếu ở giai đoạn 2
				{
					if (vy<0)
						ani = SIMON_ANI_STAIR_UP_2;
					else
						ani = SIMON_ANI_STAIR_DOWN_2;
				}
			}
			else
			{
				if (ny == -1)
					ani = SIMON_ANI_STAIR_UP_1;
				else
					ani = SIMON_ANI_STAIR_DOWN_1;
			}
		}

	}
	else // ko trên thang
	{
		if (isHurting)
		{
			ani = SIMON_ANI_HURTING;
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
	}

	if (this->GetFreeze() == true)
	{
		ani = SIMON_ANI_FREEZE;
	}

	int alpha = 255;
	if (untouchable) alpha = 128;
	bool isLeft = true;
	if (nx > 0)
		isLeft = false;
	//DebugOut(L"nx = %d\n", nx);

	//animation_set->at(ani)->Render(x, y, alpha, isLeft); // ani sẽ có tham số nx dùng để so sanh chiều trái phải của hành động
	//// hàm Render() sẽ làm thêm 1 tham số chiều trái phải

	animation_set->at(ani)->Render(x, y, alpha, isLeft);

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
			y = y + 15; // kéo simon xuống

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
		if (isHurting)
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
		if (isHurting)
			return;

		vx = 0;

		isWalking = 0;
		if (isSitting == true) // nếu simon đang ngồi
		{
			isSitting = 0; // hủy trạng thái ngồi
			y = y - 16; // kéo simon lên
		}
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	}

}

void Simon::SetHurt(LPCOLLISIONEVENT e)
{
	bool isOnStairWalking = isWalking;
	if (isHurting == true)
		return;
	if (e->nx == 0 && e->ny == 0) // ko có va chạm
		return;

	isWalking = 0;
	isAttacking = 0;
	isJumping = 0;
	if (isSitting == true)
	{
		isSitting = 0; // hủy trạng thái ngồi
		y = y - 16; 
	}

	if (isOnStair == false)
	{
		if (e->nx != 0)// hướng bay ra
		{
			vx = SIMON_WALKING_SPEED * e->nx * 2;
			vy = -SIMON_JUMP_SPEED_Y;
			isHurting = 1;
		}

		if (e->ny != 0)
		{
			vy = -SIMON_JUMP_SPEED_Y;
			isHurting = 1;
		}

	}
	else
	{
		if (isOnStairWalking)
			isWalking = 1;
	}

	StartUntouchable(); // bật untouch
	mainWeapon->SetFinish(true);
	beAttacked(2); // va chạm quái mất máu

}

void Simon::CollisionWithPortal(vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> list_Portal;
	list_Portal.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == eType::PORTAL)
			list_Portal.push_back(coObjects->at(i));
	}

	CalcPotentialCollisions(&list_Portal, coEvents);


	if (coEvents.size() == 0)
	{
		return;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);



		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				DebugOut(L"[INFO] Switching to scene %d with switchType %d", p->GetSceneId(), p->GetSwitchType());
				CGame::GetInstance()->SwitchScene(p->GetSceneId(), p->GetSwitchType()); // thực thi dòng này có bug, có thể nó đã xóa coEventsResult[i] - là Obj tồn tại ở scene hiện tại - thủ phạm là Unload() của scene. Xử lý: là portal thì dừng kiểm tra các va chạm còn lại

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

void Simon::CollisionWithBrick(vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> list_Brick;
	list_Brick.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == eType::BRICK || coObjects->at(i)->GetType() == eType::SPECIALBRICK)
			list_Brick.push_back(coObjects->at(i));
	}


	// turn off collision when die 
	//if (state != SIMON_STATE_DIE)
	CalcPotentialCollisions(&list_Brick, coEvents);

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
			if (isJumping == true) // nếu simon đang nhảy
			{
				y = y - 16; // kéo simon lên
			}
			vy = 0;
			isJumping = false;
		}

		if (nx != 0 || ny != 0)
		{
			isHurting = 0;
		}


	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void Simon::Attack(Weapon * w)
{
	if (isAttacking == true)// && dynamic_cast<MorningStar*>(w) != NULL)
	{
		//DebugOut(L"[weapon] return");
		return;
	}
	if (isWalkingOnStair != 0) // đang xử lý stair thì ko đánh
		return;

	animation_set->at(SIMON_ANI_STAND_ATTACK)->setCurrentFrame(-1);// fix?
	animation_set->at(SIMON_ANI_SIT_ATTACK)->setCurrentFrame(-1);
	animation_set->at(SIMON_ANI_STAIR_UP_ATTACK)->setCurrentFrame(-1);
	animation_set->at(SIMON_ANI_STAIR_DOWN_ATTACK)->setCurrentFrame(-1);

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

	isAutoGoX = 0;

	isHurting = 0;
	isOnStair = 0;
	isWalkingOnStair = 0;
	walkHeight = 0;
	TimeFreeze = 0;

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


void Simon::CollisionWhenOnStair(vector<LPGAMEOBJECT> *coObjects)
{
	if (ny == 1) // đang đi xuống
	{
		int CountCollisionDown = 0;
		int CountCollisionUp = 0;
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (coObjects->at(i)->GetType() == eType::STAIR_DOWN)
			{
				if (this->isCollitionObjectWithObject(coObjects->at(i))) // có va chạm với down
				{
					CountCollisionDown++;
					continue;
				}
			}

			if (coObjects->at(i)->GetType() == eType::STAIR_UP)
			{
				if (this->isCollitionObjectWithObject(coObjects->at(i))) // có va chạm với up
				{
					CountCollisionUp++;
					continue;
				}
			}
		}

		if (CountCollisionUp > 0 && CountCollisionDown > 0) // nếu đang xuống và va chạm cả 2 stair
		{
			// cho đi bình thường

			// nhưng set lại nx theo hướng của thang down hiện đang va chạm
			for (UINT i = 0; i < coObjects->size(); i++)
			{
				if (coObjects->at(i)->GetType() == eType::STAIR_DOWN)
				{
					if (this->isCollitionObjectWithObject(coObjects->at(i))) // có va chạm với down
					{


						if (this->nx < 0) // x giảm dần
						{
							if (this->x <= coObjects->at(i)->x) //auto go lên đã đúng == x, nhưng cho chắc thì đi vượt quá xíu cũng đc
							{
								y = y - 50; // kéo simon lên cao
								vy = 50; // vận tốc kéo xuống lớn
								dy = vy * dt; // cập nhật lại dy


								vector<LPCOLLISIONEVENT> coEvents;
								vector<LPCOLLISIONEVENT> coEventsResult;
								coEvents.clear();
								vector<LPGAMEOBJECT> list_Brick;
								list_Brick.clear();
								for (UINT i = 0; i < coObjects->size(); i++)
									if (coObjects->at(i)->GetType() == eType::BRICK)
										list_Brick.push_back(coObjects->at(i));
								CalcPotentialCollisions(&list_Brick, coEvents);
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

									FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
									x += min_tx * dx + nx * 0.4f;
									y += min_ty * dy + ny * 0.4f;
									if (nx != 0 || ny != 0)
									{
										vx = 0;
										vy = 0;
										isOnStair = false; // kết thúc việc đang trên cầu thang
										isWalking = false;
										isWalkingOnStair = 0;
									}
								}

								for (UINT i = 0; i < coEvents.size(); i++)
									delete coEvents[i];

								return; // ko cần xét tiếp
							}
						}
						else // x tăng dần
						{
							if (this->x >= coObjects->at(i)->x) //auto go lên đã đúng == x, nhưng cho chắc thì đi vượt quá xíu cũng đc
							{
								if (this->nx != -coObjects->at(i)->GetNx())
								{
									y = y - 50; // kéo simon lên cao
									vy = 50; // vận tốc kéo xuống lớn
									dy = vy * dt; // cập nhật lại dy


									vector<LPCOLLISIONEVENT> coEvents;
									vector<LPCOLLISIONEVENT> coEventsResult;
									coEvents.clear();
									vector<LPGAMEOBJECT> list_Brick;
									list_Brick.clear();
									for (UINT i = 0; i < coObjects->size(); i++)
										if (coObjects->at(i)->GetType() == eType::BRICK)
											list_Brick.push_back(coObjects->at(i));
									CalcPotentialCollisions(&list_Brick, coEvents);
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

										FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
										x += min_tx * dx + nx * 0.4f;
										y += min_ty * dy + ny * 0.4f;
										if (nx != 0 || ny != 0)
										{
											vx = 0;
											vy = 0;
											isOnStair = false; // kết thúc việc đang trên cầu thang
											isWalking = false;
											isWalkingOnStair = 0;
										}
									}

									for (UINT i = 0; i < coEvents.size(); i++)
										delete coEvents[i];

									return; // ko cần xét tiếp
								}
							}
						}


					}
				}
			}

			x += dx;
			y += dy;
			return;// ko cần xét tiếp
		}

		if (CountCollisionUp > 0 && CountCollisionDown == 0) // đang đi xuống và chỉ va chạm thang up
		{
			vector<LPCOLLISIONEVENT> coEvents;
			vector<LPCOLLISIONEVENT> coEventsResult;
			coEvents.clear();
			vector<LPGAMEOBJECT> list_Brick;
			list_Brick.clear();
			for (UINT i = 0; i < coObjects->size(); i++)
				if (coObjects->at(i)->GetType() == eType::BRICK)
					list_Brick.push_back(coObjects->at(i));
			CalcPotentialCollisions(&list_Brick, coEvents);
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

				FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				if (nx != 0 || ny != 0)
				{
					vx = 0;
					vy = 0;
					isOnStair = false; // kết thúc việc đang trên cầu thang
					isWalking = false;
					isWalkingOnStair = 0;
				}
			}

			for (UINT i = 0; i < coEvents.size(); i++)
				delete coEvents[i];

			return; // ko cần xét tiếp
		}

	}




	if (ny == -1) // đang đi lên
	{
		int CountCollisionDown = 0;
		int CountCollisionUp = 0;
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (coObjects->at(i)->GetType() == eType::STAIR_DOWN)
			{
				if (this->isCollitionObjectWithObject(coObjects->at(i))) // có va chạm với down
				{
					CountCollisionDown++;
					continue;
				}
			}

			if (coObjects->at(i)->GetType() == eType::STAIR_UP)
			{
				if (this->isCollitionObjectWithObject(coObjects->at(i))) // có va chạm với up
				{
					CountCollisionUp++;
					continue;
				}
			}
		}

		if (CountCollisionUp > 0 && CountCollisionDown > 0) // nếu đang lên và va chạm cả 2 stair
		{
			// cho đi bình thường

			// nhưng set lại nx theo hướng của thang up hiện đang va chạm
			for (UINT i = 0; i < coObjects->size(); i++)
			{
				if (coObjects->at(i)->GetType() == eType::STAIR_UP)
				{
					if (this->isCollitionObjectWithObject(coObjects->at(i))) // có va chạm với up
					{
						if (this->nx != coObjects->at(i)->GetNx())
						{
							this->NxStair = coObjects->at(i)->GetNx();
							this->nx = coObjects->at(i)->GetNx();
							this->vx = -this->vx;

							if (vx >= 0) //nếu vận tốc > 0 mà dx vẫn < 0 thì set lại
							{
								if (dx <= 0)
								{
									dx = -dx;
								}
							}
							else
							{
								if (dx >= 0)
								{
									dx = -dx;
								}
							}

						}
						break;
					}
				}
			}

			x += dx;
			y += dy;
			return;// ko cần xét tiếp
		}

		if (CountCollisionUp == 0 && CountCollisionDown > 0) // đang đi lên và chỉ va chạm down
		{

			y = y - 50; // kéo simon lên cao
			vy = 50; // vận tốc kéo xuống lớn
			dy = vy * dt; // cập nhật lại dy


			vector<LPCOLLISIONEVENT> coEvents;
			vector<LPCOLLISIONEVENT> coEventsResult;
			coEvents.clear();
			vector<LPGAMEOBJECT> list_Brick;
			list_Brick.clear();
			for (UINT i = 0; i < coObjects->size(); i++)
				if (coObjects->at(i)->GetType() == eType::BRICK)
					list_Brick.push_back(coObjects->at(i));
			CalcPotentialCollisions(&list_Brick, coEvents);
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

				FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				if (nx != 0 || ny != 0)
				{
					vx = 0;
					vy = 0;
					isOnStair = false; // kết thúc việc đang trên cầu thang
					isWalking = false;
					isWalkingOnStair = 0;
				}
			}

			for (UINT i = 0; i < coEvents.size(); i++)
				delete coEvents[i];

			return; // ko cần xét tiếp
		}

	}

	// nếu không đụng up và down thì di chuyển bt
	x += dx;
	y += dy;
}

void Simon::SetAutoGoX(int NxAuto1, int NxAfterAuto1, float xAuto1, float Speed)
{
	if (isAutoGoX == true)
		return;

	isAutoGoX = true;// chưa vào chế độ auto thì set

	AutoGoX_Backup_X = x;

	//Backup trạng thái
	isWalking_Backup = isWalking;
	isJumping_Backup = isJumping;
	isSitting_Backup = isSitting;
	isAttacking_Backup = isAttacking;
	isOnStair_Backup = isOnStair;
	isWalkingOnStair_Backup = isWalkingOnStair;
	NxStair_Backup = NxStair;
	ny_Backup = ny;

	xAuto = xAuto1;
	speedAuto = Speed;
	NxAuto = NxAuto1;
	NxAfterAuto = NxAfterAuto1;

	nx = NxAuto1;
	vx = Speed * NxAuto;
	isWalking = 1;
	isJumping = 0;
	isSitting = 0;
	isAttacking = 0;
	isOnStair = 0;
	isWalkingOnStair = 0;
}

void Simon::RestoreAfterAutoGoX()
{

	isWalking = isWalking_Backup;
	isJumping = isJumping_Backup;
	isSitting = isSitting_Backup;
	isAttacking = isAttacking_Backup;
	isOnStair = isOnStair_Backup;
	isWalkingOnStair = isWalkingOnStair_Backup;
	NxStair = NxStair_Backup;
	ny = ny_Backup;
	nx = NxAfterAuto;


	// đứng yên hết
	isAutoGoX = 0;
	isWalking = 0;
	vy = 0;
	vx = 0;
}

void Simon::setUntouchable(bool a)
{
	this->untouchable = a;
}

bool Simon::getUntouchable()
{
	return this->untouchable;
}

void Simon::setUntouchable_start(int time)
{
	this->untouchable_start = time;
}

DWORD Simon::getUntouchable_start()
{
	return this->untouchable_start;
}
