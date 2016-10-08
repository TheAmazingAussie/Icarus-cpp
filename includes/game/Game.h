#pragma once
#include "game/navigator/NavigatorManager.h"
#include "game/room/RoomManager.h"
#include "thread/ExecutorService.h"

class Game
{
public:
    Game();
    ~Game();

    NavigatorManager *getNavigatorManager() { return navigator_manager; }
    ExecutorService *getGameScheduler() { return executor_service; }
    RoomManager *getRoomManager() { return room_manager; }

private:
    NavigatorManager *navigator_manager;
    ExecutorService *executor_service;
    RoomManager *room_manager;
};


