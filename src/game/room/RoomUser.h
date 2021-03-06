/**
* Icarus - A multi-platform C++ server
*
* Copyright 2016 Alex "Quackster" Miller
*
* Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
* (see https://creativecommons.org/licenses/by-nc-sa/4.0/, or LICENSE.txt for a full license
*/
#pragma once
#include <map>
#include <deque>

#include "game/pathfinder/Position.h"

class Entity;
class Room; // Defined elsewhere
class Item;
class RoomUser
{

public:
    RoomUser(Entity *entity);
    ~RoomUser();
    
public:

    void reset();
    void walk();
    void stopWalking();
    void currentItemTrigger();
    bool containsStatus(std::string key);
    void setStatus(std::string key, std::string value, bool update = false);
    void updateStatus();
    void chat(std::string message, int bubble, int count, bool shout = false, bool spam_check = true);
    void leaveRoom(bool hotel_view = true);
    void awake();

    void setRotation(int rotation, bool set_head_rotation = false, bool update = false);

    int virtual_id;
    int last_chat_id;
    int dance_id;
    int rotation;
    int head_rotation;
    int sign_time;
    int afk_time;
    int chat_flood_timer;
    int chat_count;

    double height;
    Entity *entity;
    Item *current_item;

    Position next;
    Position position;
    Position goal;

    bool is_walking;
    bool needs_update;
    bool is_loading_room;
    bool is_asleep;
    bool in_room;

    std::map<std::string, std::string> &getStatuses() {
        return this->statuses;
    }

    std::deque<Position> &getPath() {
        return this->path;
    }

    void setPath(std::deque<Position> path) {
        this->path = path;
    }

    Room *getRoom() {
        return this->room;
    }

    void setRoom(Room *room) {
        this->room = room;
    }

private:
    std::map<std::string, std::string> statuses;
    std::deque<Position> path;
    Room *room;

};
