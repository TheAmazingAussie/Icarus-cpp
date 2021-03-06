/**
* Icarus - A multi-platform C++ server
*
* Copyright 2016 Alex "Quackster" Miller
*
* Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
* (see https://creativecommons.org/licenses/by-nc-sa/4.0/, or LICENSE.txt for a full license
*/
#include "stdafx.h"

#include <boost/algorithm/string.hpp>

#include <sstream>

#include "boot/Icarus.h"
#include "misc/Utilities.h"
#include "game/room/model/RoomModel.h"

/*
Constructor for RoomModel
*/
RoomModel::RoomModel(std::string name, std::string height_map, int door_x, int door_y, int door_z, int door_rotation) : 
    name(name), 
    height_map(height_map), 
    door_x(door_x), 
    door_y(door_y), 
    door_z(door_z), 
    door_rotation(door_rotation) {

    std::vector<std::string> temporary = Utilities::split(this->height_map, (char)13);

    this->map_size_x = temporary[0].length();
    this->map_size_y = temporary.size();

    //this->squares = new int[map_size_x * map_size_y];
    //this->square_char = new std::string[map_size_x * map_size_y];
    //this->square_height = new double[map_size_x * map_size_y];

    for (int y = 0; y < map_size_y; y++) {

        if (y > 0) {
            temporary[y] = temporary[y].substr(1);
        }

        for (int x = 0; x < map_size_x; x++) {

            this->squares[x][y] = 1;

            std::string square = temporary[y];
            square = square.substr(x).substr(0, 1);

            boost::algorithm::trim(square);
            boost::algorithm::to_lower(square);

            if (Utilities::isNumber(square)) {
                this->square_height[x][y] = stoi(square);
                this->squares[x][y] = OPEN;
            }
            else if (square == "x") {
                this->squares[x][y] = CLOSED;
            }

            if (this->door_x == x && this->door_y == y) {
                this->squares[x][y] = OPEN;
                this->square_height[x][y] = this->door_z;
            }

            this->square_chars[x][y] = square;
        }
    }

    std::stringstream ss;

    for (int y = 0; y < map_size_y; y++) {
        for (int x = 0; x < map_size_x; x++) {

            if (x == this->door_x && y == this->door_y) {
                ss << this->door_z;
            }
            else {
                ss << this->square_chars[x][y];
            }
        }

        ss << (char)13;
    }


    this->floor_map = ss.str();
}

std::string RoomModel::getSquareChar(int x, int y) {
    return this->square_chars[x][y];
}

double RoomModel::getSquareHeight(int x, int y) {
    return this->square_height[x][y];
}

const bool RoomModel::isValidSquare(int x, int y) {
    return squares[x][y] == 0;
}

std::map<int, std::map<int, int>> &RoomModel::getSquares() {
    return this->squares;
}

const int RoomModel::getRandomX() {
    return Icarus::getRandomNumber(0, this->map_size_x);
}

const int RoomModel::getRandomY() {
    return Icarus::getRandomNumber(0, this->map_size_y);
}

/*
Deconstructor for RoomModel
*/
RoomModel::~RoomModel() { }