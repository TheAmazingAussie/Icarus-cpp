/**
* Icarus - A multi-platform C++ server
*
* Copyright 2016 Alex "Quackster" Miller
*
* Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
* (see https://creativecommons.org/licenses/by-nc-sa/4.0/, or LICENSE.txt for a full license
*/
#include "stdafx.h"

#include "UserDao.h"
#include "RoomDao.h"

#include "game/room/misc/RoomNewbie.h"
#include "game/item/definitions/ItemDefinition.h"

#include "boot/Icarus.h"
#include "misc/Utilities.h"

/*
    Get all room models
    
    @return room model ptr instances
*/
std::map<std::string, RoomModel*> RoomDao::getModels() {

    std::map<std::string, RoomModel*> models;// = new std::map<std::string, RoomModel*>();
    std::shared_ptr<MySQLConnection> connection = Icarus::getDatabaseManager()->getConnectionPool()->borrow();

    try {

        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;
        std::shared_ptr<sql::PreparedStatement> statement = std::shared_ptr<sql::PreparedStatement>(sql_connection->prepareStatement("SELECT id, heightmap, door_x, door_y, door_z, door_dir FROM room_models "));

        std::shared_ptr<sql::ResultSet> result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery());

        while (result_set->next()) {
            models.insert(std::make_pair(result_set->getString("id"), 
                new RoomModel(
                    result_set->getString("id"), 
                    result_set->getString("heightmap"), 
                    result_set->getInt("door_x"), 
                    result_set->getInt("door_y"), 
                    result_set->getInt("door_z"), 
                    result_set->getInt("door_dir"))));
        }

    }
    catch (sql::SQLException &e) {
        Icarus::getDatabaseManager()->printException(e, __FILE__, __FUNCTION__, __LINE__);
    }

    Icarus::getDatabaseManager()->getConnectionPool()->unborrow(connection);

    return models;

}

/*
    Set all current user count back to zero

    @return none
*/
void RoomDao::resetRooms() {

    std::map<std::string, RoomModel*> models;// = new std::map<std::string, RoomModel*>();
    std::shared_ptr<MySQLConnection> connection = Icarus::getDatabaseManager()->getConnectionPool()->borrow();

    try {

        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;
        std::shared_ptr<sql::PreparedStatement> statement = std::shared_ptr<sql::PreparedStatement>(sql_connection->prepareStatement("UPDATE rooms SET users_now = 0 WHERE users_now > 0"));
        statement->execute();

    }
    catch (sql::SQLException &e) {
        Icarus::getDatabaseManager()->printException(e, __FILE__, __FUNCTION__, __LINE__);
    }

    Icarus::getDatabaseManager()->getConnectionPool()->unborrow(connection);
}

/*
Get all room models

@return room model ptr instances
*/
std::vector<RoomNewbie*> RoomDao::getNewbieRoomSelection() {

    std::vector<RoomNewbie*> newbie_rooms;// = new std::map<std::string, RoomModel*>();

    if (!Icarus::getGameConfiguration()->getBool("newuser.create.newbie.room")) {
        return newbie_rooms; // Don't bother running through with caching this data
    }

    std::shared_ptr<MySQLConnection> connection = Icarus::getDatabaseManager()->getConnectionPool()->borrow();

    try {

        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;
        std::shared_ptr<sql::PreparedStatement> statement = std::shared_ptr<sql::PreparedStatement>(sql_connection->prepareStatement("SELECT id, model, wallpaper, floor, items FROM rooms_newbie"));

        std::shared_ptr<sql::ResultSet> result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery());

        while (result_set->next()) {

            RoomNewbie *room_newbie = new RoomNewbie();
            room_newbie->model = result_set->getString("model");
            room_newbie->wallpaper = result_set->getString("wallpaper");
            room_newbie->floorpaper = result_set->getString("floor");

            std::string items_datas = result_set->getString("items");

            if (items_datas.length() > 0) {

                for (std::string item_data : Utilities::split(items_datas, '|')) {

                    bool floor_item = true;

                    if (Utilities::contains(item_data, " ")) {
                        floor_item = false; // windowed item
                    }

                    std::vector<std::string> data = Utilities::split(item_data, ';');

                    RoomNewbieItem newbie_item;

                    newbie_item.item_id = stoi(data[0]);
                    newbie_item.definition = Icarus::getGame()->getItemManager()->getDefinitionByID(newbie_item.item_id);

                    if (floor_item) {
                        newbie_item.x = stoi(data[1]);
                        newbie_item.y = stoi(data[2]);
                        newbie_item.rotation = stoi(data[3]);

                    }
                    else {
                        newbie_item.position = data[1];
                    }

                    room_newbie->items.push_back(newbie_item);
                }

            }

            newbie_rooms.push_back(room_newbie);

        }

    }
    catch (sql::SQLException &e) {
        Icarus::getDatabaseManager()->printException(e, __FILE__, __FUNCTION__, __LINE__);
    }

    Icarus::getDatabaseManager()->getConnectionPool()->unborrow(connection);

    return newbie_rooms;

}

