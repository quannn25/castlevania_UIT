#include "Raven.h"

Raven::Raven(float X, float Y, int Nx)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(RAVEN_ANI_SET_ID);
	SetAnimationSet(ani_set);

	this->x = X;
	this->y = Y;
	this->nx = Nx;
	this->ny = 1;
	health = 1;
	vx = RAVEN_SPEED_X * this->nx;
	vy = 0;
	this->type = eType::RAVEN;
	isActive = false;
	timeReady = 0;
	SetState(RAVEN_STATE_WAIT);
}

Raven::~Raven()
{
}

void Raven::Update(DWORD dt, Simon * simon, vector<LPGAMEOBJECT>* coObjects)
{
	if (health > 0)
	{
		CGameObject::Update(dt);

		if (x >= simon->GetX())
			nx = -1;
		else
			nx = 1;
		if (y >= simon->GetY())
			ny = -1;
		else
			ny = 1;

		if (abs(this->x - simon->x) <= RAVEN_ACTIVEZONE && abs(this->y - simon->y) <= RAVEN_ACTIVEZONE)
		{
			if (isActive == false)
			{
				isActive = true;
			}
		}


		if (isActive == true && timeReady <= TIME_READY)
		{
			timeReady += dt;
			SetState(RAVEN_STATE_READY); // cho bay tạo chỗ
			return;
		}

		if (this->state == RAVEN_STATE_READY)
			SetState(RAVEN_STATE_FIND_TARGET);

		if (this->state == RAVEN_STATE_FIND_TARGET)
		{
			randX = rand() % RANDX_READY; // chọn random 1 khoảng x
			randY = abs(this->y - ((rand() % SIMON_BBOX_HEIGHT) + simon->GetY())); // chọn y trong khoảng từ đầu simon tới chân simon

			SetState(RAVEN_STATE_GO_TARGET);
		}

		if (this->state == RAVEN_STATE_GO_TARGET)
		{
			x += dx;
			y += dy;

			randX -= abs(dx);
			randY -= abs(dy);

			if (randX <= 0) // đủ khoảng randX thì ko bay chiều x nữa
				vx = 0;

			if (randY <= 0) //...
				vy = 0;

			if (randX <= 0 && randY <= 0) // bay đủ chiều x y (đủ go_target) thì tấn công theo chiều x
			{
				randX = abs(this->x - simon->GetX());
				randY = 0;
				SetState(RAVEN_STATE_ATTACK);
			}
		}

		if (this->state == RAVEN_STATE_ATTACK)
		{
			x += dx;
			y += dy;

			randX -= abs(dx);
			randY -= abs(dy);

			if (randX <= 0)
				vx = 0;

			if (randY <= 0)
				vy = 0;

			if (randX <= 0 && randY <= 0) // đủ go_attack thì bay xuyên simon với randX randY mới
			{
				randX = 96 + rand() % (150 + 1 - 96); // bay thêm 1 khoảng qua simon từ 96-150
				randY = 0 + rand() % (64 + 1 - 0); // y thì random trong khoảng độ cao người simon

				SetState(RAVEN_STATE_GO_RANDOM);
			}
		}

		if (this->state == RAVEN_STATE_GO_RANDOM)
		{
			x += dx;
			y += dy;

			randX -= abs(dx);
			randY -= abs(dy);

			if (randX <= 0)
				vx = 0;

			if (randY <= 0)
				vy = 0;

			if (randX <= 0 && randY <= 0) // xong go_random thì tìm target mới
			{
				randX = 2 * (rand() % (int)(abs(this->x - simon->GetX()))); // cái này ko cần? qua find target again ko cần
				randY = abs(this->y - (rand() % SIMON_BBOX_HEIGHT) + simon->GetY());
				SetState(RAVEN_STATE_FIND_TARGET_AGAIN);
			}
		}

		if (this->state == RAVEN_STATE_FIND_TARGET_AGAIN)
		{
			randX = 0;
			randY = abs(this->y - ((rand() % SIMON_BBOX_HEIGHT) + simon->GetY()));

			SetState(RAVEN_STATE_GO_TARGET);
		}
	}

}

void Raven::Render()
{
	if (health <= 0)
		return;

	bool isLeft = true;
	if (nx > 0)
		isLeft = false;
	if (state == RAVEN_STATE_WAIT)
		animation_set->at(RAVEN_STATE_WAIT)->Render(x, y, 255, isLeft);
	else
		animation_set->at(RAVEN_STATE_ATTACK)->Render(x, y, 255, isLeft);
	RenderBoundingBox();
}

void Raven::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case RAVEN_STATE_WAIT:
		vx = 0;
		vy = 0;
		break;
	case RAVEN_STATE_ATTACK:
		vx = RAVEN_SPEED_X * nx;
		vy = 0;
		break;
	case RAVEN_STATE_FIND_TARGET:

		break;
	case RAVEN_STATE_FIND_TARGET_AGAIN:

		break;
	case RAVEN_STATE_GO_TARGET:
		vx = RAVEN_SPEED_X * nx;
		vy = RAVEN_SPEED_Y * ny;
		break;
	case RAVEN_STATE_GO_RANDOM:
		if (rand() % 2)
		{
			vx = RAVEN_SPEED_X;
			vy = RAVEN_SPEED_Y;
		}
		else
		{
			vx = -RAVEN_SPEED_X;
			vy = -RAVEN_SPEED_Y;
		}
		break;
	case RAVEN_STATE_READY:
		vx = 0;
		vy = 0;
		break;
	}
}

void Raven::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (health > 0)
	{
		left = x;
		top = y;
		right = left + RAVEN_FRAMEWIDTH;
		bottom = top + RAVEN_FRAMEHEIGHT;
	}
}
