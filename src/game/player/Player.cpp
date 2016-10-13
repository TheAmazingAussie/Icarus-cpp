/**
* Icarus - A multi-platform C++ server
*
* Copyright 2016 Alex "Quackster" Miller
*
* Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
* (see https://creativecommons.org/licenses/by-nc-sa/4.0/, or LICENSE.txt for a full license
*/
#include "stdafx.h"

#include "game/player/Player.h"
#include "game/player/PlayerDetails.h"
#include "boot/Icarus.h"

#include "communication/outgoing/navigator/FlatCategoriesMessageComposer.h"
#include "communication/outgoing/navigator/NavigatorCategoriesComposer.h"
#include "communication/outgoing/navigator/NavigatorMetaDataComposer.h"

/*
    Session constructor

    @param NetworkConnection ptr
    @return session instance
*/
Player::Player(NetworkConnection *network_connection) : 
    network_connection(network_connection), 
    session_details(nullptr), 
    room_user(nullptr) {
    
    std::cout << " [SESSION] Client connected with ID: " << this->getNetworkConnection()->getConnectionId() << std::endl;
}

/*
    Initialise player and load their data

    @return none
*/
void Player::login() {

    // init variables
    this->logged_in = true;
    this->room_user = new RoomUser(this);

    // add user to logged in sessions for quick lookup
    Icarus::getPlayerManager()->getPlayers()->insert(std::make_pair(this->session_details->getId(), this));

    // load player rooms
    Icarus::getGame()->getRoomManager()->createPlayerRooms(this->session_details->getId());

<<<<<<< HEAD
=======
    // New room user instance
    this->room_user = new RoomUser(this);
>>>>>>> refs/remotes/origin/master



}

/*
    Send message composer to the session's socket

    @param MessageComposer ptr
    @return none
*/
void Player::send(MessageComposer &composer) {
    this->network_connection->send(composer);
}

/*
    Clear session of any responsibilities

    @return none
*/
void Player::clear() {
     
    // Can't continue clearing if session details is null...
    if (session_details == nullptr) {
        return;
    }

    if (this->logged_in) {

        if (this->room_user != nullptr) {
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
            if (this->room_user->getRoom() != nullptr) {
                this->room_user->getRoom()->leave(this, false, true);
            }
        }

        std::vector<Room*> rooms = Icarus::getGame()->getRoomManager()->getPlayerRooms(this->session_details->getId()); {
            for (Room *room : rooms) {
                room->dispose();
            }
        }

        Icarus::getPlayerManager()->getPlayers()->erase(this->session_details->getId());
    }
}

/*
    Session deconstructor, called when session disconnects, 
    is deleted from SessionManager class

    networkManager->connectionState has already been set to false 
    so there won't be any more packet receiving
*/
Player::~Player() {
    std::cout << " [SESSION] Client disconnected with ID: " << this->getNetworkConnection()->getConnectionId() << std::endl;

    if (!logged_in) {
        return;
    }

    if (session_details != nullptr) {
        delete session_details;
    }

    if (room_user != nullptr) {
        delete room_user;
    }
}