/*
    Load public rooms

    @return none
*/
void RoomDao::addPublicRooms() {

    std::shared_ptr<MySQLConnection> connection = Icarus::getDatabaseManager()->getConnectionPool()->borrow();

    try {

        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;
        std::shared_ptr<sql::PreparedStatement> statement = std::shared_ptr<sql::PreparedStatement>(sql_connection->prepareStatement("SELECT id FROM rooms WHERE room_type = 1 "));

        std::shared_ptr<sql::ResultSet> result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery());

        while (result_set->next()) {
            Room *room = getRoom(result_set->getInt("id"));
            Icarus::getGame()->getRoomManager()->getPublicRooms().push_back(room);
            Icarus::getGame()->getRoomManager()->getRooms().insert(std::make_pair(room->id, room));

            room->getData()->private_room = false; // If this is set to true, then when a user leaves the public room would be disposed/removed from server memory...
        }

    }
    catch (sql::SQLException &e) {
        Icarus::getDatabaseManager()->printException(e, __FILE__, __FUNCTION__, __LINE__);
    }

    Icarus::getDatabaseManager()->getConnectionPool()->unborrow(connection);

}

/*
    Get list of room ids that the player owns

    @param room id
    @return vector of room ids
*/
std::vector<int> RoomDao::getPlayerRooms(int user_id) {

    std::vector<int> rooms;

    std::shared_ptr<MySQLConnection> connection = Icarus::getDatabaseManager()->getConnectionPool()->borrow();

    try {

        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;
        std::shared_ptr<sql::PreparedStatement> statement = std::shared_ptr<sql::PreparedStatement>(sql_connection->prepareStatement("SELECT id FROM rooms WHERE owner_id = ? "));
        statement->setInt(1, user_id);

        std::shared_ptr<sql::ResultSet> result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery());

        while (result_set->next()) {
            rooms.push_back(result_set->getInt("id"));
        }

    }
    catch (sql::SQLException &e) {
        Icarus::getDatabaseManager()->printException(e, __FILE__, __FUNCTION__, __LINE__);
    }

    Icarus::getDatabaseManager()->getConnectionPool()->unborrow(connection);

    return rooms;
}

/*
    Get a single room by room id

    @param room id
    @param room ptr instance
*/
Room* RoomDao::getRoom(int room_id) {

    // Convert room id to a vector if we want to get one room
    std::vector<int> rooms = { room_id };

    // Return single room
    return getRooms(rooms)[0];
}


