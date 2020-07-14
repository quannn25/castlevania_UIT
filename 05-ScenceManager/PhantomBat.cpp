﻿#include "PhantomBat.h"

PhantomBat::PhantomBat(float X, float Y, float _leftBoundary, float _rightBoundary, Simon *_simon)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(PHANTOMBAT_ANI_SET_ID);
	SetAnimationSet(ani_set);

	type = eType::PHANTOMBAT;
	this->x = X;
	this->y = Y;
	this->leftBoundary = _leftBoundary;
	this->rightBoundary = _rightBoundary;
	this->simon = _simon;
	this->isHurt = false;
	this->hurtTime = 0;
	health = PHANTOMBAT_DEFAULT_HEALTH;
	StatusProcessing = PHANTOMBAT_PROCESS_SLEEP;
	xBefore = x;
	yBefore = y;

	isWaiting = false;
	yLastFrame = y;
	vx = vy = 0;
	isUseBezierCurves = false;
}

PhantomBat::~PhantomBat()
{
}


void PhantomBat::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + PHANTOMBAT_FRAMEWIDTH;
	bottom = y + PHANTOMBAT_FRAMEHEIGHT;
}

void PhantomBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetHealth() <= 0)
		return;

	if (isHurt)
	{
		hurtTime += dt;
		if (hurtTime >= PHANTOMBAT_HURT_TIME)
		{
			isHurt = false;
			hurtTime = 0;
		}
	}


	switch (StatusProcessing)
	{
	case PHANTOMBAT_PROCESS_SLEEP:
	{

		break;
	}

	case PHANTOMBAT_PROCESS_START_1: // đi xuống
	{
		// bắt đầu thì đi xuống 1 đoạn ngắn
		if (y >= yTarget)
		{
			vy = 0;
			StatusProcessing = PHANTOMBAT_PROCESS_START_2; // qua trạng thái di chuyển đến cửa sổ

			xBefore = x;
			yBefore = y;

			xTarget = 1380;
			yTarget = 201; // tương đối, vì process_2 xét xTarget

			vx = ((xTarget - xBefore) / (1500.0f)); // Vận tốc cần để đi đến target trong 1.5s
			vy = 0.12f;
		}
		break;
	}

	case PHANTOMBAT_PROCESS_START_2: // đi cong xuống ngay cửa sổ
	{
		if (!isWaiting)
		{
			// tạo độ cong
			vy -= 0.0001f * dt;
			if (vy < 0)
				vy = 0;

			if (x >= xTarget) // di chuyển xong đến mục tiêu 2
			{
				vx = 0;
				vy = 0;


				isWaiting = true; // bật trạng thái chờ
				TimeWaited = 0; // reset lại time đã chờ
			}
		}
		else
		{
			TimeWaited += dt;
			if (TimeWaited >= (UINT)(2000 + rand() % 1500))
			{
				isWaiting = false; // ngừng chờ

				StartCurves();
			}
		}


		break;
	}

	case PHANTOMBAT_PROCESS_CURVES:
	{
		if (abs(x - xBefore) >= abs(xTarget - xBefore)) // đi xong thi đi thẳng
		{
			vx = 0;
			vy = 0;
			isUseBezierCurves = false;

			StartStaight();

			break;
		}


		float perc = (x - xBefore) / (xTarget - xBefore); // sử dụng phần trăm đã đi được tương ứng t của Bézier curve

		float ya = getPt(y1, y2, perc);
		float yb = getPt(y2, y3, perc);

		float yy = getPt(ya, yb, perc);

		vy = (yy - yLastFrame/*Khoảng cách y giữa frame trước và y dự tính đi*/) / dt;


		break;
	}

	case PHANTOMBAT_PROCESS_STRAIGHT_1:
	{
		if (abs(x - xBefore) >= abs(xTarget - xBefore) ||
			abs(y - yBefore) >= abs(yTarget - yBefore)) // đi xong thì đi thẳng theo hướng random
		{
			vx = vy = 0;

			StartStaight();

		}
		break;
	}

	case PHANTOMBAT_PROCESS_STRAIGHT_2:
	{
		if (!isWaiting)
		{
			if (abs(x - xBefore) >= abs(xTarget - xBefore) ||
				abs(y - yBefore) >= abs(yTarget - yBefore)) // đi xongthì dừng
			{
				vx = vy = 0;


				isWaiting = true; // bật trạng thái chờ
				TimeWaited = 0; // reset lại time đã chờ
			}
		}
		else
		{
			TimeWaited += dt;
			if (TimeWaited >= 3000)
			{
				isWaiting = false; // ngừng chờ

				if (rand() % 2 == 0)
				{
					StartCurves();
				}
			}
			else
			{
				ProcessSmart();
			}
		}


		break;
	}

	default:
		break;
	}

	CGameObject::Update(dt);
	x += dx;
	y += dy;

	float xCam = Camera::GetInstance()->Getx();
	float yCam = Camera::GetInstance()->Gety();
	int widthCam = Camera::GetInstance()->GetScreenWidth();
	int heightCam = Camera::GetInstance()->GetScreenHeight();

	if (x < xCam
		|| xCam + widthCam <x + PHANTOMBAT_FRAMEWIDTH
		|| y < yCam
		|| yCam + heightCam < y + PHANTOMBAT_FRAMEHEIGHT

		) // ra khỏi cam thì xử lí hướng tiếp theo
	{
		

		switch (StatusProcessing)
		{
		case PHANTOMBAT_PROCESS_CURVES:
		{
			isUseBezierCurves = false;
			StartStaight();
			break;
		}

		case PHANTOMBAT_PROCESS_STRAIGHT_1:
		{
			StartStaight();
			break;
		}

		case PHANTOMBAT_PROCESS_STRAIGHT_2:
		{
			int random = rand() % 3;
			switch (random)
			{
			case 0: //33 %
				//
				break;

			default: // 66%
				StartCurves();

				break;
			}

			break;
		}

		}

	}

	yLastFrame = y;// lưu lại y frame hiện tại
}

