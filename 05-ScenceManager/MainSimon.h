#ifndef MAINSIMON_H
#define	MAINSIMON_H

#include "Simon.h"

class MainSimon
{
private:
	static MainSimon * __instance; // mục đích tránh việc suy nghĩ Simon là duy nhất, có thể có item ăn vào phân thân chẳng hạn, thì có thể có 3 Obj Simon cùng 1 Scene, trong đó chỉ có 1 Simon là Player

	Simon * simon;
public:
	MainSimon();
	~MainSimon();

	Simon * GetSimon();
	void SetSimon(Simon *s);

	static MainSimon * GetInstance();
};
#endif