/*
    Get list of room instances by list of room ids

    @param vector room ids
    @return room ptr instances
*/
std::vector<Room*> RoomDao::getRooms(std::vector<int> room_ids) {

    std::vector<Room*> rooms;
    std::shared_ptr<MySQLConnection> connection = Icarus::getDatabaseManager()->getConnectionPool()->borrow();

    try {

        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;

        for (int room_id : room_ids) {

            std::shared_ptr<sql::PreparedStatement> statement = std::shared_ptr<sql::PreparedStatement>(sql_connection->prepareStatement("SELECT id, name, room_type, thumbnail, owner_id, group_id, description, password, users_max, model, wallpaper, floor, outside, tags, trade_state, state, score, category, allow_pets, allow_pets_eat, allow_walkthrough, hidewall, wall_thickness, floor_thickness, chat_mode, chat_size, chat_speed, chat_flood, chat_distance, who_can_mute, who_can_kick, who_can_ban FROM rooms WHERE id = ? ")); {
                statement->setInt(1, room_id);
            }

            std::shared_ptr<sql::ResultSet> result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery());

            while (result_set->next()) {

                // These pointers get deleted when either
                // 
                // The user leaves the room and they're not the owner, and they're the last one in the room
                // The user disconnects and there's no one else in the room so they get deleted
                //
                // 'delete' called from the .deleteRoom function from RoomManage
                RoomData *room_data = new RoomData();
                Room *room = new Room(room_id, room_data);

                room_data->id = room_id;
                room_data->name = result_set->getString("name");
                room_data->room_type = (char)result_set->getInt("room_type");
                room_data->thumbnail = result_set->getString("thumbnail");
                room_data->owner_id = result_set->getInt("owner_id");
                room_data->owner_name = UserDao::getName(result_set->getInt("owner_id"));
                room_data->group_id = result_set->getInt("group_id");
                room_data->description = result_set->getString("description");
                room_data->password = result_set->getString("password");
                room_data->users_max = result_set->getInt("users_max");
                room_data->model = Icarus::getGame()->getRoomManager()->getModel(result_set->getString("model"));
                room_data->wallpaper = result_set->getString("wallpaper");
                room_data->floor = result_set->getString("floor");
                room_data->outside = result_set->getString("outside");
                room_data->tags = Utilities::split(result_set->getString("tags"), ',');
                room_data->trade_state = result_set->getInt("trade_state");
                room_data->state = result_set->getInt("state");
                room_data->score = result_set->getInt("score");
                room_data->category = result_set->getInt("category");
                room_data->allow_pets = result_set->getBoolean("allow_pets");
                room_data->allow_pets_eat = result_set->getBoolean("allow_pets_eat");
                room_data->allow_walkthrough = result_set->getBoolean("allow_walkthrough");
                room_data->hide_wall = result_set->getBoolean("hidewall");
                room_data->wall_thickness = result_set->getInt("wall_thickness");
                room_data->floor_thickness = result_set->getInt("floor_thickness");
                room_data->chat_mode = result_set->getInt("chat_mode");
                room_data->chat_size = result_set->getInt("chat_size");
                room_data->chat_speed = result_set->getInt("chat_speed");
                room_data->chat_flood = result_set->getInt("chat_flood");
                room_data->chat_distance = result_set->getInt("chat_distance");
                room_data->who_can_mute = result_set->getInt("who_can_mute");
                room_data->who_can_kick = result_set->getInt("who_can_kick");
                room_data->who_can_ban = result_set->getInt("who_can_ban");
                room_data->user_rights = getRights(room->getData()->id);
                room_data->user_rights.push_back(room_data->owner_id);

             

                rooms.push_back(room);
            }
        }

    }
    catch (sql::SQLException &e) {
        Icarus::getDatabaseManager()->printException(e, __FILE__, __FUNCTION__, __LINE__);
    }

    Icarus::getDatabaseManager()->getConnectionPool()->unborrow(connection);

    return rooms;
}

