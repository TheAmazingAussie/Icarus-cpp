/**
* Icarus - A multi-platform C++ server
*
* Copyright 2016 Alex "Quackster" Miller
*
* Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
* (see https://creativecommons.org/licenses/by-nc-sa/4.0/, or LICENSE.txt for a full license
*/
#pragma once

#include "boot/Icarus.h"
#include "game/room/model/RoomModel.h"

#include "game/room/Room.h"
#include "game/player/Player.h"

#include "communication/outgoing/MessageComposer.h"

class HeightMapMessageComposer : public MessageComposer {

public:
    HeightMapMessageComposer(Room *room) :
        room(room) { }

    const Response compose() const {

        RoomModel *model = room->getModel();

        Response response = this->createResponse();

        response.writeInt(model->getMapSizeX());
        response.writeInt(model->getMapSizeX() * model->getMapSizeY());

        for (int y = 0; y < model->getMapSizeY(); y++) {
            for (int x = 0; x < model->getMapSizeX(); x++) {
                response.writeShort(model->getSquareHeight()[x * model->getMapSizeY() + y] * 256);
            }
        }

        return response;
    }

    const int getHeader() const {
        return Outgoing::HeightMapMessageComposer;
    }

private:
    Room *room;

};