void PhantomBat::Render()
{
	if (GetHealth() <= 0)
		return;

	if (StatusProcessing == PHANTOMBAT_PROCESS_SLEEP)
		animation_set->at(PHANTOMBAT_PROCESS_SLEEP)->Render(x, y);
	else
		animation_set->at(PHANTOMBAT_PROCESS_START_1)->Render(x, y);
}

float PhantomBat::getPt(float n1, float n2, float perc)
{
	float diff = n2 - n1;

	return n1 + (diff * perc);
}

void PhantomBat::ProcessSmart()
{
	if (simon->isJumping && sqrt((simon->GetX() - x)*(simon->GetX() - x) + (simon->GetY() - y)*(simon->GetY() - y)) <= 150.0f) // nếu nhảy lên & khoảng cách nhỏ hơn 150 thì random cách xử lí
	{
		int random = rand() % 6;
		switch (random)
		{
		case 0:
			StartStaight();
			return;
		case 1:
			StartCurves();
			return;
		default:
			break;
		}
	}

	if (rand() % 5 == 0)
	{
		if (health <= 10 && simon->isAttacking)
		{
			StartCurves();
		}
	}

}

void PhantomBat::Start()
{
	StatusProcessing = PHANTOMBAT_PROCESS_START_1;
	yBefore = y;
	vy = 0.05f;
	vx = 0.0f;
	yTarget = y + 40; // lúc đầu thì đi xuống 40px
}

void PhantomBat::StartCurves()
{
	xBefore = x;
	yBefore = y;

	x1 = x;
	y1 = y;

	x2 = simon->GetX();
	y2 = simon->GetY() + SIMON_BBOX_HEIGHT;

	float xCam = Camera::GetInstance()->Getx();
	float yCam = Camera::GetInstance()->Gety();
	int widthCam = Camera::GetInstance()->GetScreenWidth();
	int heightCam = Camera::GetInstance()->GetScreenHeight();

	if (simon->GetX() < x) // simon bên trái boss
		xTarget = xCam - 100 + rand() % ((int)(simon->GetX() - xCam + 100)); // -100 để đảm bảo simon đứng sát cạnh trái cam vẫn bị boss sát thương
	else // simon bên phải boss
		xTarget = simon->GetX() + SIMON_BBOX_WIDTH;

	yTarget = simon->GetY() - 40; // điểm p3(là y3) trong bezier cao hơn xíu so với p2 (là y2) để lượng vòng chữ u

	x3 = xTarget;
	y3 = yTarget;

	vx = -(x - xTarget) / (abs(xTarget - xBefore)*1000.0f / 150); // vận tốc cần để đi đến Target // quy ước: cứ 1 giây đi 150px
	vy = 0;

	isUseBezierCurves = true;
	StatusProcessing = PHANTOMBAT_PROCESS_CURVES;
}

void PhantomBat::StartStaight()
{
	switch (StatusProcessing)
	{
	case PHANTOMBAT_PROCESS_STRAIGHT_1:
		StatusProcessing = PHANTOMBAT_PROCESS_STRAIGHT_2;
		break;
	default:
		StatusProcessing = PHANTOMBAT_PROCESS_STRAIGHT_1;
		break;
	}
	xBefore = x;
	yBefore = y;
	xTarget = leftBoundary + rand() % (int)(rightBoundary - leftBoundary);
	yTarget = 80.0f + rand() % (190 - 80); // khoảng 80 190


	vx = (xTarget - xBefore) / (1000); // cho đi 1 giây
	vy = (yTarget - yBefore) / (1000);
}
