#include "ItemManager.h"

ItemManager * ItemManager::__Instance = NULL;


ItemManager * ItemManager::GetInstance()
{
	if (__Instance == NULL)
		__Instance = new ItemManager();
	return __Instance;
}

ItemManager::ItemManager()
{
	ListItem.clear();
	HeartCollected = 0;
}


ItemManager::~ItemManager()
{
}