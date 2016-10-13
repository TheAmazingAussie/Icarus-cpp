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
#include "game/room/tasks/RoomRunnable.h"
#include "communication/outgoing/room/UserStatusMessageComposer.h"


int Calculate(int X1, int Y1, int X2, int Y2);

/*
    Constructor for room runnable
*/
RoomRunnable::RoomRunnable(Room *room) : room(room) { }

int Calculate(int X1, int Y1, int X2, int Y2)
{
    int Rotation = 0;

    if (X1 > X2 && Y1 > Y2)
    {
        Rotation = 7;
    }
    else if (X1 < X2 && Y1 < Y2)
    {
        Rotation = 3;
    }
    else if (X1 > X2 && Y1 < Y2)
    {
        Rotation = 5;
    }
    else if (X1 < X2 && Y1 > Y2)
    {
        Rotation = 1;
    }
    else if (X1 > X2)
    {
        Rotation = 6;
    }
    else if (X1 < X2)
    {
        Rotation = 2;
    }
    else if (Y1 < Y2)
    {
        Rotation = 4;
    }
    else if (Y1 > Y2)
    {
        Rotation = 0;
    }

    return Rotation;
}

/*
    Tick handler for room runnable

    @return none
*/
void RoomRunnable::run() {

    if (room->isDisposed()) {
        return;
    }
    RoomModel *room_model = this->room->getData()->getModel();
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
    std::vector<Entity*> entities_update;

    for (Entity *entity : *this->room->getEntities()) {

<<<<<<< HEAD
=======
        printf("tick!!\n");

>>>>>>> refs/remotes/origin/master
        RoomUser *room_user = entity->getRoomUser();
        

        if (room_user->getPosition().sameAs(room_user->getGoal())) {
            room_user->stopWalking(false);
        }

        if (room_user->isWalking()) {

            if (room_user->getPath().size() > 0) {

                Position next = room_user->getPath().front();
<<<<<<< HEAD
                room_user->getPath().pop_front();
=======
                room_user->getPath().pop_front(); // remove front



               /* if (room_user->getStatuses().count("mv") > 0) {
                    room_user->getStatuses().erase("mv"); // remove status
                }

                if (room_user->getStatuses().count("sit") > 0) {
                    room_user->getStatuses().erase("sit"); // remove status
                }

                if (room_user->getStatuses().count("lay") > 0) {
                    room_user->getStatuses().erase("lay"); // remove status
                }*/
>>>>>>> refs/remotes/origin/master

                room_user->setStatus("mv", "");
                room_user->setStatus("lay", "");
                room_user->setStatus("sit", "");

                int height = room_model->getSquareHeight()[next.getX() * room_model->getMapSizeY() + next.getY()];
                room_user->setRotation(Calculate(room_user->getPosition().getX(), room_user->getPosition().getY(), next.getX(), next.getY()), true, false);
                
                std::stringstream ss;
                ss << next.getX();
                ss << ",";
                ss << next.getY();
                ss << ",";
                ss << height;
                std::string status = ss.str();

                room_user->setStatus("mv", status);
                room_user->updateStatus();

<<<<<<< HEAD
                room_user->setX(next.getX());
                room_user->setY(next.getY());
                room_user->setHeight(height);
=======

                           /* # Update collision map
                            if not self.room.data.allow_walkthrough:
                self.room.room_mapping.update_map(room_user.position.x, room_user.position.y, False)
                    self.room.room_mapping.update_map(next.x, next.y, True)*/
                room_user->setX(next.getX());
                room_user->setY(next.getY());
                room_user->setHeight(height);

                    //room_user.position.z = height               
                
                //printf("virtual id: %i\n", room_user->getStatuses().size());


>>>>>>> refs/remotes/origin/master
            }
            else {
                room_user->stopWalking(true);
            }

        }
        else if (room_user->getNeedsUpdate()) {
            room_user->stopWalking(false);
            entities_update.push_back(entity);
        }
    }

    if (entities_update.size() > 0) {
        this->room->send(UserStatusMessageComposer(entities_update));
    }

    if (this->room->getPlayers().size() > 0) {
        this->room->scheduleRunnable(); // reschedule again!!
    }
}