/*
    Get list of room ids that the player owns

    @param room id
    @return vector of room ids
*/
std::vector<int> RoomDao::getRights(int room_id) {

    std::vector<int> rooms;

    std::shared_ptr<MySQLConnection> connection = Icarus::getDatabaseManager()->getConnectionPool()->borrow();

    try {

        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;
        std::shared_ptr<sql::PreparedStatement> statement = std::shared_ptr<sql::PreparedStatement>(sql_connection->prepareStatement("SELECT user_id FROM room_rights WHERE room_id = ? "));
        statement->setInt(1, room_id);

        std::shared_ptr<sql::ResultSet> result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery());

        while (result_set->next()) {
            rooms.push_back(result_set->getInt("user_id"));
        }

    }
    catch (sql::SQLException &e) {
        Icarus::getDatabaseManager()->printException(e, __FILE__, __FUNCTION__, __LINE__);
    }

    Icarus::getDatabaseManager()->getConnectionPool()->unborrow(connection);

    return rooms;
}

/*
    Delete room from database entry

    @param room id
    @return none
*/
void RoomDao::deleteRoom(int room_id) {

    std::shared_ptr<MySQLConnection> connection = Icarus::getDatabaseManager()->getConnectionPool()->borrow();

    try {

        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;
        std::shared_ptr<sql::PreparedStatement> statement = std::shared_ptr<sql::PreparedStatement>(sql_connection->prepareStatement("DELETE FROM rooms WHERE id = ? "));
        statement->setInt(1, room_id);
        statement->execute();

    }
    catch (sql::SQLException &e) {
        Icarus::getDatabaseManager()->printException(e, __FILE__, __FUNCTION__, __LINE__);
    }

    Icarus::getDatabaseManager()->getConnectionPool()->unborrow(connection);
}

/*
    Updates the entire room details

    @return none
*/
void RoomDao::updateRoom(int room_id, Room *room) {

    std::shared_ptr<MySQLConnection> connection = Icarus::getDatabaseManager()->getConnectionPool()->borrow();

    try {

        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;
        std::shared_ptr<sql::PreparedStatement> statement = std::shared_ptr<sql::PreparedStatement>(sql_connection->prepareStatement("UPDATE rooms SET name = ?, description = ?, state = ?, users_max = ?, users_now = ?, category = ?, tags = ?, trade_state = ?, allow_pets = ?, allow_pets_eat = ?, allow_walkthrough = ?, hidewall = ?, wall_thickness = ?, floor_thickness = ?, who_can_mute = ?, who_can_ban = ?, who_can_kick = ?, chat_mode = ?, chat_size = ?, chat_speed = ?, chat_distance = ?, chat_flood = ?, password = ?, wallpaper = ?, floor = ?, outside = ? WHERE id = ?")); {

            RoomData *room_data = room->getData();

            statement->setString(1, room_data->name);
            statement->setString(2, room_data->description);
            statement->setInt(3, room_data->state);
            statement->setInt(4, room_data->users_max);
            statement->setInt(5, room_data->users_now);
            statement->setInt(6, room_data->category);
            statement->setString(7, Utilities::join(room_data->tags, ","));
            statement->setInt(8, room_data->trade_state);
            statement->setInt(9, room_data->allow_pets);
            statement->setInt(10, room_data->allow_pets_eat);
            statement->setInt(11, room_data->allow_walkthrough);
            statement->setInt(12, room_data->hide_wall);
            statement->setInt(13, room_data->wall_thickness);
            statement->setInt(14, room_data->floor_thickness);
            statement->setInt(15, room_data->who_can_mute);
            statement->setInt(16, room_data->who_can_kick);
            statement->setInt(17, room_data->who_can_ban);
            statement->setInt(18, room_data->chat_mode);
            statement->setInt(19, room_data->chat_size);
            statement->setInt(20, room_data->chat_speed);
            statement->setInt(21, room_data->chat_distance);
            statement->setInt(22, room_data->chat_flood);
            statement->setString(23, room_data->password);
            statement->setString(24, room_data->wallpaper);
            statement->setString(25, room_data->floor);
            statement->setString(26, room_data->outside);
            statement->setInt(27, room_data->id);
        }

        statement->execute();
    }
    catch (sql::SQLException &e) {
        Icarus::getDatabaseManager()->printException(e, __FILE__, __FUNCTION__, __LINE__);
    }

    Icarus::getDatabaseManager()->getConnectionPool()->unborrow(connection);
}