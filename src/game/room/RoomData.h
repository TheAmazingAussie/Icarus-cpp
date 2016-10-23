/**
* Icarus - A multi-platform C++ server
*
* Copyright 2016 Alex "Quackster" Miller
*
* Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
* (see https://creativecommons.org/licenses/by-nc-sa/4.0/, or LICENSE.txt for a full license
*/
#pragma once
#include <string>
#include <vector>

#include "model/RoomModel.h"

class Room;
class RoomData
{
public:
    RoomData(int id,
        std::string name,
        char room_type,
        std::string thumbnail,
        int owner_id,
        std::string owner_name,
        int group_id,
        std::string description,
        std::string password,
        int users_max,
        RoomModel *model,
        //std::string model,
        std::string wallpaper,
        std::string floor,
        std::string outside,
        std::vector<std::string> tags,
        int trade_state,
        int state,
        int score,
        int category,
        bool allow_pets,
        bool allow_pets_eat,
        bool allow_walkthrough,
        bool hide_wall,
        int floor_thickness,
        int wall_thickness,
        int chat_type,
        int chat_balloon,
        int chat_speed,
        int chat_max_distance,
        int chat_flood_protection,
        int who_can_mute,
        int who_can_kick,
        int who_can_ban,
        std::vector<int> user_rights);

    int &getId() { return id; }
    std::string &getName() { return name; }
    bool isPrivate() { return room_type == 0; }
    std::string &getThumbnail() { return thumbnail;  }
    int &getOwnerId() { return owner_id; }
    std::string &getOwnerName() { return owner_name; }
    Player *getOwner() { return owner; }
    bool isOwnerOnline();
    int &getGroupId() { return group_id; }
    std::string &getDescription() { return description; }
    std::string &getPassword() { return password; }
    int &getUsersMax() { return users_max; }
    //std::string getModel() { return model; }
    RoomModel *getModel() { return this->model; }
    std::string &getWallpaper() { return wallpaper; }
    std::string &getFloor() { return floor; }
    std::string &getOutside() { return outside; }
    std::vector<std::string> &getTags() { return tags; }
    int &getTradeState() { return trade_state; }
    int &getState() { return state; }
    int &getScore() { return score; }
    int &getCategory() { return category; }
    bool &hasAllowPets() { return allow_pets; }
    int getWallHeight() { return -1; }
    int &getWhoCanMute() { return who_can_mute; }
    int &getWhoCanKick() { return who_can_kick; }
    int &getWhoCanBan() { return who_can_ban; }
    //bool hasMoonwalk() { return moonwalk; }

    int getVirtualId() { this->virtual_id = this->virtual_id + 1; return virtual_id; }

    std::vector<int> &getUserRights() { return user_rights; }

    ~RoomData();

private:
    int id;
    std::string name;
    char room_type;
    std::string thumbnail;
    int owner_id;
    Player *owner;
    std::string owner_name;
    int group_id;
    std::string description;
    std::string password;
    int users_max;
    //std::string model;
    RoomModel *model;
    std::string wallpaper;
    std::string floor;
    std::string outside;
    std::vector<std::string> tags;
    int trade_state;
    int state;
    int score;
    int category;
    bool allow_pets;
    bool allow_pets_eat;
    bool allow_walkthrough;
    int floor_thickness;
    int wall_thickness;
    bool hide_wall;
    int chat_type;
    int chat_balloon;
    int chat_speed;
    int chat_max_distance;
    int chat_flood_protection;
    int who_can_mute;
    int who_can_kick;
    int who_can_ban;
    int virtual_id;

    //bool moonwalk = true;

    std::vector<int> user_rights;
};

