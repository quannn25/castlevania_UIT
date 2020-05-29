#include "Font.h"

Font::Font()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(FONT_ANI_SET_ID);

	SetAnimationSet(ani_set);
}


Font::~Font()
{
}

void Font::Draw(float x, float y, string s)
{
	int a;
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == ' ')
			continue;
		if (s[i] >= 'A' && s[i] <= 'Z')
		{
			a = s[i] - 'A'; //  A = 95, C = 97, sC - 'A' = 2 thì chọn thứ tự là 2 trong ani_set chính là C
		}
		else
			if (s[i] >= '0' && s[i] <= '9')
			{
				a = s[i] - '0' + 26;// 26 chữ cái
			}
			else
				a = 36;
		animation_set->at(a)->Render(x + i * FONT_FRAMEWIDTH, y);
	}
}