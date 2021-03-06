/**
* Icarus - A multi-platform C++ server
*
* Copyright 2016 Alex "Quackster" Miller
*
* Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
* (see https://creativecommons.org/licenses/by-nc-sa/4.0/, or LICENSE.txt for a full license
*/
#pragma once

#include "dao/MessengerDao.h"
#include "communication/incoming/MessageEvent.h"
#include "communication/outgoing/messenger/MessengerSearchMessageComposer.h"

class MessengerSearchMessageEvent : public MessageEvent {

public:
    MessengerSearchMessageEvent() { }

    void handle(Player *player, Request &request) {

        std::string searchQuery = request.readString();

        if (searchQuery.length() < 0) {
            return;
        }

        Messenger *messenger = player->getMessenger();

        if (messenger == nullptr) {
            return;
        }

        if (!messenger->initialised) {
            return;
        }

        player->send(MessengerSearchMessageComposer(player, MessengerDao::search(searchQuery)));

    }
};