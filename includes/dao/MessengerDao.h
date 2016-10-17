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
#include <string>

#include "game/messenger/MessengerUser.h"

#include "mysql_connection.h"
#include "mysql.h"

class MessengerDao
{

private:
    MessengerDao() { };

public:
    static std::vector<MessengerUser*> *getFriends(int user_id);
    static std::vector<MessengerUser*> *getRequests(int user_id);
    static std::vector<int> search(std::string query);
    static bool newRequest(int fromId, int toId);
    static bool removeRequest(int fromId, int toId);
    static bool newFriend(int sender, int receiver);
};

