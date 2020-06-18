#ifndef STOPWATCHITEM_H
#define STOPWATCHITEM_H

#define STOPWATCH_GRAVITY 0.25f
#define STOPWATCH_TIMEDISPLAYMAX 3000
#define STOPWATCH_TIMEWAITMAX 300
#define STOPWATCH_FRAMEWIDTH 32
#define STOPWATCH_FRAMEHEIGHT 32


#include "Item.h"
class StopWatchItem : public Item
{
public:
	StopWatchItem(float X, float Y);
	~StopWatchItem();


	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *listObject = NULL);


};

#endif
