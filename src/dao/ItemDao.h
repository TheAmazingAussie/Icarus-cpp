/**
* Icarus - A multi-platform C++ server
*
* Copyright 2016 Alex "Quackster" Miller
*
* Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
* (see https://creativecommons.org/licenses/by-nc-sa/4.0/, or LICENSE.txt for a full license
*/
#pragma once
#include <vector>
#include <map>
#include <string>

#include "game/item/Item.h"
#include "game/item/ItemDefinition.h"

class ItemDao
{

private:
	ItemDao() { };

public:
    static std::map<int, ItemDefinition*> getItemDefinitions();
	static std::vector<Item*> getInventoryItems(int user_id);
	static Item *newItem(int item_id, int owner_id, std::string extra_data);
};

