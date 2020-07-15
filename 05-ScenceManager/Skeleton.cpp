#include "Skeleton.h"

Skeleton::Skeleton(float X, float Y, int Nx)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(SKELETON_ANI_SET_ID);
	SetAnimationSet(ani_set);

	this->x = X;
	this->y = Y;
	this->nx = Nx;
	health = 1;
	vx = SKELETON_SPEED_X * this->nx;
	vy = 0;
	this->type = eType::SKELETON;
	SetState(SKELETON_STATE_IDLE);
	isJumping = true;
	isAllowJump = false;
	FreeFallDown = 0; // có khoảng rơi tự do >= 2 mới đc phép nhảy
	deltaX = 0;
}

Skeleton::~Skeleton()
{
}

void Skeleton::Update(DWORD dt, Simon * simon, vector<LPGAMEOBJECT>* coObjects)
{
	if (health > 0)
	{
		if (x >= simon->GetX())
			nx = -1;
		else
			nx = 1;

		if (abs(this->x - simon->x) <= SKELETON_ACTIVEZONE && abs(this->y - simon->y) <= SKELETON_ACTIVEZONE)
		{
			if (state == SKELETON_STATE_IDLE)
			{
				SetState(SKELETON_STATE_ATTACK);
			}
		}

		vy += SIMON_GRAVITY * dt;
		CGameObject::Update(dt);

		vector<LPGAMEOBJECT> listObject_Brick;
		listObject_Brick.clear();
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (dynamic_cast<CBrick*>(coObjects->at(i)))
				listObject_Brick.push_back(coObjects->at(i));
		}

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		coEvents.clear();
		CalcPotentialCollisions(&listObject_Brick, coEvents);

		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;

			if (isJumping == false)
			{
				FreeFallDown += dy;
				if (FreeFallDown >= 2)
				{
					isJumping = true;
					//FreeFallDown = 0;
				}
			}
		}
		else
		{

			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.1f;

			if (ny < 0)
			{
				if (isJumping == true)
				{
					isJumping = false;
				}
				vy = 0;
				FreeFallDown = 0;// có va chạm gạch thì set lại freeFallDown
			}

			if (ny > 0) // va chạm dưới brick
			{
				vy = 0;
			}

			if (nx != 0)
			{
				vx = -vx;
				deltaX = 0;
			}
		}
		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];

		if (isJumping == true && FreeFallDown != 0) // nếu có 'cờ nhảy' và 'rơi tự do' != 0 thì nhảy
		{
			vy = -SKELETON_SPEED_Y;
			FreeFallDown = 0; // gán lại 0 để ko nhảy nữa
		}


		deltaX += abs(dx); // update deltaX

		if (abs(simon->GetX() - x) > ZONEATTACK_MAX && deltaX >= SKELETON_DELTAX && state != SKELETON_STATE_IDLE)
		{
			if (coEvents.size() != 0) // tránh hiện tượng đổi vx trên không trung
			{
				vx = SKELETON_SPEED_X * nx;
				deltaX = 0;
			}
		}

		if (abs(simon->GetX() - x) < ZONEATTACK_MIN && deltaX >= SKELETON_DELTAX && state != SKELETON_STATE_IDLE)
		{
			if (coEvents.size() != 0) // tránh hiện tượng đổi vx trên không trung
			{
				vx = -SKELETON_SPEED_X * nx;
				deltaX = 0;
			}
		}



		// update Bone của nó
		int b = rand() % 50;
		if (b == 0)
		{
			if (state == SKELETON_STATE_ATTACK)
			{
				Bone * bone = new Bone();
				bone->Create(this->x, this->y, this->nx);
				listBone.push_back(bone);
			}
		}


		for (int i = 0; i < listBone.size(); i++)
		{
			if (listBone[i]->GetFinish() == false)
			{
				listBone[i]->Update(dt);

				// kiểm tra va chạm simon với bone
				if (GetTickCount() - simon->getUntouchable_start() > SIMON_UNTOUCHABLE_TIME)
				{
					simon->setUntouchable_start(0);
					simon->setUntouchable(false);
				}

				if (simon->getUntouchable() == false) // tắt chế độ ko cho đụng
				{
					LPCOLLISIONEVENT e = simon->SweptAABBEx(listBone[i]);
					if (e->t > 0 && e->t <= 1) // có va chạm, chưa AABB
					{
						simon->SetHurt(e);
						continue; // ko cần xét tiếp vì đang untouchable
					}

					if (simon->isCollitionObjectWithObject(listBone[i])) // chủ yếu dùng hàm này để có AABBcheck, chứ nếu có (e->t > 0 && e->t <= 1) == true thì nó dính if ở trên
					{
						LPCOLLISIONEVENT e = new CCollisionEvent(1, -simon->GetNx(), 0, NULL); // player->nx đẩy hướng ngược lại

						simon->SetHurt(e);
					}
				}
			}
		}

		// end Update Bone
	}
}

void Skeleton::Render()
{
	for (int i = 0; i < listBone.size(); i++)
	{
		if (listBone[i]->GetFinish() == false)
		{
			listBone[i]->Render();
		}
	}

	if (health <= 0)
		return;

	bool isLeft = true;
	if (nx > 0)
		isLeft = false;

	animation_set->at(0)->Render(x, y, 255, isLeft);
	RenderBoundingBox();
}

void Skeleton::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SKELETON_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case SKELETON_STATE_ATTACK:
		vx = SKELETON_SPEED_X * nx;
		break;
	}
}

void Skeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (health > 0)
	{
		left = x;
		top = y;
		right = left + SKELETON_FRAMEWIDTH;
		bottom = top + SKELETON_FRAMEHEIGHT;
	}
}


