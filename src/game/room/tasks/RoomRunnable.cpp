/**
* Icarus - A multi-platform C++ server
*
* Copyright 2016 Alex "Quackster" Miller
*
* Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
* (see https://creativecommons.org/licenses/by-nc-sa/4.0/, or LICENSE.txt for a full license
*/
#include "stdafx.h"

#include <thread>
#include <chrono>
#include <sstream>

#include "boot/Icarus.h"

#include "communication/outgoing/room/user/UserStatusMessageComposer.h"
#include "communication/outgoing/room/user/IdleStatusMessageComposer.h"

#include "game/room/model/Rotation.h"
#include "game/room/tasks/RoomRunnable.h"

/*
    Constructor for room runnable
*/
RoomRunnable::RoomRunnable(Room *room) : 
    room(room),
    tick(0) { }

/*
Tick handler for room runnable

@return none
*/
void RoomRunnable::run() {

    try {
        if (this->room == nullptr ||
            this->room->isDisposed() ||
            this->room->getEntities().size() == 0) {
            this->room->setRunnable(nullptr);
            return;
        }

        std::vector<Entity*> update_entities;

        RoomModel *room_model = this->room->getModel();

        mtx.lock(); // Lock entities thread

        std::map<int, Entity*> entities = this->room->getEntities();

        for (int i = 0; i < entities.size(); i++) {

            Entity *entity = entities[i];

            if (entity != nullptr) {
                if (entity->getRoomUser() != nullptr) {

                    this->processEntity(entity);

                    RoomUser *room_user = entity->getRoomUser();

                    if (room_user->needs_update) {
                        update_entities.push_back(entity);
                    }

                    if (this->hasTickedSecond()) {
                        if (entity->getEntityType() == PLAYER) {

                            int afk_time = room_user->afk_time + 1;
                            room_user->afk_time = afk_time;

                            if (afk_time > Icarus::getGameConfiguration()->getInt("room.idle.seconds")) {
                                if (!room_user->is_asleep) {
                                    this->room->send(IdleStatusMessageComposer(room_user->virtual_id, true));
                                    room_user->is_asleep = true;
                                }
                            }
                        }
                    }
                }
            }
        }

        mtx.unlock(); // Unlock entities thread

        this->room->send(UserStatusMessageComposer(update_entities));

        if (room->getPlayers().size() > 0 && this->room->getRunnable() != nullptr) {
            Icarus::getGame()->getGameScheduler()->schedule(this->room->getRunnable());
        }
        else {
            this->room->setRunnable(nullptr);
        }

        tick++;
    }
    catch (std::exception& e) {
        cout << endl << " Error occurred in room runnable: " << e.what() << endl;
    }
    catch (...) {
        cout << endl << " Error occurred in room runnable... " << endl;
    }
}

void RoomRunnable::processEntity(Entity *entity) {

    RoomUser *room_user = entity->getRoomUser();

    if (Icarus::getUnixTimestamp() > room_user->sign_time) {
        if (room_user->containsStatus("sign")) {
            room_user->setStatus("sign", "");
            room_user->needs_update = true;
        }
    }

    if (room_user->is_walking) {
        if (room_user->path.size() > 0) {

            Position next = room_user->path.front();
            room_user->path.pop_front();

            room_user->setStatus("lay", "");
            room_user->setStatus("sit", "");

            int rotation = Rotation::getRotation(room_user->position.x, room_user->position.y, next.x, next.y);
            int height = this->room->getModel()->getSquareHeight(next.x, next.y);

            room_user->setRotation(rotation, true, false);

            std::stringstream ss;
            ss << next.x;
            ss << ",";
            ss << next.y;
            ss << ",";
            ss << height;

            room_user->setStatus("mv", ss.str());
            room_user->needs_update = true;
            room_user->next = next;  

        }
        else {
            room_user->next = Position();
            room_user->needs_update = true;
        }

    }
}