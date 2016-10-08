#pragma once
#include <map>

#include "game/room/Room.h"

class RoomManager
{
public:
    RoomManager();
    ~RoomManager();
    void createPlayerRooms(int user_id);
    std::vector<Room*> getPlayerRooms(int user_id);

    bool hasRoom(int room_id);
    Room *getRoom(int room_id);
    void addRoom(Room *room);
    void deleteRoom(int room_id);

private:
    std::map<int, Room*> *rooms;
};

