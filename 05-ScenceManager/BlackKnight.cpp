#include "BlackKnight.h"



BlackKnight::BlackKnight(float left_boundary1, float right_boundary1, int nx1)
{
	this->nx = nx1;
	this->left_boundary = left_boundary1;
	this->right_boundary = right_boundary1;
	this->delta = 0;
	this->walkDelta = 130;

	health = 2; // set máu
	vx = BLACKKNIGHT_SPEED * this->nx;
	isHurt = 0;
	hurtTime = 0;
}

BlackKnight::~BlackKnight()
{
}

void BlackKnight::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + BLACKKNIGHT_FRAMEWIDTH;
	bottom = y + BLACKKNIGHT_FRAMEHEIGHT;
}

void BlackKnight::Update(DWORD dt, Simon * simon, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (isHurt)
	{
		hurtTime += dt;
		if (hurtTime >= BLACKKNIGHT_HURT_TIME)
			isHurt = false;
	}

	//DebugOut(L"[INFO] BK_health = %d\n", this->health);
	if (simon->GetY() >= this->y - 10 && simon->GetY() <= this->y + 10) // simon trong tầm thì hướng về simon
	{
		if (simon->GetX() <= this->x) // kiểm tra vị trí simon
		{
			delta = 0; // cho delta luôn về 0
			nx = -1;
			dx = -(abs(dx));

			vx = BLACKKNIGHT_SPEED * this->nx;
		}
		else
		{
			delta = 0; // cho delta luôn về 0
			nx = 1;
			dx = abs(dx);

			vx = BLACKKNIGHT_SPEED * this->nx;
		}

	}

	if (x > right_boundary || x < left_boundary) // kiểm tra lại vượt biên
	{
		if (x > right_boundary)
		{
			x = right_boundary;
		}
		if (x < left_boundary)
		{
			x = left_boundary;
		}

		nx = -nx;
		dx = -dx;

		vx = BLACKKNIGHT_SPEED * this->nx;
	}

	if (delta >= walkDelta) // kiểm tra delta
	{
		delta = 0;
		nx = -nx;
		dx = -dx;

		vx = BLACKKNIGHT_SPEED * this->nx;
	}

	x += dx;
	delta += abs(dx);

	// nếu đụng biên phải thì quay lại
	// xong tiếp tục đi hướng của nó, nếu y simon trong độ cao khoảng xác định dựa trên y bkacknight (yBK - 10 < ySimon < yBK + 10 ) thì quay về hướng của simon (xsimon < xBK => nxBK = -1, ngược lại)


	vy += BLACKKNIGHT_GRAVITY * dt;

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
		
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		
		y += min_ty * dy + ny * 0.4f;
	

		if (ny != 0)
		{
			vy = 0;
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}
