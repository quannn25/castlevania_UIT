#include "Hunchback.h"

Hunchback::Hunchback(float X, float Y, int Nx)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(HUNCHBACK_ANI_SET_ID);
	SetAnimationSet(ani_set);

	this->x = X;
	this->y = Y;
	this->nx = Nx;
	health = 1;
	vx = HUNCHBACK_SPEED_X * this->nx;
	vy = 0;
	this->type = eType::HUNCHBACK;
	isActive = false;
	timeReady = 0;
	isJumping = true;
	SetState(HUNCHBACK_STATE_WAIT);
}

Hunchback::~Hunchback()
{
}

void Hunchback::Update(DWORD dt, Simon * simon, vector<LPGAMEOBJECT>* coObjects)
{
	if (health > 0)
	{
		CGameObject::Update(dt);

		vy += HUNCHBACK_GRAVITY * dt;

		if (x >= simon->GetX())
			nx = -1;
		else
			nx = 1;
		if (abs(this->x - simon->x) <= HUNCHBACK_ACTIVEZONE && abs(this->y - simon->y) <= HUNCHBACK_ACTIVEZONE)
		{
			if (isActive == false)
			{
				isActive = true;
			}
		}

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
		CalcPotentialCollisions(&listObject_Brick, coEvents); // Lấy danh sách các va chạm 
		if (coEvents.size() == 0)
		{
			y += dy;
			x += dx;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (e->ny != 0)
				{
					if (e->ny == -1)
					{
						vy = 0;
						if (isJumping == true)
						{
							isJumping = false;
						}
					}
					else
						y += dy;
				}
			}
		}

		if (isActive == true && timeReady <= TIME_READY)
		{
			timeReady += dt;
			SetState(HUNCHBACK_STATE_READY);
			return;
		}

		if (this->state == HUNCHBACK_STATE_READY)
			SetState(HUNCHBACK_STATE_JUMP);

		if (state != HUNCHBACK_STATE_WAIT)
		{
			if (x < simon->GetX())
			{
				if (abs(simon->GetX() - x) > HUNCHBACK_ATTACKZONE)
				{
					if (isJumping == false)
					{
						nx = 1;
					}
					SetState(HUNCHBACK_STATE_ATTACK);
				}
			}
			else if (x > simon->GetX())
			{
				if ((abs(simon->GetX() - x) > HUNCHBACK_ATTACKZONE))
				{
					if (isJumping == false)
					{
						nx = -1;
					}
					SetState(HUNCHBACK_STATE_ATTACK);
				}
			}

			if ((rand() % 10000 < 300))
			{
				if (isJumping == false && (abs(simon->GetX() - x) < HUNCHBACK_ATTACKZONE))
				{
					SetState(HUNCHBACK_STATE_JUMP);
				}
			}
		}

		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}

}

void Hunchback::Render()
{
	if (health <= 0)
		return;

	bool isLeft = true;
	if (nx > 0)
		isLeft = false;
	animation_set->at(state)->Render(x, y, 255, isLeft);
	RenderBoundingBox();
}

void Hunchback::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case HUNCHBACK_STATE_WAIT:
		vx = 0;
		break;
	case HUNCHBACK_STATE_ATTACK:
		if (nx > 0)
			vx = HUNCHBACK_SPEED_X;
		else
			vx = -HUNCHBACK_SPEED_X;
		break;
	case HUNCHBACK_STATE_JUMP:
		isJumping = true;
		vy = -HUNCHBACK_SPEED_Y;
		break;
	case HUNCHBACK_STATE_READY:
		vx = 0;
		break;
	}
}

void Hunchback::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (health > 0)
	{
		left = x;
		top = y;
		right = left + HUNCHBACK_FRAMEWIDTH;
		bottom = top + HUNCHBACK_FRAMEHEIGHT;
	}
}
