#include "BlackKnight.h"



BlackKnight::BlackKnight(float left_boundary1, float right_boundary1, int nx1)
{
	this->nx = nx1;
	this->left_boundary = left_boundary1;
	this->right_boundary = right_boundary1;
	this->delta = 0;
	this->walkDelta = 100;

	health = 2; // set máu
	vx = BLACKKNIGHT_SPEED * this->nx;
	
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

	DebugOut(L"[INFO] BK_health = %d\n", this->health);
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

	if (x >= right_boundary || x <= left_boundary) // kiểm tra lại vượt biên
	{
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
}
