/**
* Icarus - A multi-platform C++ server
*
* Copyright 2016 Alex "Quackster" Miller
*
* Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
* (see https://creativecommons.org/licenses/by-nc-sa/4.0/, or LICENSE.txt for a full license
*/
#include "stdafx.h"

#include "dao/NavigatorDao.h"

#include "NavigatorManager.h"
#include "populators/DefaultPopulator.h"
#include "populators/MyRoomPopulator.h"
#include "populators/OfficialRoomPopulator.h"
#include "populators/PopularPopulator.h"
#include "populators/FriendsPopulator.h"

/*
    Constructor for Navigator Manager

    Initialises the list for storing navigator tabs
*/
NavigatorManager::NavigatorManager() :
    tabs(NavigatorDao::getTabsByChildId(-1)), 
    categories(NavigatorDao::getCategories()) {

    //this->room_populators = std::map<std::string, RoomPopulator*>();
    this->room_populators.insert(std::make_pair("DefaultPopulator", new DefaultPopulator()));
    this->room_populators.insert(std::make_pair("OfficialRoomPopulator", new OfficialRoomPopulator()));
    this->room_populators.insert(std::make_pair("PopularPopulator", new PopularPopulator()));
    this->room_populators.insert(std::make_pair("MyRoomPopulator", new MyRoomPopulator()));
    this->room_populators.insert(std::make_pair("FriendsPopulator", new FriendsPopulator()));
}
/*
    Gets tab by name

    @param tab name
    @return tab ptr
*/
NavigatorTab* NavigatorManager::getTab(std::string tab_name) {

    for (auto tab : this->tabs) {
        if (tab->getTabName() == tab_name) {
            return tab;
        }
    }

    return nullptr;
}

/*
    Returns all parent tabs, (those who have -1 as child id

    @return vector list of parent tabs
*/
std::vector<NavigatorTab*> NavigatorManager::getParentTabs() {

    std::vector<NavigatorTab*> tabs = std::vector<NavigatorTab*>();

    for (auto tab : this->tabs) {
        if (tab->getChildId() == -1) {
            tabs.push_back(tab);
        }
    }

    return tabs;
}

/*
    Returns category by specified id, will return nullptr if no
    category has been found

    @return navigator category ptr
*/
NavigatorCategory *NavigatorManager::getCategoryById(int category_id) {

    for (auto category : this->categories) {
        if (category->id == category_id) {
            return category;
        }
    }

    return nullptr;
}

/*
    Returns populator by name, if it doesn't exist it defaults to the default populator

    @param populator name
    @return populator ptr
*/
RoomPopulator *NavigatorManager::getPopulator(std::string populator_name) {

    if (this->room_populators.count(populator_name)) {
        return this->room_populators.find(populator_name)->second;
    }

    return this->room_populators.find("DefaultPopulator")->second;
}

/*
Deconstructor for Navigator Manager

Deletes all pointer variables
*/
NavigatorManager::~NavigatorManager() {

    for (auto tab : this->tabs) delete tab;
    for (auto category : this->categories) delete category;
    for (auto populator : this->room_populators) delete populator.second;

    /*delete categories;
    delete tabs;
    delete room_populators;*/
}
