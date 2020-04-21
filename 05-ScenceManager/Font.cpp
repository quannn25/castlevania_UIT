#include "Font.h"

Font::Font()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(FONT_ANNI_SET_ID);

	SetAnimationSet(ani_set);
}


Font::~Font()
{
}

void Font::Draw(float x, float y, string s)
{
	//for (int i = 0; i < s.size(); i++)
	//{
	//	if (s[i] == ' ')
	//		continue;
	//	if (s[i] >= 'A' && s[i] <= 'Z')
	//	{
	//		_sprite->SelectIndex(s[i] - 'A');
	//	}
	//	else
	//		if (s[i] >= '0' && s[i] <= '9')
	//		{
	//			_sprite->SelectIndex(s[i] - '0' + 26);
	//		}
	//		else
	//			_sprite->SelectIndex(36);
	//	_sprite->Draw(x + i * _texture->FrameWidth, y);
	//}
	animation_set->at(16)->Render(x, y);
	animation_set->at(15)->Render(x+15, y);
}