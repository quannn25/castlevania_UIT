#include "DaggerItem.h"



DaggerItem::DaggerItem(float x, float y)
{
	//_texture = new GTexture("Resources\\item\\4.png");
	//_sprite = new GSprite(_texture, 0);
	//type = eID::DaggerItem;

	//this->x = X;
	//this->y = Y;
	//vy = ITEMDAGGER_GRAVITY;
	//TimeDisplayMax = ITEMDAGGER_TIMEDISPLAYMAX;
	//TimeDisplayed = 0;
}

void DaggerItem::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	//left = x;
	//top = y;
	//right = x + _texture->FrameWidth;
	//bottom = y + _texture->FrameHeight /*- 18*/;
}

void DaggerItem::Update(DWORD dt, vector<LPGAMEOBJECT> *listObject)
{
	TimeDisplayed += dt;
	if (TimeDisplayed >= TimeDisplayMax)
	{
		isFinish = true;
		return;
	}

	Item::Update(dt);


	vector<LPGAMEOBJECT> listObject_Brick;
	listObject_Brick.clear();
	for (UINT i = 0; i < listObject->size(); i++)
	{
		if (dynamic_cast<CBrick*>(listObject->at(i)))
			listObject_Brick.push_back(listObject->at(i));
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
	{
		delete coEvents[i];
	}

}




DaggerItem::~DaggerItem()
{
}