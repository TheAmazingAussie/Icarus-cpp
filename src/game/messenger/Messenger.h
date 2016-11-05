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

class MessengerUser;
class Entity;
class Messenger {

private:
    bool initialised;
    int user_id;
    const Entity *player;

    std::map<int, MessengerUser*> friends;
    std::map<int, MessengerUser*> requests;

public:
    Messenger(Entity *player, int user_id, std::map<int, MessengerUser*> friends, std::map<int, MessengerUser*> requests);
    ~Messenger();
    MessengerUser *getRequest(int user_id);
    MessengerUser *getFriend(int user_id);
    bool hasRequest(int id);
    bool isFriend(int id);
    void removeRequest(int user_id);
    void removeFriend(int user_id);
    void sendStatus(bool force_offline, bool login_status = false);
    void send(const MessageComposer &composer);

    std::map<int, MessengerUser*> getFriends() {
        return friends;
    }

    std::map<int, MessengerUser*> getRequests() {
        return requests;
    }

    void clearFriends();
    void clearRequests();

    bool isInitialised() { return this->initialised; }
    void setInitialised(bool flag) { this->initialised = flag; }
};