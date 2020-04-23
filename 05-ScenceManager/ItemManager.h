#ifndef itemM_h
#define itemM_h

#include "define.h"
#include "Item.h"

class ItemManager
{
public:
	static ItemManager * __Instance;
	static ItemManager * GetInstance();

	vector<Item*> ListItem;
	int HeartCollected;

	ItemManager();
	~ItemManager();
};

#endif